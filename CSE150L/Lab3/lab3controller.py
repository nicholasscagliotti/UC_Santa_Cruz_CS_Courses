# Lab 3 Skeleton
#
# Based on of_tutorial by James McCauley

from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Firewall (object):
  """
  A Firewall object is created for each switch that connects.
  A Connection object for that switch is passed to the __init__ function.
  """
  def __init__ (self, connection):
    # Keep track of the connection to the switch so that we can
    # send it messages!
    self.connection = connection

    # This binds our PacketIn event listener
    connection.addListeners(self)

  def do_firewall (self, packet, packet_in):
    # The code in here will be executed for every packet.
    arp = packet.find('arp')    # Detect the packets we want, everything else
    tcp = packet.find('tcp')    # will be dropped and we don't need to specify
    ipv4 = packet.find('ipv4')  # the type of traffic

    def flood (message = None):
      """Floods the packet"""
      msg = of.ofp_flow_mod()
      msg.match = of.ofp_match.from_packet(packet)
      msg.idle_timeout = 25000  # Set arbitrarily high to ensure dump-flow
      msg.hard_timeout = 50000  # command works
      msg.buffer_id = packet_in.buffer_id
      msg.actions.append(of.ofp_action_output(port = of.OFPP_FLOOD))
      #print "Flood"             
      self.connection.send(msg)

    def drop (duration = None):
      msg = of.ofp_flow_mod()
      msg.match = of.ofp_match.from_packet(packet)
      msg.idle_timeout = 25000
      msg.hard_timeout = 50000
      msg.buffer_id = packet_in.buffer_id 
      #print "Drop"              
      self.connection.send(msg)
    
    if arp is not None:         # Checking for arp packets
      flood()
      print "Flood arp"         # Test to see if we flooded the arp packet
    elif tcp is not None and ipv4 is not None:    # Checking for tcp + ipv4 packets
      flood()
      print "Flood tcp ipv4"    # Test to see if we flooded the tcp/ipv4 packet
    else:                       # Dump anything that isn't the above traffic type
      drop()                    # Test to see if we dropped the packet
      print "Drop everything else"

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """

    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_firewall(packet, packet_in)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Firewall(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
