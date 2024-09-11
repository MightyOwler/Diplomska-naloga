f := FreeGroup("x", "y");
AssignGeneratorVariables(f);

g := PSL(2,3);
g3 := SymmetricGroup(4);
# g := DihedralGroup(36);

Size(g);

presek := f;
for a in g do
    for b in g do
        phi := GroupHomomorphismByImages(f, g, [x,y], [a,b]);
        k := Kernel(phi);
        presek := Intersection(presek, k);
    od;
od;

kvocient := FactorGroup(f, presek);
struktura := StructureDescription(kvocient);