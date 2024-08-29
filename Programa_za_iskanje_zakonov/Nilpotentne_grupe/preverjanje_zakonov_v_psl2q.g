# preverjanje_zakonov_v_psl2q

Print("preverjanje_zakonov_v_psl2q \n");

p := 5;
k := 2; 
q := p^k;

G := PSL(2, q);

# Get the orders of all elements in the group
orders := List(Elements(G), g -> Order(g));

# Extract unique orders
unique_orders := Set(orders);

# Print the unique orders
Print("Different orders of elements in the group: ", unique_orders, "\n");

F2 := FreeGroup(2);   # Free group on 2 generators
F2 := FreeGroup("a", "b");
a := F2.1;            # First generator of F
b := F2.2;            # Second generator of F

CustomCommutator := function(a, b)
    return a * b * a^-1 * b^-1;
end;

# w := a^p * b^(q -1) * a^-p * b^(q + 1) * a^p * b^(1 - q) * a^-p * b^(-1 - q);
# w := CustomCommutator(CustomCommutator(a^p, b^(q - 1)), CustomCommutator(b^(q + 1), a^p)); # tole je zakon le v primeru p = 2, lahko opišeš
# w := CustomCommutator(Comm(a^p, b^-(q - 1)), CustomCommutator(a^-(q + 1), b^p));
w := CustomCommutator(CustomCommutator(CustomCommutator(b, a^p), CustomCommutator(b, a^(q - 1))), CustomCommutator(b, a^(q + 1))); # direktna uporaba komutatorske leme tega ne dovoli, to je očitno, če preveriš izginjajoče množice
w := CustomCommutator(CustomCommutator(b * a^p * b^-1, a^(q -1)), a^(q + 1));
Print(Length(w), "je dolžina besede w = ", w ,"\n");

# # Create the homomorphism and evaluate the word
# hom := GroupHomomorphismByImages(F2, G, [a, b], [g1, g2]);
# result := Image(hom, w);

# # Print the result
# Print("\n"result, "\n");

pairsinG := Tuples(G, 2);

# List(pairsinG, Image(FreeGroupHomomorphismByImages(F2, G, [a, b], [g1, g2]), w);)
seznam := List(pairsinG, tup -> Image(GroupHomomorphismByImages(F2, G, [a, b], [tup[1], tup[2]]), w));

# Evaluate the word for each pair in pairsinG
# results := List(pairsinG, tup -> Image(GroupHomomorphismByImages(F2, G, [a, b], [tup[1], tup[2]]), w));

# Check if all elements are the identity element
all_identity := ForAll(seznam, x -> x = ());

# Output the result
if all_identity then
    Print("All elements are the identity element ()\n");
else
    Print("Not all elements are the identity element ()\n");
fi;