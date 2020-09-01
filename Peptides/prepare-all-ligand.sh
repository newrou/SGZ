
for fname in *-v[0-9].pdb
do
    echo $fname
    ./prepare_ligand4.py -l $fname
done

#rename "s/_model[0-9]+//" *.pdbqt

