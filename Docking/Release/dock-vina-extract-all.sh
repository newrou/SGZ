
for i in *.dlg
do
    name=`basename $i .dlg`
    echo $name
    ./dock-vina-extract.py $name < $i > $name.csv
done

cat 1ah1*~EGLNRPSG*[0-9].csv > 1ah1~EGLNRPSG-all.csv
cat 1ah1*~YDPEYRNFW*[0-9].csv > 1ah1~YDPEYRNFW-all.csv
cat 1ah1~EGLNRPSG-all.csv 1ah1~YDPEYRNFW-all.csv > 1ah1-all.csv
./make-TD-spectr.py < 1ah1-all.csv > 1ah1-all-td.csv
grep "EGLNRPSG" < 1ah1-all-td.csv > 1ah1~EGLNRPSG-all-td.csv
grep "YDPEYRNFW" < 1ah1-all-td.csv > 1ah1~YDPEYRNFW-all-td.csv
