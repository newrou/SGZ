
for i in *.dlg
do
    name=`basename $i .dlg`
    echo $name
    ./dock-vina-extract.py $name < $i > $name.csv
done

cat 1ah1*~EGLNRPSGGCG-v1.csv > 1ah1~EGLNRPSGGCG-all.csv
cat 1ah1*~YDPEYRNFWGCG-v1.csv > 1ah1~YDPEYRNFWGCG-all.csv
cat 1ah1~EGLNRPSGGCG-all.csv 1ah1~YDPEYRNFWGCG-all.csv > 1ah1-all.csv
./make-TD-spectr.py < 1ah1-all.csv > 1ah1-all-td.csv

grep "YDPEYRNFWGCG" < 1ah1-all-td.csv > 1ah1~YDPEYRNFWGCG-all-td.csv
grep "EGLNRPSGGCG" < 1ah1-all-td.csv > 1ah1~EGLNRPSGGCG-all-td.csv

./sum-occupation.py 1ah1~YDPEYRNFWGCG-all-td.csv | tee 1ah1~YDPEYRNFWGCG-all-td.stat
./sum-occupation.py 1ah1~EGLNRPSGGCG-all-td.csv | tee 1ah1~EGLNRPSGGCG-all-td.stat

gnuplot Plot-TD-spectr.gnu
