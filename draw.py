#imports
import numpy
import matplotlib.pyplot
import matplotlib.animation

#model
with open("Model.txt", "r") as file:
	#data
	nn = int(file.readline().strip())
	ne = int(file.readline().strip())
	ns = int(file.readline().strip())
	#nodes
	nodes = numpy.zeros((nn, 3))
	for i in range(nn):
		nodes[i] = list(map(float, file.readline().strip().split()))
	#elements
	elements = [[0, 0] for _ in range(ne)]
	for i in range(ne):
		elements[i] = list(map(int, file.readline().strip().split()))

#deformed
deformed = numpy.zeros((ns, nn, 3))
for node in range(nn):
	with open("Deformed/N%d.txt" % node) as file:
		data = [float(entry) for entry in file.read().split()]
		for step in range(ns):
			deformed[step, node, 0] = data[3 * step + 0]
			deformed[step, node, 1] = data[3 * step + 1]
			deformed[step, node, 2] = data[3 * step + 2]

#draw
figure = matplotlib.pyplot.figure()
axes = figure.add_subplot(projection = "3d")
scatter = axes.scatter(deformed[0, :, 0], deformed[0, :, 1], deformed[0, :, 2], color = "red")
lines = [axes.plot(deformed[0, element, 0], deformed[0, element, 1], deformed[0, element, 2], color = "blue")[0] for element in elements]

def update(frame):
	#scatter
	scatter._offsets3d = (deformed[frame, :, 0], deformed[frame, :, 1], deformed[frame, :, 2])
	#lines
	for i, element in enumerate(elements):
		lines[i].set_data(deformed[frame, element, 0], deformed[frame, element, 1])
		lines[i].set_3d_properties(deformed[frame, element, 2])
	#return
	return scatter, *lines

#animation
animation = matplotlib.animation.FuncAnimation(figure, update, frames = ns, interval = 1, blit = False)

#setup
axes.set_aspect("equal")

#show
matplotlib.pyplot.show()