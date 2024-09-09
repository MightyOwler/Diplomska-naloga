LoadPackage("nq");
LoadPackage("anupq");

F2 := FreeGroup(2);
F2 := FreeGroup("a", "b", "x");
AssignGeneratorVariables(F2); # tole predpiše vrednosti generatorjem

filePath := "Rezultati/";

# spodnja_meja mora biti več kot 1, sicer ne dela
# v resnici nas trivialna grupa itak ne zanima
spodnja_meja := 97;
zgornja_meja := 100;  
    
all_gps := Concatenation(List([spodnja_meja..zgornja_meja], n -> AllSmallGroups(Size, n)));
velikost_vseh := Length(all_gps);

# Seznam grup moči želenih redov
all_nilpotent_gps := Filtered(all_gps, G -> IsNilpotent(G) and Size(G) >= spodnja_meja and Size(G) <= zgornja_meja);

Print("Število vseh grup moči v razponu: ", spodnja_meja, "-", zgornja_meja, ": ", velikost_vseh , " od tega nilpotentnih: ", Length(all_nilpotent_gps), "\n\n");

beseda := function(a, b)
    return a * b^-3 * a^3 * b^17 * a^-3;
end;

results := [];

for G in all_nilpotent_gps do
    start_time := Runtime();

    # Vsi pari elementov iz G
    pairsinG := Tuples(G, 2);

    exp := Exponent(G);
    razred_nilpotentnosti := NilpotencyClassOfGroup(G);
    nq := NilpotentQuotient(F2/[x^exp] : class := razred_nilpotentnosti, idgens := [x]);

    # Naredimo seznam homomorfizmov, ki jih dobimo med končnima grupama
    homphis := List(pairsinG, tup -> GroupHomomorphismByImages(nq, G, [nq.1, nq.2], [tup[1], tup[2]]));

    # Naredimo seznam jeder
    kers := List(homphis, phi -> Kernel(phi));

    # Step 4: Find the intersection of all kernels (the "laws" of the group)
    zakoni := Intersection(kers);
    quotient_group := FactorGroup(nq, zakoni);

    zakoni := PcGroupToPcpGroup(zakoni);
    # nq := PcGroupToPcpGroup(nq);

    # Printamo rezultate
    Print("Struktura nilpotentne grupe G: << ", StructureDescription(G), " >>\n");
    Print("Grupa G je moči ", Size(G), " in razreda nilpotentnosti ", razred_nilpotentnosti, "\n");
    Print("Grupa zakonov kvocienta nq = F_2 / (F_2^exp gama_", razred_nilpotentnosti + 1, "(F_2)): ", zakoni, "\n");
    PrintPcpPresentation(zakoni);
    Print("Generatorji kvocienta nq: ", GeneratorsOfGroup(zakoni), "\n");
    # Print("PCP prezentacija kvocienta: ", PcpPresentation(zakoni), "\n\n");

    
    Print("Struktura kvocienta nq/zakoni: <<< ", StructureDescription(quotient_group), " >>>\n");
    Print("Indeks podgrupe zakoni v nq: ", Size(quotient_group), "\n");
    Print("Generatorji kvocienta nq/zakoni: ", GeneratorsOfGroup(quotient_group), "\n");

    end_time := Runtime();
    elapsed_time := end_time - start_time;
    Print("Izračunano v: ", elapsed_time, " ms\n");
    Print("\n");

    Append(results, [rec(
        group := G,
        laws := zakoni,
        quotient := quotient_group
    )]);
    Print("\n");
od;




# Sestavljanje imena
filename := Concatenation("C:/Users/jasak/Documents/Jasa/Sola/FMF/3_letnik/Diploma/Programa_za_iskanje_zakonov/Nilpotentne_grupe/Rezultati/zakoni_za_nilpotentne_grupe_velikosti (", 
                          String(spodnja_meja), "-", String(zgornja_meja), ").txt");

# Ustvarimo datoteko za shranjevanje rezultatov
outputFile := OutputTextFile(filename, false);

# Za vsak vnos v seznam rezultatov izpišemo en blok teksta
for result in results do
    AppendTo(outputFile, Concatenation("Struktura nilpotentne grupe G: << ", String(StructureDescription(result.group)), " >> \n"));
    AppendTo(outputFile, Concatenation("Grupa G je moči ",  String(Size(result.group)), " in razreda nilpotentnosti ", String(NilpotencyClassOfGroup(result.group)), "\n"));
    AppendTo(outputFile, Concatenation("nq = F_2 / (F_2^exp gama_", String(1 + NilpotencyClassOfGroup(result.group)), "(F_2))\n"));
    AppendTo(outputFile, Concatenation("Grupa zakonov v kvocientu nq: ", String(result.laws), "\n"));
    AppendTo(outputFile, Concatenation("Struktura kvocienta nq/zakoni: <<< ", String(StructureDescription(result.quotient)), " >>>\n"));
    AppendTo(outputFile, Concatenation("Indeks podgrupe zakoni v nq: ", String(Size(result.quotient)), "\n"));
    AppendTo(outputFile, Concatenation("Generatorji kvocienta nq/zakoni: ", String(GeneratorsOfGroup(result.quotient)), "\n\n"));
od;

# Close the file when done
CloseStream(outputFile);