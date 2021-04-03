#!/afs/cats.ucsc.edu/courses/cse112-wm/usr/smalltalk/bin/gst -f
"$Id: mbint.st,v 1.16 2021-02-11 19:21:43-08 - - $"

Object subclass: NumberStream [
   |atEndFlag stream buffer|
   NumberStream class >> new [
      ^ super new init: stdin.
   ]
   NumberStream class >> new: aStream [
      ^ super new init: aStream.
   ]
   init: aStream [
      atEndFlag := false.
      stream := aStream.
      buffer := OrderedCollection new.
   ]
   atEnd [ ^ atEndFlag ]
   nextNumber [
      [atEndFlag] whileFalse: [
         buffer size > 0 ifTrue: [
            ^ buffer removeFirst asNumber asFloatD.
         ].
         stdout flush.
         stderr flush.
         stream atEnd "This condition fills the input buffer."
            ifTrue: [atEndFlag := true]
            ifFalse: [buffer := stream nextLine substrings: ' ']
      ].
      ^ 0.0 / 0.0 "NaN".
   ]
].


Object subclass: Debug [
   level := 0. "multiple -d, -dd, -ddd options set debug level"
   Debug class >> incr [level := level + 1]
   Debug class >> > value [^ level > value]
].

Object subclass: MiniBasic [                             
   nl := Character nl.
   MiniBasic class >> unimplemented: object [                     
      |caller|
      caller := thisContext parentContext.
      stdout << object class << ' inherits '
             << caller method << ': ' << object << nl.
   ] 
   prefix [^ '(', self class printString, ' ']
   assertionFailed: message [
      stderr << 'Assertion Failed: ' << message << nl.
      stderr << self << nl.
      ObjectMemory quit: 1.
   ]
].

MiniBasic subclass: Expr [ "abstract base class"
   nan := 0.0 / 0.0.
   printOn: file [^ file << self prefix << ')']
   value [MiniBasic unimplemented: self. ^ nan]              "Needs to be implented, overwritten in subclasses"      
].

"27 min in feb 11 lecture"
"take the class Expr that we've already defined and add more stuff to it"
Expr extend [
   Expr class >> parse: expr [ "parse just rearranges things"
      expr isNumber ifTrue: [^ NumExpr new: expr]. "create a new numeric object"
      expr isSymbol ifTrue: [^ VarExpr new: expr]. "create a new variable object"
      expr isArray ifTrue: [
         expr size = 2 ifTrue: [
            ^ UnopExpr new: (expr at: 1)
                       with: (Expr parse: (expr at: 2)).
         ].
         expr size = 3 ifTrue: [
            ((expr at: 1) = #aget) | ((expr at: 1) = #aput)
            ifTrue: [
               " stdout << 'hello!!!!!!!!' << nl. "
               ^ BinopExpr new: (expr at: 1)
                           with: (ArrayExpr new: (expr at: 2))
                           with: (Expr parse: (expr at: 3)).
            ]
            ifFalse: [
               ^ BinopExpr new: (expr at: 1)
                           with: (Expr parse: (expr at: 2))
                           with: (Expr parse: (expr at: 3)).
            ]
         ].
         self assertionFailed: 'Expr>>parse: isArray wrong size'.
      ].
      self assertionFailed: 'Expr>>parse: expr wrong type'. "this means there is something wrong"
   ]
].

Expr subclass: NumExpr [   "already works"
   |value|
   NumExpr class >> new: val [^ super new init: val]        
     "class method that creates a new value. having created the class variable, thus creating a new object "
   init: val [value := val + 0.0d0]                         
      "instance method. takes val, adds 0.0, and stores it in the value of the object expression"
   printOn: file [^ file << self prefix << value << ')']    
      "used for debugging"
   value [^ value + 0.0]                                    
      "evaluate it and return the stored value"
]. 

Expr subclass: VarExpr [   "already works"
   |variable|
      "variable is declared as a variable local to the instance"
   varDict := Dictionary from: {                            
      "dictionary that contains our variables. store new variables here"
      "initialized from each one of the following associations: symbol(key) -> assocaiated value."
      "class field within a variable. shared with all elements / 11:45 in Feb 11 lecture"
      #e   -> 1.0 exp.
      #eof -> 0.0.
      #nan -> (0.0 / 0.0).
      #pi  -> -1.0 arcCos.

   }.
   VarExpr class >> new: var [^ super new init: var]
      "class method new which creates the object"          
   init: var [variable := var]
      "instance method init which actually initializes the object itself"
      "many of these classes follow the same format of constructors
         - class method called new to create the object
         - instance method called init to initialize the object itself"
   printOn: file [^ file << self prefix << variable << ')']
      "debug statement. display a VarExpr object itself"

      "TWO IMPORTANT THINGS"
   put: val [varDict at: variable put: val]           
      "if you send a put message to a variable, you want it to remember its new value"
      "whatever val is, is stored into the varDict, at the location specified by the variable itself"
      "instance method of a variable expression. takes a value as an argument and executes the following 'value' block"
   value [^ varDict at: variable ifAbsent: [0.0]]
      "looks up the value of a variable"
      "if no such variable exists, return a variable with 0.0"
].
   "VarExpr has 2 functions (methods): 
      -value : return a variable using VarDict and a given key, ifAbsent returns 0 for unitialized variables
      -put : takes a value as an argument, and stores that variable in the varDict using the variable passed as an argument (key)
             
   "

"talks abut UnopExpr at 13:35 in Feb 11 lecture"
Expr subclass: UnopExpr [                                   
   |oper opnd|
   UnopExpr class >> new: op with: opn [
      ^ super new init: op init: opn.
   ]
   init: op init: opn [
      oper := op. opnd := opn.
   ]
   printOn: file [                                       "Implement Value function"
      ^ file << self prefix << oper << ' ' << opnd << ')'.
   ]
   value [^ opnd value perform: oper "+ 0.0".]

         "NOTES FROM MACKEY LECTURE"
   "Add a value method here to evaluate the unary operator"
   "contains a single expression(operator)  and a single operand"
   "
      Value method:
         - send the value message to the operand and perform the result with the operator             examples in previous lecture he says
   "
   "declare local variables val1 and val2"

   "sin vs #sin
      - #sin is a symbol
      - sin is a variable
   "
].

Expr subclass: BinopExpr [                                   
   |oper opnd1 opnd2|
   BinopExpr class >> new: op with: opn1 with: opn2 [
      ^ super new init: op with: opn1 with: opn2.
   ]
   init: op with: opn1 with: opn2 [
      stdout << 'oper' << nl.
      oper inspect.
      stdout << 'opnd1' << nl.
      opnd1 inspect.
      stdout << 'opnd2' << nl.
      opnd2 inspect.

      oper := op. opnd1 := opn1. opnd2 := opn2.

      " stdout << 'oper' << nl.
      oper inspect.
      stdout << 'opnd1' << nl.
      opnd1 inspect.
      stdout << 'opnd2' << nl.
      opnd2 inspect. "
   ]
   printOn: file [                                       "Implement Value function"
      ^ file << self prefix << oper << ' ' << opnd1
             << ' ' << opnd2 << ')'.
   ]
   value [^ opnd1 value perform: oper with: opnd2 value.] "https://www2.ucsc.edu/courses/cse112-wm/:/Assignments/asg3-smalltalk-mbst/misc-evalexpr/evalexpr.st"
   
   getOne [^ opnd1]
   getTwo [^ opnd2]
   getOp [^ oper]


         "OLD NON-FUNCTIONAL CODE"
   " value [^ opnd1 perform: oper with: opnd2.] "
   " a := [^ (opnd1)].
   b := opnd2.
   c := oper. "
   "value [^ (opnd1 perform: oper with: opnd2) + 0.0.]"
   

   
         "NOTES FROM MACKEY LECTURE"
   "Same as UnopExpr but for a binary operation eg 1 + 1"
      "
      Value method:
         - in this case, send the value method to opnd1 & opnd2
         -
   "
].


"talks about arrayexpr" "35 minutes, feb 11 lec"
"very similar to a varExpr, except it contains the name of an array"
"containing an array name and an array dictionary"
"stores the name of the array along with the array value itself, in the array dictionary"
Expr subclass: ArrayExpr [
   |arrayName arrayValue|
   arrayDict := Dictionary new.
   ArrayExpr class >> new: var [^ super new init: var]
   " init:need to also store array name in variable but also add in another parameter to accept dim of array" "26 min in feb 11 lecture"
   init: var [
      arrayName := var.
   ]

   make: var2[
      arrayValue := Array new: var2.
      1 to: var2 do: [:index|
         arrayValue at: index put: 0.
      ].
      arrayDict at: arrayName put: arrayValue.
      arrayDict inspect.
   ]

   aput: index with: val[
      |tempArr|
      tempArr := arrayDict at: arrayName.
      tempArr at: index put: val.
      tempArr inspect.
   ]

   aget: index [
      |tempArr|
      tempArr := arrayDict at: arrayName.
      [^ tempArr at: index]
   ]

   value [
      [^ (arrayDict at: arrayName)]
   ]
   printOn: file [^ file << self prefix << arrayName << ')']
].

      "31:15 in feb 11 lecture:
         -add a method dimension
            - when we see a dimension statement, it is going to have args (ArrayExpr #a) (VarExpr #size)
            - needs to send the value message to its second argument
            - then it needs to send a dimension message with that to the array expression itself

         -dimension takes a number as an argument
            - array new with the dimension as an argument
            - store that new array in the arrayDict
            - right a loop that sets all the list elements to 0

         
         -aget/aput, future stuff, need to actually fetch the array, so we add in a method get
            - when get is sent to an array expression, it returns a particular array itself so that it may be updated
      "


   


MiniBasic subclass: Stmt [ "abstract base class"
   stmtNr := 1.
   labelDict := Dictionary new.
   "FOR FUTURE USE in GoTo       60 min in feb 11 lecture"
   Stmt class >> getStmtNr [^ stmtNr]
   Stmt class >> decrStmtNr [stmtNr := stmtNr - 1]
   Stmt class >> incrStmtNr [stmtNr := stmtNr + 1]
   Stmt class >> addLabel: label to: line [labelDict at: label put: line]
   " Stmt class >> setStmtNr: label [stmtNr := labelDict at: label] "
   Stmt class >> setStmtNr: label [stmtNr := labelDict at: label]


   Stmt class >> inspectLabels [labelDict inspect]


   printOn: file [^ file << self prefix << ')']
   interp [MiniBasic unimplemented: self.]     

].



"-add a method dimension
            - when we see a dimension statement, it is going to have args (ArrayExpr #a) (VarExpr #size)
            - needs to send the value message to its second argument
            - then it needs to send a dimension message with that to the array expression itself

         -dimension takes a number as an argument
            - array new with the dimension as an argument
            - store that new array in the arrayDict
            - right a loop that sets all the list elements to 0"
Stmt subclass: DimStmt [
   |name size|
   DimStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [
      name := ArrayExpr new: (stmt at: 2).
      size := Expr parse: (stmt at: 3).
   ]
   printOn: file [^ file << self prefix << name << ' ' << size << ')']

   interp [
      name make: ((size value) asInteger).
   ]
].
         "NOTES FROM MACKEY LECTURE"
   "DimStmt takes an array name (ArrayExpr) and a size (Expr)"



Stmt subclass: LetStmt [
   |name expr|
   LetStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [
      name := Expr parse: (stmt at: 2).
      expr := Expr parse: (stmt at: 3).
   ]
   printOn: file [^ file << self prefix << name << ' ' << expr << ')']
   interp [
      " stdout << 'new' << nl.
      name inspect.
      expr inspect. "
      (name isKindOf: BinopExpr) 
      ifTrue: [
         |arrName arrInd arrVal|


         ((name getOp) = #aput) 
            ifTrue: [
               arrName := (name getOne). 
               arrInd := ((((name getTwo) value) asInteger) + 1).  
               arrVal := expr value.
               arrName aput: arrInd with: arrVal.
            ].
         ((name getOp) = #aget) 
            ifTrue: [
               stdout << 'test' << nl.
               " name inspect.
               expr inspect.
               stdout << 'test' << nl. "
               " arrName := (name getOne). 
               stdout << 'arrName:' << arrName << nl.   
               arrInd := ((((name getTwo) value) asInteger) + 1).  
               stdout << 'arrInd:' << arrInd << nl.
               arrName aget: arrInd.     "
            ].
      ]
      ifFalse: [
         name put: expr value.
      ]
      " 
      if name is BinopExpr:
         |arrName arrInd arrVal|
         arrName = (name opnd1)  //arrName = a
         arrInd = (name opnd2)   //arrInd = max
         arrVal = expr value     //arrVal = x
         arrName aput: arrInd with: arrVal.
       "

      ]
].

Stmt subclass: GotoStmt [
   |label|
   GotoStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [label := stmt at: 2]
   printOn: file [^ file << self prefix << label << ')']
   interp [
      Stmt setStmtNr: label.
   ]
].

Stmt subclass: IfStmt [
   |expr label|
   IfStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [
      expr := Expr parse: (stmt at: 2).
      label := stmt at: 3.
   ]
   interp [
      expr value ifTrue: [
         Stmt setStmtNr: label.   
      ].
   ]
   printOn: file [^ file << self prefix << expr << ' ' << label << ')']
].

Stmt subclass: InputStmt [
   |inputVars|
   stdinNumbers := NumberStream new.
   InputStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [
      |args|
      args := stmt at: 2.
      inputVars := Array new: args size.
      1 to: args size do: [:index|
         inputVars at: index put: (Expr parse: (args at: index)).
      ]
   ]
   printOn: file [^ file << self prefix << inputVars << ')']
   interp [
      inputVars do: [:var|
         |value|
         value := stdinNumbers nextNumber.

         stdinNumbers atEnd ifTrue:[
            |temp|
            temp := VarExpr new: #eof.
            temp put: 1.0.
         ].
         
         var put: value.
      ].
   ]
].

Stmt subclass: PrintStmt [
   |printables|
   PrintStmt class >> new: stmt [^ super new init: stmt]
   init: stmt [
      |args|
      args := stmt at: 2.
      printables := Array new: args size.
      1 to: args size do: [:index|
         |arg|
         arg := args at: index.
         arg isString ifFalse: [ arg := Expr parse: arg].
         printables at: index put: arg.
      ]
   ]
   printOn: file [^ file << self prefix << printables << ')']
   interp [
      Debug > 2 ifTrue: [printables inspect].
      printables do: [:obj|
         obj isString ifTrue: [stdout << obj]
                      ifFalse: [stdout << ' hellllooooo ' << obj value].
      ].
      stdout << nl.
   ]
].

Stmt subclass: NullStmt [
   NullStmt class >> new: stmt [^ super new]
   printOn: file [^ file << self prefix << ')']
   interp []
].

Stmt extend [
   stmtDict := Dictionary from: {
      #dim   -> DimStmt.
      #let   -> LetStmt.
      #goto  -> GotoStmt.
      #if    -> IfStmt.
      #input -> InputStmt.
      #print -> PrintStmt.
      #null  -> NullStmt.
   }.
   Stmt class >> parse: stmt [
      |stmtSym stmtClass|
      stmt size = 0 ifTrue: [stmtSym := #null]
                    ifFalse: [stmtSym := stmt at: 1].
      stmtClass := stmtDict at: stmtSym.
      ^ stmtClass new: stmt.
   ]
]

MiniBasic subclass: Interpreter [
   |statements|
   Interpreter class >> new: program [^ super new init: program]

   init: program [      "creates a new program, which is an array of statments"
      statements := Array new: program size.
      1 to: program size do: [:index|
         |stmt parsed label line|
         stmt := (program at: index) at: 3.
         statements at: index put: (Stmt parse: stmt).


         label := (program at: index) at: 2.
         line := (program at: index) at: 1.
         label isSymbol ifTrue: [Stmt addLabel: label to: index].

      ].
   ]

   print [             "purely for debugging purposes"
      stdout << 'Interpreter statements: [' << nl.
      1 to: statements size do: [:index|
         stdout << '   ' << index << ': '
                << (statements at: index) << nl.
      ].
      stdout << '].' << nl.
   ]

   interpret [          "actual interpreter"
      |stmtNr|
      [stmtNr := Stmt getStmtNr. stmtNr <= statements size]
      whileTrue: [
         |stmtObj|
         Stmt incrStmtNr.
         stmtObj := statements at: stmtNr.
         Debug > 1 ifTrue: [stdout << stmtNr << ': ' << stmtObj << nl].
         stmtObj interp.      "something like let or print or if or goto or..."
                              "if it's a goto,"
      ]
   ]


].

Object subclass: Main [
   nl := Character nl.
   scriptName := thisContext currentFileName.

   Main class >> usage [
      stderr << 'Usage: ' << scriptName << ' [-d] fileName' << nl.
      ObjectMemory quit: 1.
   ]

   Main class >> print: program [
      stdout << 'Main program: [' << nl.
      1 to: program size do: [:index|
         stdout << '   ' << index << ': ' << (program at: index) << nl.
      ].
      stdout << '].' << nl.
   ]

   Main class >> getopts [
      |fileName|
      Smalltalk arguments: '-d' do: [:opt :arg|
         opt = $d ifTrue: [Debug incr].
         opt = nil ifTrue: [
            fileName isNil ifFalse: [Main usage value].
            fileName := arg.
         ].
      ] ifError: [Main usage value].
      Debug > 0 ifTrue: [
         stdout << scriptName << ': ' << Smalltalk arguments << nl.
      ].
      ^ fileName.
   ]

   Main class >> main [
      |fileName|
      fileName := Main getopts.
      fileName isNil ifTrue: [Main usage value].
      [
         |program interpreter|
         FileStream fileIn: fileName.
         program := Program get.
         Debug > 0 ifTrue: [Main print: program].
         interpreter := Interpreter new: program.
         Debug > 0 ifTrue: [interpreter print].
         "76 min in feb11 lecture"
         interpreter interpret.
      ] on: SystemExceptions.FileError do: [:signal|
         stderr << scriptName << ': ' << fileName << ': '
                << signal messageText << nl.
         ObjectMemory quit: 1.
      ].
   ]
].

Main main.


" NOTES "


"SCANNING FOR LABELS, IMPORTANT"
"https://piazza.com/class/khgog3bf8wc67v?cid=251"

"
Model debug output with basic explanation:

bash-4.2$ mbint.st -d files/00-hello-world.mbst 
mbint.st: ('-d' 'files/00-hello-world.mbst' )

      DUMP OF THE MAIN PROGRAM
Main program: [
   1: (2 nil (#print ('Hello, World!' ) ) )
]. 
               - essentialy a set of arrays
               - nested arrays

Interpreter statements: [
   1: (PrintStmt ('Hello, World!' ))
].
               - represent the actual objects
               - in this case, we have one obj, PrintStmt

Hello, World!
"

"RULES OF THIS PROJECT
   -Uninitialized variables automatically become 0, so we don't need to worry about them
   -
"


"NEED TO REPLACE INHERITED CALLS TO unimplemented"


"Print using printOn: "


" 
ARRAY NOTES:
- As shown below, when putting at element at index 1, it puts it at the first element which is traditionally 0
- Test programs in minibasic assume index 0, so we'll need +1 and -1 in different places in the interpreter to compensate for this

Code Example:
st > a:= Array new: 5
      (nil nil nil nil nil)

st > a at:1 put: 10
   
st > a
        (10 nil nil nil nil)
 
"



"38-43 mins i zoned out coding, could rewatch"




