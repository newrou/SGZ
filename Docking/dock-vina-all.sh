
for target in *-v[0-9][0-9].pdbqt
do
    echo $target" ~ "EGLNRPSG_1.pdbqt
    ./dock-vina.sh $target EGLNRPSG_1.pdbqt
    echo $target" ~ "YDPEYRNFW-1.pdbqt
    ./dock-vina.sh $target YDPEYRNFW-1.pdbqt
done

