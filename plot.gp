#stats
stats "data.txt" using (-1e3 * $1) : ($2) nooutput

#setup
set grid
set key above
set xlabel "u (mm)"
set ylabel "{/Symbol l}" norotate
set xrange [STATS_min_x : STATS_max_x]
set yrange [STATS_min_y : STATS_max_y]

#plot
plot 'data.txt' using (-1e3 * $1) : ($2) with lines linecolor rgb "#0000ff" notitle

#animation
nf = 100
np = STATS_records
set output "plot.gif"
set ylabel "Load factor" rotate
set terminal gif animate delay 0 size 800, 800
do for [t = 1 : np : np / nf] {
	plot "data.txt" using (-1e3 * $1) : ($2) every ::1::t with lines linecolor rgb "#0000ff" notitle
}