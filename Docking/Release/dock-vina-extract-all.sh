
for i in *.dlg
do
    name=`basename $i .dlg`
    echo $name
    ./dock-vina-extract.py $name < $i > $name.csv
done

cat 1ah1*~EGLNRPSG*.csv > 1ah1~EGLNRPSG-all.csv
cat 1ah1*~YDPEYRNFW*.csv > 1ah1~YDPEYRNFW-all.csv
cat 1ah1~EGLNRPSG-all.csv 1ah1~YDPEYRNFW-all.csv > 1ah1-all.csv
grep "EGLNRPSG" < 1ah1-all.csv > 1ah1~EGLNRPSG-all-td.csv
grep "YDPEYRNFW" < 1ah1-all.csv > 1ah1~YDPEYRNFW-all-td.csv

