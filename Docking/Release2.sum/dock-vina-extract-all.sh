
cat 1ah1-all-[0-9].csv > 1ah1-all.csv
./make-TD-spectr.py < 1ah1-all.csv > 1ah1-all-td.csv

grep "YDPEYRNFWGCG" < 1ah1-all-td.csv > 1ah1~YDPEYRNFWGCG-all-td.csv
grep "EGLNRPSGGCG" < 1ah1-all-td.csv > 1ah1~EGLNRPSGGCG-all-td.csv

./sum-occupation.py 1ah1~YDPEYRNFWGCG-all-td.csv | tee 1ah1~YDPEYRNFWGCG-all-td.stat
./sum-occupation.py 1ah1~EGLNRPSGGCG-all-td.csv | tee 1ah1~EGLNRPSGGCG-all-td.stat

gnuplot Plot-TD-spectr.gnu
