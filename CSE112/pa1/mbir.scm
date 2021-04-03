#!/afs/cats.ucsc.edu/courses/cse112-wm/usr/racket/bin/mzscheme -qr
;; $Id: mbir.scm,v 1.9 2021-01-12 11:57:59-08 - - $
;;
;; NAME
;;    mbir.scm filename.mbir
;;
;; SYNOPSIS
;;    mbir.scm - mini basic interper
;;
;; DESCRIPTION
;;    The file mentioned in argv[1] is read and assumed to be an mbir
;;    program, which is the executed.  Currently it is only printed.
;;

(define *DEBUG* #f)
(define *STDIN* (current-input-port))
(define *STDOUT* (current-output-port))
(define *STDERR* (current-error-port))
(define *ARG-LIST* (vector->list (current-command-line-arguments)))

(define *stmt-table*     (make-hash))
(define *function-table* (make-hash))
(define *var-table*      (make-hash))
(define *array-table*    (make-hash))
(define *label-table*    (make-hash))

(for-each
    (lambda (symfun) (hash-set! *function-table* (car symfun) (cadr symfun)))
    `(
        (= ,=)
        (< ,<)
        (> ,>)
        (<= ,<=)
        (!= ,(lambda (x y) (not (equal? x y))))
        (+    ,+)
        (-    ,-)
        (*    ,*)
        (/    ,/)
        (^    ,expt)
        ; (asub, asub)
        (sqrt ,sqrt)
        (sqr  ,sqr)
        (atan ,atan)
        (exp ,exp)
        (log ,log)
        (sin ,sin)
        (cos ,cos)
        (tan ,tan)
        (acos ,acos)
        (asin ,asin)
        (atan ,atan)
        (abs ,abs)
        (ceil ,ceiling)
        (floor ,floor)
        (round ,round)        

    )
)


(define NAN (/ 0.0 0.0))


;hash-set takes 3 args. table, key, val
(for-each (lambda (var) (hash-set! *var-table* (car var) (cadr var)))
   `(
        ;(nameOfVar, valueOfVar)
        ;comma in front of expression evaluates everything INSIDE QUASI QUOTE

        ;e = VALUE OF exp 1.0
        (e    ,(exp 1.0))
        (eof  0.0)
        (nan  ,(/ 0.0 0.0))
        (pi   ,(acos -1.0))
    )
)

(define *RUN-FILE*
    (let-values
        (((dirname basename dir?)
            (split-path (find-system-path 'run-file))))
        (path->string basename)))

(define (die list)
    (for-each (lambda (item) (fprintf *STDERR* "~a " item)) list)
    (fprintf *STDERR* "~n")
    (when (not *DEBUG*) (exit 1)))


;print all args being dumped
(define (dump . args)
    (when *DEBUG*
        (printf "DEBUG:")
        (for-each (lambda (arg) (printf " ~s" arg)) args)
        (printf "~n")))

(define (usage-exit)
    (die `("Usage: " ,*RUN-FILE* " [-d] filename")))


;line is top level object
;
(define (line-number line)
    (car line))

(define (line-label line)
    ;tail = cdr line
    (let ((tail (cdr line)))
        ; and if tail != NULL
        (and (not (null? tail))
            ; 
              (symbol? (car tail))
              (car tail)
        )
    )
)

;extract a statement from a line
(define (line-stmt line)
    (let ((tail (cdr line)))
        ; if tail is null, return false
        (cond ((null? tail) #f)
                ;if car tail is pair, return car tail
               ((pair? (car tail)) (car tail))
               ;if cdr of tail == NULL
               ((null? (cdr tail)) #f)
               ; 
               (else (cadr tail))
        )
    )
)

;goal is to GET RID OF (or not use at all?) this function
;takes 2 or 3 arguments (nl is optional)
;dot means next arg become a list of the rest of the args
(define (not-implemented function args . nl)
    (printf "(NOT-IMPLEMENTED: ~s ~s)" function args)
    (when (not (null? nl)) (printf "~n"))
)




;(asub a (+ j 1))
; car expr = asub
; cadr expr = a
; caddr expr = (+ j 1)
; func=asub


;partially implemented
(define (eval-expr expr)
    ;if expression is a number, convert number to floating point
    (cond ((number? expr) (+ expr 0.0))
        ; else if expression is sybol, look up symbol in variable table
        ; hash-rep args: Where, What, what to return on failure
            ((symbol? expr) (hash-ref *var-table* expr 0.0))
            ((pair? expr) 
                (if (equal? (car expr) 'asub)
                    (vector-ref 
                        (hash-ref *array-table* (cadr expr) #f) 
                        (exact-round (eval-expr (caddr expr)))
                    )               
                    (let ((func (hash-ref *function-table* (car expr) #f))
                        (opnds (map eval-expr (cdr expr))))
                        (if (not func) NAN
                            (apply func opnds)
                        )
                    )
                )
            )
            (else NAN)
    )
) 


;take names of array
;evaluates expression
;insert into table

;we always will get asub with dim
;rounded size = (exact-round (eval-expr (caddar args)))
;name = (cadar args)
(define (interp-dim args continuation)
    (let ((temp (make-vector (exact-round (eval-expr (caddar args))) 0.0)))
         (hash-set! *array-table* (cadar args) temp)
    )
    ;prints array for testing
    ; (let ((temp2 (hash-ref *array-table* (cadar args) #f)))
    ;         (printf "please work:~s~n" temp2)
    ; )
    (interp-program continuation)
)

;could be var (insert var), index into array (insert array element)

;args = let x y
;(car args) = x
;(cadr args) = y

;(caddr (car args)) = index to reference
(define (interp-let args continuation)
    ;(let (asub a j) (asub a (+ j 1))))
    ; let x y
    ; set x to y
    ; x is a[j]

    ;LINE:(let (asub a j) (asub a (+ j 1))))

    ; args = ((asub a j) (asub a (+ j 1)))
    ; car args = (asub a j) 
    ; cadr args = (asub a (+ j 1))

    ; cdr (car args) = a = name of array to change
    ; (cadr (car args)) = j = index of array to change
    ; (eval-expr (cadr args)) = value at (asub a (+ j 1))


    ;(let (asub a max) x)

    (if (pair? (car args))
        ;(vector-set! (cadr (car args)) (caddr (car args)) (eval-expr (cadr args)))
        (vector-set! 
        (hash-ref *array-table* (cadr (car args)) #f) 
        (exact-round (eval-expr (caddr (car args))))
        (eval-expr (cadr args))
        )
        (hash-set! *var-table* (car args) (eval-expr (cadr args)))
    )
    (interp-program continuation)
)

;similar args
;lookup lable in label table and then call interp program if found
;else call interpret-program with continuation
(define (interp-goto args continuation)

    (let ((temp (hash-ref *label-table* (car args) #f)))
            (if(not temp) (interp-program continuation) (interp-program temp))
    )
)
    ; (not-implemented 'interp-goto args 'nl)
    ; (interp-program continuation)

    


    ;hash-rep args: Where, What, what to return on failure
    ; ((hash-ref *var-table* expr NAN))
    

;
(define (interp-if args continuation)
    ; (printf "args:~s~n" args)
    ; (printf "car args:~s~n" (car args))
    ; (printf "evaluted args:~s~n" (eval-expr (car args)))
    ; (printf "label:~s~n" (cadr args))
    
    ;cadr args = label
    ;car args = expression

    (if (eval-expr (car args))
        (let ((temp (hash-ref *label-table* (cadr args) #f)))
            (if(not temp) 
                (interp-program continuation) 
                (interp-program temp)
            )
        )
        (interp-program continuation)      
    )
)

;continuation = rest of program
(define (interp-print args continuation)

    (define (print item)
        ;if item == string, print it out
        (if (string? item)
            (printf "~a" item)
            ;else print a space then the item
            (printf " ~a" (eval-expr item))))

    (for-each print args)
    (printf "~n");
    (interp-program continuation)
)

(define (interp-input args continuation)
    ;if args is pair
    ;   temp = (car args)
    ;   eofCheck = eof
    ;   if(eof == 0)
    ;       take input
    ;           if input is number
    ;               add (temp,number) to var table
    ;           else if input is eof-object
    ;               hash-set eof to 1
    ;               add (temp, NAN) to var table
    ;           else
    ;               add (temp, NAN) to var table
    ;   else
    ;       add (temp, NAN) to var table
    ;
    ;   if (cdr args) is pair
    ;       (interp-input (cdr args))
    ;   else
    ;       (interp-program continuation)

    (define (readnumber key)
        (printf "key: ~s ~n" key)
        ;eofCheck = eof
        (let ((eofCheck (hash-ref *var-table* 'eof #f)))
            ;if(eof == 0)
            (if(= 0.0 eofCheck) 
                ;take input
                (let ((object (read)))
                    (cond 
                        ;if input is number
                        ((number? object) 
                            (hash-set! *var-table* key (+ object 0.0))
                        )
                        ;if input is eof-object
                        ((eof-object? object) 
                            (begin 
                                (hash-set! *var-table* 'eof 1)
                                (hash-set! *var-table* key (/ 0.0 0.0))
                            )
                            ; (hash-set! *var-table* eof 1)
                            ;(printf "set eof to 1~n")
                            ;(hash-set! *var-table* key (/ 0.0 0.0))
                        )
                        (else (begin 
                                (hash-set! *var-table* key (/ 0.0 0.0))
                              )
                        ) 
                    )
                )
                (hash-set! *var-table* key (/ 0.0 0.0))
            )
        )            
    )
    
    (if (pair? args)
        (let ((temp (car args)))
            (readnumber temp)
        )
        (printf "args is invalid~n")
    )
    (if (pair? (cdr args))
        (interp-input (cdr args) continuation)
        (interp-program continuation)
    )
    
)

(for-each (lambda (fn) (hash-set! *stmt-table* (car fn) (cadr fn)))
   `(
        (dim   ,interp-dim)
        (let   ,interp-let)
        (goto  ,interp-goto)
        (if    ,interp-if)
        (print ,interp-print)
        (input ,interp-input)
    ))

(define (interp-program program)
    (when (not (null? program))
          (let ((line (line-stmt (car program)))
                (continuation (cdr program)))
                ;if line is true
               (if line
                    ;grab func from *stmt-table*
                   (let ((func (hash-ref *stmt-table* (car line) #f)))
                        (func (cdr line) continuation))
                   (interp-program continuation)))))

; (define (scan-for-labels program)
;     ; (let ((temp (car program)))
;     ;      (printf "car: ~s~n" temp))
;     ; (let ((temp (cdr program)))
;     ;     (printf "cdr: ~s~n" temp))
;     ;(not-implemented 'scan-for-labels '() 'nl)

;     (define (dump-line prog)
;         ; (dump (line-number line) (line-label line) (line-stmt line)))
;         (if (null? prog)
;             (printf"end of prog~n")
;             (let ((temp (car prog)))
;                 (if (line-number temp) 
;                     (printf "number: ~s~n" (line-number temp)) 
;                     (printf "number: none~n")
;                 )
;                 (if (line-label temp) 
;                     ( (printf "label found: ~s~n" (line-label temp)) (dump-line (cdr prog)) )
;                     (dump-line (cdr prog)) 
;                 )
;             )
            
;         )
;     )
    
;     ;lambda takes line as argument. dump-line for each line in program?
;     ; (for-each (lambda (line) (dump-line line)) program)
;     (printf"dumping~n")
;     (dump-line program)

; )

(define (scan-for-labels program)
    (define (get-label line)
        (and (not (null? line))
             (not (null? (cdr line)))
             (cadr line)
        )
    )
    (when (not (null? program))
          (let ((label (get-label (car program))))
               (when (symbol? label)
                     (hash-set! *label-table* label program)
                )
          )
          (scan-for-labels (cdr program))
    )
)

(define (readlist filename)
    ;inputfile = filename
    (let ((inputfile (open-input-file filename)))
        ;if inputfile != inputport, "open failed"
         (if (not (input-port? inputfile))
             (die `(,*RUN-FILE* ": " ,filename ": open failed"))
             ;else program reads inputfile
             (let ((program (read inputfile)))
                  (close-input-port inputfile)
                         program))))

(define (dump-program filename program)
    (define (dump-line line)
        (dump (line-number line) (line-label line) (line-stmt line)))
    (dump *RUN-FILE* *DEBUG* filename)
    (dump program)
    ;lambda takes line as argument. dump-line for each line in program?
    (for-each (lambda (line) (dump-line line)) program))

(define (main arglist)
    (cond ((null? arglist)
    ; 1st condition: if arglist is null, do usage exit
                (usage-exit))
        ; else if car arglist == "-d"
        ; "string=?" is string == "-d"
          ((string=? (car arglist) "-d")
                ; set debug to true
                (set! *DEBUG* #t)
                ; print this thing. ~a is the arg, -s is string with quotes
                (printf "~a: ~s~n" *RUN-FILE* *ARG-LIST*)
                ; tail call. "continues on"
                (main (cdr arglist)))
        ; else if cdr arglist != null
          ((not (null? (cdr  arglist)))
                (usage-exit))
        ; else var mbprogfile = car arglist
        ;let* is in order. "let" does in parallel
        (else (let* ((mbprogfile (car arglist))
                    ; var program = readlist mbprogfile = readlist car arglist
                       (program (readlist mbprogfile)))
                ; when debug is true, debug program
                ; 
                (begin (when *DEBUG* (dump-program mbprogfile program))
                        (scan-for-labels program)
                        (interp-program program)
                )
              )
        )
    )
)


;define (name arg1 arg2 arg...)
;   define(privateFuncName arg)
;       func


;dump . args = dump with var args

(main *ARG-LIST*)

