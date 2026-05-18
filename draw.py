#imports
import sys
import numpy
import matplotlib.pyplot

#path
path = "dome.txt" if len(sys.argv) == 1 else sys.argv[1]

#read
with open(path, "r") as file:
	#nodes
	nn = int(file.readline().strip())
	nodes = numpy.zeros((nn, 3))
	for i in range(nn):
		nodes[i] = list(map(float, file.readline().strip().split()))
	#elements
	ne = int(file.readline().strip())
	elements = [[0, 0] for _ in range(ne)]
	for i in range(ne):
		elements[i] = list(map(int, file.readline().strip().split()))

#draw
axes = matplotlib.pyplot.figure().add_subplot(projection = "3d")
axes.scatter(nodes[:, 0], nodes[:, 1], nodes[:, 2], color = "red")
for element in elements:
	axes.plot(nodes[element, 0], nodes[element, 1], nodes[element, 2], color = "blue")

#setup
axes.set_aspect("equal")

#show
matplotlib.pyplot.show()