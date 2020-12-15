
echo "Ligand; min dG; Occupation" > All-stat.csv

for fligand in Ligands/*.pdbqt
do
    ligand=`basename $fligand .pdbqt`
    echo "*** Ligand $ligand ***"
    ./make-TD-spectr.py < All.csv > All-td.csv
    grep "$ligand\;" < All-td.csv > $ligand.csv
    ./sum-occupation.py $ligand.csv | tee $ligand.stat
    tail -n 1 $ligand.stat >> All-stat.csv
done

#gnuplot Plot-TD-spectr.gnu
