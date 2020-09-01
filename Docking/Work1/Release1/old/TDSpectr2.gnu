#set terminal postscript eps
set terminal postscript eps color
set key inside right top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 0.200
#set title "TD spectr" 
set xlabel "Occupation, %" 
set ylabel "dG, kJ/mol" 
set bars small
#set xrange [0:100]
#set yrange [-100:0]

set output "Spectr3.eps"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 3 , \
 '' using (100*$3):2:2 with labels tc lt 3 offset 3,0.0 font "Helvetica,10" notitle

set output "Spectr4.eps"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 2


set terminal emf color

set output "Spectr3.emf"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 3 , \
 '' using (100*$3):2:2 with labels tc lt 3 offset 3,0.0 font "Helvetica,10" notitle

set output "Spectr4.emf"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 2

set terminal gif giant size 1200,900 transparent linewidth 2

set output "Spectr3.gif"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 2 , \
 '' using (2+100*$3):2:2 with labels tc lt 3 offset 3,0.0 notitle

set output "Spectr4.gif"
plot 'spectr.dat' u (0.1+50*$3):2:(0.2+50*$3):(0.00001) with xyerrorbars notitle ps 0 lw 2


quit
