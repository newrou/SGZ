
for fname in *-v[0-9][0-9].pdb
do
    echo $fname
    ./prepare_receptor4.py -r $fname
#    rename "s/_model[0-9]+//" $fname\_*.pdbqt
done

rename "s/_model[0-9]+//" *.pdbqt

