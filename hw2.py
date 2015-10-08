import numpy as np
import re

class Graph(object):
	num_nodes = 0
	adj_matrix = np.matrix('0')

	def __init__(self, num_nodes):
		self.num_nodes = num_nodes
		self.adj_matrix = np.matrix( [ [ 0 ] * self.num_nodes for i in range(self.num_nodes) ])
		
	def init_edges(self, input_edges):
		for edge in input_edges:
			self.adj_matrix[ edge[0], edge[1] ] = 1
			self.adj_matrix[ edge[1], edge[0] ] = 1
			#print "edge: ", edge[0], edge[1]

	def find_num_of_path(self, length, vertices ):
		tmp_matrix = self.adj_matrix
		while length > 1:
			tmp_matrix *= tmp_matrix
			length -= 1
			print "tmp_mat: ", tmp_matrix

		return tmp_matrix[ vertices[0], vertices[1] ]

	def __repr__(self):
		return str(self.adj_matrix)

def init_graph(filename):
	with open(filename, "r") as input_file:
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

		tmp_graph = Graph(num)
		tmp_graph.init_edges(edges)
		return tmp_graph
	
my_graph = init_graph("input2.txt")
print my_graph

print my_graph.find_num_of_path(3, (1,2) )