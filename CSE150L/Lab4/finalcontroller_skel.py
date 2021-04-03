# Final Skeleton
#
# Hints/Reminders from Lab 3:
#
# To check the source and destination of an IP packet, you can use
# the header information... For example:
#
# ip_header = packet.find('ipv4')
#
# if ip_header.srcip == "1.1.1.1":
#   print "Packet is from 1.1.1.1"
#
# Important Note: the "is" comparison DOES NOT work for IP address
# comparisons in this way. You must use ==.
# 
# To send an OpenFlow Message telling a switch to send packets out a
# port, do the following, replacing <PORT> with the port number the 
# switch should send the packets out:
#
#    msg = of.ofp_flow_mod()
#    msg.match = of.ofp_match.from_packet(packet)
#    msg.idle_timeout = 30
#    msg.hard_timeout = 30
#
#    msg.actions.append(of.ofp_action_output(port = <PORT>))
#    msg.data = packet_in
#    self.connection.send(msg)
#
# To drop packets, simply omit the action.
#

from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Final (object):
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

  def do_final (self, packet, packet_in, port_on_switch, switch_id):
    ipv4 = packet.find('ipv4')
    if packet.find('ipv4'):
      srcip = ipv4.srcip
      dstip = ipv4.dstip
      print "Source IP:"
      print srcip
      print "Destination IP:"
      print dstip
    arp = packet.find('arp')
    tcp = packet.find('tcp')
    icmp = packet.find('icmp')
    pos = port_on_switch
    s_id = switch_id


    def flood (message = None):
      msg = of.ofp_flow_mod()
      msg.match = of.ofp_match.from_packet(packet)
      msg.idle_timeout = 2500
      msg.hard_timeout = 50000
      msg.buffer_id = packet_in.buffer_id
      msg.actions.append(of.ofp_action_output(port = of.OFPP_FLOOD))
      self.connection.send(msg)

    def drop (duration = None):
      msg = of.ofp_flow_mod()
      msg.match = of.ofp_match.from_packet(packet)
      msg.idle_timeout = 2500
      msg.hard_timeout = 50000
      msg.buffer_id = packet_in.buffer_id
      self.connection.send(msg)


    if packet.find('ipv4'):
      if srcip == "10.1.1.10":        #h10
        if dstip == "10.2.5.50":
          drop()
        elif dstip == "10.2.6.60":
          drop()
        elif dstip == "10.2.7.70":
          drop()
        elif dstip == "10.2.8.80":
          drop()
        elif dstip == "10.1.2.20":
          flood()
        elif dstip == "10.1.3.30":
          flood()
        elif dstip == "10.1.4.40":
          flood()

      if srcip == "10.1.2.20":        #h20
        if dstip == "10.2.5.50":
          drop()
        elif dstip == "10.2.6.60":
          drop()
        elif dstip == "10.2.7.70":
          drop()
        elif dstip == "10.2.8.80":
          drop()
        elif dstip == "10.1.1.10":
          flood()
        elif dstip == "10.1.3.30":
          flood()
        elif dstip == "10.1.4.40":
          flood()
      
      if srcip == "10.1.3.30":        #h30
        if dstip == "10.2.5.50":
          drop()
        elif dstip == "10.2.6.60":
          drop()
        elif dstip == "10.2.7.70":
          drop()
        elif dstip == "10.2.8.80":
          drop()
        elif dstip == "10.1.1.10":
          flood()
        elif dstip == "10.1.2.20":
          flood()
        elif dstip == "10.1.4.40":
          flood()

      if srcip == "10.1.4.40":        #h40
        if dstip == "10.2.5.50":
          drop()
        elif dstip == "10.2.6.60":
          drop()
        elif dstip == "10.2.7.70":
          drop()
        elif dstip == "10.2.8.80":
          drop()
        elif dstip == "10.1.1.10":
          flood()
        elif dstip == "10.1.3.30":
          flood()
        elif dstip == "10.1.2.20":
          flood()

      if srcip == "10.2.5.50":        #h50
        if dstip == "10.2.6.60":
          flood()
        elif dstip == "10.2.7.70":
          flood()
        elif dstip == "10.2.8.80":
          flood()
        elif dstip == "10.1.1.10":
          drop()
        elif dstip == "10.1.2.20":
          drop()
        elif dstip == "10.1.3.30":
          drop()
        elif dstip == "10.1.4.40":
          drop()
      
      if srcip == "10.2.6.60":        #h60
        if dstip == "10.2.5.50":
          flood()
        elif dstip == "10.2.7.70":
          flood()
        elif dstip == "10.2.8.80":
          flood()
        elif dstip == "10.1.1.10":
          drop()
        elif dstip == "10.1.2.20":
          drop()
        elif dstip == "10.1.3.30":
          drop()
        elif dstip == "10.1.4.40":
          drop()

      if srcip == "10.2.7.70":        #h70
        if dstip == "10.2.6.60":
          flood()
        elif dstip == "10.2.5.50":
          flood()
        elif dstip == "10.2.8.80":
          flood()
        elif dstip == "10.1.1.10":
          drop()
        elif dstip == "10.1.2.20":
          drop()
        elif dstip == "10.1.3.30":
          drop()
        elif dstip == "10.1.4.40":
          drop()  

      if srcip == "10.2.8.80":        #h80
        if dstip == "10.2.6.60":
          flood()
        elif dstip == "10.2.7.70":
          flood()
        elif dstip == "10.2.5.50":
          flood()
        elif dstip == "10.1.1.10":
          drop()
        elif dstip == "10.1.2.20":
          drop()
        elif dstip == "10.1.3.30":
          drop()
        elif dstip == "10.1.4.40":
          drop()    

      if srcip == "106.44.82.103":    #h_untrust
        drop()
      
      if srcip == "108.24.31.112":    #h_trust
        if dstip == "10.1.1.10":
          flood()
        if dstip == "10.1.2.20":
          flood()
        if dstip == "10.1.3.30":
          flood()
        if dstip == "10.1.4.40":
          flood()
        if dstip == "10.2.5.50":
          drop()
        if dstip == "10.2.6.60":
          drop()
        if dstip == "10.2.7.70":
          drop()
        if dstip == "10.2.8.80":
          drop()
        if dstip == "10.3.9.90":
          drop()

    if arp is not None:
      flood()
    elif tcp is not None and ipv4 is not None:
      flood()
    elif icmp is not None:
      flood()
    else:
      drop()


    # This is where you'll put your code. The following modifications have 
    # been made from Lab 3:
    #   - port_on_switch: represents the port that the packet was received on.
    #   - switch_id represents the id of the switch that received the packet.
    #      (for example, s1 would have switch_id == 1, s2 would have switch_id == 2, etc...)
    # You should use these to determine where a packet came from. To figure out where a packet 
    # is going, you can use the IP header information.

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """
    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_final(packet, packet_in, event.port, event.dpid)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Final(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
