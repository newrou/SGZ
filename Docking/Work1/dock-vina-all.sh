
for target in *-v[0-9][0-9].pdbqt
do
    echo $target" ~ "EGLNRPSGGCG-v1.pdbqt
    ./dock-vina.sh $target EGLNRPSGGCG-v1.pdbqt
    echo $target" ~ "YDPEYRNFWGCG-v1.pdbqt
    ./dock-vina.sh $target YDPEYRNFWGCG-v1.pdbqt

#    echo $target" ~ "EGLNRPSG-v1.pdbqt
#    ./dock-vina.sh $target EGLNRPSG-v1.pdbqt
#    echo $target" ~ "YDPEYRNFW-v1.pdbqt
#    ./dock-vina.sh $target YDPEYRNFW-v1.pdbqt
done

