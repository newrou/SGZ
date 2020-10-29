
for fligand in Ligands/*.pdbqt
do
    ligand=`basename $fligand`
    echo "*** Ligand $ligand ***"
    for ftarget in Targets/*.pdbqt
    do
	target=`basename $ftarget`
	echo "$target ~ $ligand"
	./dock-vina.sh $target $ligand
    done
    echo "-------"
    echo " "
done
