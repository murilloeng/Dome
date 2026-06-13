#data
nf = 100

#stats
stats "plot.txt" using (-1e3 * $3) : ($4 / 1e3) nooutput

#setup
set grid
set key above
set xlabel "u (mm)"
set ylabel "Load factor"
set terminal png size 900, 900
set xrange [STATS_min_x : STATS_max_x]
set yrange [STATS_min_y : STATS_max_y]

#animation
np = STATS_records
do for [i = 1 : nf] {
	index = np * i / nf
	set output sprintf("plot-%03d.png", i)
	plot "plot.txt" using (-1e3 * $3) : ($4 / 1e3) every ::1::index with lines linecolor rgb "#0000ff" notitle
}