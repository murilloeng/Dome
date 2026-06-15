#!/bin/bash

#setup
set -e

#gnuplot
gnuplot animation.gp

#ffmpeg
ffmpeg -y -loglevel error -framerate 20 -i plot-%03d.png -c:v libx264 -pix_fmt yuv420p animation.mp4

#remove
rm plot-*.png

#play
gio open animation.mp4