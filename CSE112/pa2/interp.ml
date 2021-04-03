(* $Id: interp.ml,v 1.18 2021-01-29 11:08:27-08 - - $ *)
(* Nicholas Scagliotti nscaglio Rahul Arora raarora *)

open Absyn

(* pointer to false *)
let want_dump = ref false

(* pointer to empty string *)
let source_filename = ref ""

let rec eval_expr (expr : Absyn.expr) : float = match expr with
    (* if we have number, return number *)
    | Number number -> number
    (* if we have memref, eval_memref *)
    | Memref memref -> eval_memref memref
    (* write eval_unary expression *)
    | Unary (oper, expr) -> 
    (Hashtbl.find Tables.unary_fn_table oper) (eval_expr expr) 
    (* write eval_binary expression *)
    | Binary (oper, expr1, expr2) ->
     (Hashtbl.find Tables.binary_fn_table oper) 
     (eval_expr expr1) (eval_expr expr2)


and eval_memref (memref : Absyn.memref) : float = match memref with
    | Arrayref (ident, expr) -> 
    (Hashtbl.find Tables.array_table ident).
    (int_of_float (eval_expr expr))
    (* look for identifier in hash table and return it*)
    | Variable ident -> try Hashtbl.find Tables.variable_table ident
                        with Not_found -> 0.0

(* this will be deleted *)
and eval_STUB reason = (
    print_string ("(" ^ reason ^ ")");
    nan)

let rec interpret (program : Absyn.program) = match program with
    | [] -> ()
    | firstline::continue -> match firstline with
       | _, _, None -> interpret continue  
       | _, _, Some stmt -> (interp_stmt stmt continue)

and interp_stmt (stmt : Absyn.stmt) (continue : Absyn.program) =
    match stmt with
    | Dim (ident, expr) -> interp_dim ident expr continue
    | Let (memref, expr) -> interp_let memref expr continue
    | Goto label -> interp_goto label continue
    | If (expr, label) -> interp_if expr label continue
    | Print print_list -> interp_print print_list continue
    | Input memref_list -> interp_input memref_list continue


and interp_let  (memref : Absyn.memref) 
                (expr : Absyn.expr) 
                (continue : Absyn.program) =
    match memref with
    | Arrayref (ident, expr2) -> 
    ((Hashtbl.find Tables.array_table ident).
    (int_of_float (eval_expr expr2)) 
    <- (eval_expr expr) );
    interpret continue
    | Variable ident -> 
    (Hashtbl.replace Tables.variable_table ident (eval_expr expr));
    interpret continue


and interp_dim  (ident : Absyn.ident) 
                (expr : Absyn.expr) 
                (continue : Absyn.program) =
    ( Hashtbl.replace Tables.array_table ident 
        (Array.make (int_of_float (eval_expr expr)) 0.0) 
    );
    interpret continue


and interp_goto  (label : Absyn.label) 
                 (continue : Absyn.program) =
    interpret (Hashtbl.find Tables.label_table label) 

and interp_if   (expr : Absyn.relexpr) 
                (label : Absyn.label) 
                (continue : Absyn.program)  = 
    match expr with
    | Relexpr (oper, expr1, expr2) -> 
    let temp = ((Hashtbl.find Tables.bool_fn_table oper) 
                (eval_expr expr1) 
                (eval_expr expr2))
    in
    if temp = true 
        then (interp_goto label continue)
    else
        interpret continue


and interp_print (print_list : Absyn.printable list)
                 (continue : Absyn.program) =
    let print_item item =
        match item with
        (* if string*)
        | String string ->
          let regex = Str.regexp "\"\\(.*\\)\""
          in print_string (Str.replace_first regex "\\1" string)
        (* if expr *)
        | Printexpr expr ->
          print_string " "; print_float (eval_expr expr)
     (*print_newline is function without argument *) 
    in (List.iter print_item print_list; print_newline ());
    (* after printing, calls interpret continue *)
    interpret continue



and interp_input (memref_list : Absyn.memref list)
                 (continue : Absyn.program)  =
    
    let input_number memref =
        (* read_number returns float *)
        try let number = Etc.read_number () 
            in match memref with
            | Arrayref (ident, expr) -> ()
            | Variable ident -> 
            (Hashtbl.replace Tables.variable_table ident number);

        with End_of_file -> 
            (* line below should end program as well *)
             Hashtbl.replace Tables.variable_table "eof" 1.0
             
    in List.iter input_number memref_list;
    (* print_string "Done with input_number"; *)
    interpret continue

let interpret_program program =
    (Tables.init_label_table program; 
     if !want_dump then Tables.dump_label_table ();
     if !want_dump then Dumper.dump_program program;
     interpret program)

