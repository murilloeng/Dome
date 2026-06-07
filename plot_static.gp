set grid
set key above
set xlabel "u (m)"
set ylabel "F (kN)"

plot 'data.txt' using (-1e3 * $1) : ($2 / 1e3) with lines linecolor rgb "#0000ff" notitle