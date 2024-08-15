let generate_words p =
  let elements = ['x'; 'y'; 'X'; 'Y'] in

  let is_inverse a b = 
    (a = 'x' && b = 'X') || 
    (a = 'X' && b = 'x') ||
    (a = 'y' && b = 'Y') ||
    (a = 'Y' && b = 'y') 
  in

  let next_elements e = 
    List.filter (fun x -> not (is_inverse e x)) elements
  in

  let queue = Queue.create () in
  List.iter (fun e -> Queue.add (String.make 1 e, 1) queue) elements;

  let results = ref [] in

  while not (Queue.is_empty queue) do
    let (current, len) = Queue.take queue in
    if len = p then
      results := current :: !results
    else
      let next_elems = next_elements (String.get current (String.length current - 1)) in
      List.iter (fun e -> Queue.add (current ^ String.make 1 e, len + 1) queue) next_elems
  done;

  !results

(* Ta metoda je kar dobra .... *)



(* 
open Owl




let matrix_multiply () =
  (* Define two matrices *)
  let a = Mat.of_array [| [| 1.; 2.; 3. |]; 
                          [| 4.; 5.; 6. |] |] 2 3 in

  let b = Mat.of_array [| [| 7.; 8. |]; 
                          [| 9.; 10. |]; 
                          [| 11.; 12. |] |] 3 2 in

  (* Multiply the matrices *)
  let c = Mat.dot a b in

  (* Print the result *)
  Mat.print c

let () = matrix_multiply ()

type matrix = float array array

let multiply (a: matrix) (b: matrix) : matrix =
  let a_rows = Array.length a in
  let a_cols = Array.length a.(0) in
  let b_rows = Array.length b in
  let b_cols = Array.length b.(0) in

  if a_cols <> b_rows then
    failwith "Matrix dimensions do not match for multiplication";

  let result = Array.make_matrix a_rows b_cols 0. in

  for i = 0 to a_rows - 1 do
    for j = 0 to b_cols - 1 do
      for k = 0 to a_cols - 1 do
        result.(i).(j) <- result.(i).(j) +. a.(i).(k) *. b.(k).(j)
      done;
    done;
  done;

  result

let print_matrix (m: matrix) =
  Array.iter (fun row ->
      Array.iter (fun v -> Printf.printf "%f " v) row;
      print_newline ()) m

let () =
  let a = [| [| 1.; 2.; 3. |]; [| 4.; 5.; 6. |] |] in
  let b = [| [| 7.; 8. |]; [| 9.; 10. |]; [| 11.; 12. |] |] in

  let c = multiply a b in
  print_matrix c *)




type matrix = {
  a11: int;
  a12: int;
  a21: int;
  a22: int;
}

let multiply (m1: matrix) (m2: matrix) : matrix =
  {
    a11 = m1.a11 * m2.a11 + m1.a12 * m2.a21;
    a12 = m1.a11 * m2.a12 + m1.a12 * m2.a22;
    a21 = m1.a21 * m2.a11 + m1.a22 * m2.a21;
    a22 = m1.a21 * m2.a12 + m1.a22 * m2.a22;
  }

(* let print_matrix (m: matrix) =
  Printf.printf "%d %d\n%d %d\n" m.a11 m.a12 m.a21 m.a22

let () =
  let m1 = { a11 = 1; a12 = 2; a21 = 3; a22 = 4 } in
  let m2 = { a11 = 2; a12 = 0; a21 = 1; a22 = 3 } in
  
  let result = multiply m1 m2 in
  print_matrix result *)


  let det (m: matrix) =
    m.a11 * m.a22 - m.a12 * m.a21
  
  let mod_p x p =
    ((x mod p) + p) mod p
  
  let generate_sl_2_p p =
    let matrices = ref [] in
    for a11 = 0 to p - 1 do
      for a12 = 0 to p - 1 do
        for a21 = 0 to p - 1 do
          for a22 = 0 to p - 1 do
            let m = {a11; a12; a21; a22} in
            if mod_p (det m) p = 1 then
              matrices := m :: !matrices
          done
        done
      done
    done;
    !matrices
  
  let scalar_multiple lambda m p =
    {
      a11 = mod_p (lambda * m.a11) p;
      a12 = mod_p (lambda * m.a12) p;
      a21 = mod_p (lambda * m.a21) p;
      a22 = mod_p (lambda * m.a22) p;
    }
  
  let matrix_equals m1 m2 =
    m1.a11 = m2.a11 && m1.a12 = m2.a12 && m1.a21 = m2.a21 && m1.a22 = m2.a22
  
  let remove_scalar_multiples m lst p =
    let rec aux acc = function
      | [] -> acc
      | hd :: tl ->
        let to_remove = ref false in
        for lambda = 1 to p - 1 do
          if matrix_equals hd (scalar_multiple lambda m p) then
            to_remove := true
        done;
        if !to_remove then aux acc tl
        else aux (hd :: acc) tl
    in
    aux [] lst

    

(* Tole velja samo za praštevilske inpute! Pri q = p^k so jedra bolj komplicirana!! *)

  let generate_psl_2_p p =
    let sl_2_p = generate_sl_2_p p in
    let rec aux acc = function
      | [] -> acc
      | hd :: tl ->
        let reduced_tl = remove_scalar_multiples hd tl p in
        aux (hd :: acc) reduced_tl
    in
    aux [] sl_2_p
  
  
  (* Testing *)
  (* let () =
    let p = 5 in (* Example prime number *)
    let matrices = generate_psl_2_p p in
    print_endline (string_of_int (List.length matrices)) *)
(* 
    q    |   |PSL(2,q)| 
    ---------------------
     2    |       6
     3    |      12
     4    |      60
     5    |      60
     7    |     168
     8    |     504
     9    |     360
    13    |    1092
    16    |    4080
    17    |    2448
    19    |    3420
    23    |    6084
    29    |    12260
    25    |    7500 *)
    

(* Funkcija za vrednotenje dveh zakonov *)

let mod_inverse a q =
  let rec extended_gcd a b =
    if b = 0 then (1, 0)
    else let (x, y) = extended_gcd b (a mod b) in
         (y, x - (a / b) * y)
  in
  let (inv, _) = extended_gcd a q in
  (inv + q) mod q

let inverse_matrix q m =
  let det = (m.a11 * m.a22 - m.a12 * m.a21) mod q in
  let det_inv = mod_inverse det q in
  {
    a11 = (m.a22 * det_inv) mod q;
    a12 = (-m.a12 * det_inv) mod q;
    a21 = (-m.a21 * det_inv) mod q;
    a22 = (m.a11 * det_inv) mod q
  }

let eval_expression q word matA matB =
  let identity = { a11 = 1; a12 = 0; a21 = 0; a22 = 1 } in
  let rec eval_aux w result =
    match w with
    | "" -> result
    | _ ->
        let mat = match String.get w 0 with
                  | 'x' -> matA
                  | 'y' -> matB
                  | 'X' -> inverse_matrix q matA
                  | 'Y' -> inverse_matrix q matB
                  | _   -> failwith "Invalid letter"
        in
        eval_aux (String.sub w 1 (String.length w - 1)) (multiply result mat)
  in
  mod_matrix q (eval_aux word identity)

let mod_matrix p m =
  {
    a11 = m.a11 mod p;
    a12 = m.a12 mod p;
    a21 = m.a21 mod p;
    a22 = m.a22 mod p;
  }


    (* Jutri!! *)
    (* Ne pozabi! Treba je poračunati za vsa števila, ki so dovolj blizu!! *)

let evaluate_on_pairs p word matA matB =
  let rec eval_aux w result =
      match w with
      | "" -> result
      | _ ->
          let mat = match String.get w 0 with
                  | 'x' -> matA
                  | 'y' -> matB
                  | 'X' -> inverse_matrix p matA
                  | 'Y' -> inverse_matrix p matB
                  | _   -> failwith "Invalid letter"
          in
          eval_aux (String.sub w 1 (String.length w - 1)) (multiply result mat)
  in
  mod_matrix p (eval_aux word { a11 = 1; a12 = 0; a21 = 0; a22 = 1 })

let is_scalar_identity (mat: matrix) : bool =
  mat.a11 = mat.a22 && mat.a12 = 0 && mat.a21 = 0

  (* Test je pokazal, da sta ti funkciji skoraj enako hitri! *)

let ustvari_pare_matrik_1 lst =
  let rec aux acc = function
    | [] -> acc
    | hd :: tl -> 
      let new_pairs = List.map (fun x -> (hd, x)) tl in
      aux (new_pairs @ List.rev new_pairs @ acc) tl
  in
  aux [] lst
  
let ustvari_pare_matrik_2 lst =
  let rec aux acc = function
    | [] -> acc
    | hd :: tl -> 
      let new_pairs = List.flatten (List.map (fun x -> [(hd, x); (x, hd)]) tl) in
      aux (new_pairs @ acc) tl
  in
  aux [] lst

(* let evaluate_words_on_matrices p matA matB =
  let words = generate_words p in
  List.map (fun word -> (word, eval_expression p word matA matB)) words *)


let evaluate_word_on_matrix_tuples (word: string) (matrix_tuples: (matrix * matrix) list) p : (string * matrix) list =
  List.map (fun (matA, matB) -> (word, eval_expression p word matA matB)) matrix_tuples

(* Input: 
 1. seznam besed ustrezne dolžine (preconstructed)
 2. seznam vseh parov matrik (preconstructed)
 3. število p, ki določa modul   
*)

let evaluate_words_on_matrix_tuples (words: string list) (matrix_tuples: (matrix * matrix) list) p : (string * matrix) list list =
  List.map (fun word -> evaluate_word_on_matrix_tuples word matrix_tuples p) words
  

let evaluate_words_on_tuples words matrix_tuples p : (string * matrix) list =
  let evaluate_single_word word =
    List.map (fun (matA, matB) -> (word, eval_expression p word matA matB)) matrix_tuples
  in
  List.flatten (List.map evaluate_single_word words)

let generate_and_evaluate n p =
  evaluate_words_on_tuples (generate_words n) (ustvari_pare_matrik_1 (generate_psl_2_p p)) p

  (* Prvi argument je n-dolžina besed, drugi pa p-praštevilo*)
let results = generate_and_evaluate 3 3

let write_to_file filename tuples =
let oc = open_out filename in
let write_tuple (word, matrix) =
  Printf.fprintf oc "Word: %s, Matrix: (%d, %d, %d, %d)\n"
    word matrix.a11 matrix.a12 matrix.a21 matrix.a22
in
List.iter write_tuple tuples;
close_out oc
;;

write_to_file "output.txt" results;;