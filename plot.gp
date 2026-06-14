#stats
stats "plot.txt" using (-$3) : ($4) nooutput

#setup
set grid
set key above
set xlabel "u (m)"
set ylabel "{/Symbol l}" norotate
set xrange [STATS_min_x : STATS_max_x]
set yrange [STATS_min_y : STATS_max_y]

#plot
plot 'plot.txt' using (-$3) : ($4) with lines linecolor rgb "#0000ff" notitle