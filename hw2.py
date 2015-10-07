import numpy as np
import re

class Graph(object):
	num_nodes = 0
	edges = []
	matrix = np.matrix('0')

	def __init__(self, num_nodes):
		self.num_nodes = num_nodes
		self.edges = [ [ 0 ] * self.num_nodes for i in range(self.num_nodes) ]
		self.matrix = np.matrix(self.edges)

	def init_edges(self, input_edges):
		for edge in input_edges:
			self.edges[edge[0]][edge[1]] = 1
			#print "edge: ", edge[0], edge[1]
			#print self.edges

	def __repr__(self):
		return str(self.edges)

with open("input.txt", "r") as input_file:
	num = 0

	#if input_file.readline() == "[num_nodes]":
	print "a", input_file.readline()
	num = int(input_file.readline())
	
	print "b", input_file.readline(), "c", input_file.readline()

	edges = []

	for line in input_file:
		#print edges, type(edges)
		edgesObj = re.match(r'(\d+)\D*(\d+)', line)
		
		edge = ( int(edgesObj.group(1)), int(edgesObj.group(2)) )
		edges.append(edge)

		#print "a ", edgesObj.group(1), "b ", edgesObj.group(2)

	my_graph = Graph(num)
	my_graph.init_edges(edges)
	
print my_graph