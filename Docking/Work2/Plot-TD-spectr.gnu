#set terminal postscript eps
set terminal postscript eps color
set key inside right top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 0.200
set title "TD spectr - whole protein molecule" 
set ylabel "𝚫G, kJ/mol" font "Helvetica-Bold,26"
set xlabel "Occupacion, %" font "Helvetica-Bold,26"
set bars small
#set xrange [0:50]
#set yrange [-100:0]
#set size 0.5,0.5
#set terminal postscript enhanced "Courier" 20

set terminal svg size 1200,900 font "Helvetica,26"
set key autotitle columnhead
set datafile separator ";"

set termoption dash

#set linestyle 1 lt 1 lw 2 lc rgb "red" dashtype 1
#set linestyle 2 lt 2 lw 2 lc rgb "blue" dashtype 1
#set linestyle 3 lt 3 lw 2 lc rgb "green" dashtype 1

set linestyle 1 lt 1 lw 2
set linestyle 2 lt 2 lw 2
set linestyle 3 lt 3 lw 2
set linestyle 4 lt 4 lw 2
set linestyle 5 lt 5 lw 2
set linestyle 6 lt 6 lw 2
set linestyle 7 lt 7 lw 2

set style fill transparent solid 1.0 noborder

#set output "TD Spectr-1ah1.svg"
#set yrange [:0]
#set key samplen -1 spacing 1.3 font ",26" box lw 0
#set nokey
#plot \
#     "1ah1~YDPEYRNFWGCG-all-td.csv" using (100.0*$5):4:(10.0*sqrt($5)) with points lc rgb "red"   pt 6 lw 0 ps variable ti "YDPEYRNFWGCG (+) 45%", \
#     "1ah1~EGLNRPSGGCG-all-td.csv"  using (100.0*$5):4:(10.0*sqrt($5)) with points lc rgb "green" pt 6 lw 0 ps variable ti "EGLNRPSGGCG (-) 55%"

set output "TD-Spectr-1ah1.svg"
set yrange [:-10]
#set key samplen -1 spacing 1.3 font ",26" box lw 0
#set nokey
plot \
     "ARHPSWYRPFE-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "ARHPSWYRPFE  -34.31; 16.99%", \
     "ARLRWLE-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "ARLRWLE  -30.96; 5.87%", \
     "EGLKRQFE-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "EGLKRQFE  -28.87; 3.58%", \
     "EGLNRPSG-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "EGLNRPSG  -30.54; 5.37%", \
     "EYARLRGDYQ-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "EYARLRGDYQ  -31.38; 8.77%", \
     "KNKRRLEEFE-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "KNKRRLEEFE  -28.45; 2.55%", \
     "NGLERVE-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "NGLERVE  -30.12; 4.06%", \
     "NLRVSLEH-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "NLRVSLEH  -29.71; 4.05%", \
     "NWRQFR-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "NWRQFR  -32.22; 8.29%", \
     "YDPEYRNFW-v1.csv" using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "YDPEYRNFW  -35.15; 20.11%", \
     "YRSRHPPGVYD-v1.csv"  using (50.0*$5):4:(50.0*$5):(0.00000) with xyerrorbars ps 0 lw 1 ti "YRSRHPPGVYD  -34.31; 20.29%"

quit
