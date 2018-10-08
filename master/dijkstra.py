# !!!!!!!!!!!!!!
# Copied from: https://bytes.com/topic/python/insights/877227-dijkstras-algorithm-finding-shortest-route
# Modified and adapted to suit my needs
# !!!!!!!!!!!!!!

#This is meant to solve a maze with Dijkstra's algorithm
import numpy as np
from numpy import inf
from copy import copy
 
class Dijkstra(object):
    """A graph object that has a set of singly connected,weighted,
    directed edges and a set of ordered pairs. Can be changed into
    a connection matrix. Vertices are [0,1,...,n], and edges are 
    [[1,2,3],[2,1,1],...] where the first means 1 connected to 2 
    with weight 3, and the second means 2 connected to 1 with 
    weight 1."""

    # -------------- START OF ADDINGS ----------------
    def __init__(self, edges):
        (V, E, lookup) = self.make_adjacency_matrix(edges)
        self.lookup = lookup
        self.other_init(V, E)


    def shortest_path(self, coords_vertice_start, coords_vertice_end):
        # Transform coordinates of vertices to indexes of these vertices in 
        # vertices set
        index_vertice_start = self.get_index(coords_vertice_start)
        index_vertice_end = self.get_index(coords_vertice_end)
        (total_distance, path_in_indexes) = self.dijkstra(index_vertice_end,
            index_vertice_start)
        path_in_coords = []
        for index_vertice in path_in_indexes:
            path_in_coords.append(self.get_coords(index_vertice))
        return path_in_coords


    def coords_to_key(self, coords):
        x = str(coords[0])
        y = str(coords[1])
        return '-'.join((x,y))


    def key_to_coords(self, key):
        (x,y) = key.split('-')
        return (int(x), int(y))


    def get_index(self, coords):
        return self.lookup[self.coords_to_key(coords)]


    def get_coords(self, index):
        for key, idx in self.lookup.items():
            if index == idx:
                return self.key_to_coords(key)
        return None


    def create_index(self, lookup, free_index, key):
        idx = free_index
        if key in lookup:
            idx = lookup[key]
        else:
            lookup[key] = idx
            free_index += 1

        return (idx, lookup, free_index)


    def make_adjacency_matrix(self, edges):
        free_index = 0
        E = []
        lookup = dict()  # Lookup table between real coordinates to index of node

        for edge in edges:
            # Get coordinates of start and end vertices of current edge
            (coords_vertice_start, coords_vertice_end) = edge

            # Get weight of edge (distance or L2 norm)
            weight = np.linalg.norm(np.array(coords_vertice_start) -
                np.array(coords_vertice_end))

            # Transform coordinates of vertices to indexes from 0 to N_VERTICES
            key_vertice_start = self.coords_to_key(coords_vertice_start)
            (index_vertice_start, lookup, free_index) = self.create_index(lookup,
                free_index, key_vertice_start)
            key_vertice_end = self.coords_to_key(coords_vertice_end)
            (index_vertice_end, lookup, free_index) = self.create_index(lookup,
                free_index, key_vertice_end)

            # Append to new format of edges
            E.append([index_vertice_start, index_vertice_end, weight])

        n_vertices = free_index
        V = range(n_vertices) 
        return (V, E, lookup)

    # -------------- END OF ADDINGS ----------------
 
    def other_init(self,vertices,edges):
        self.vertices=vertices
        self.size=len(self.vertices)
        self.edges=edges
        self.makematrix()
 
    def makematrix(self):
        "creates connection matrix"
        self.matrix=[]
        for i in range(self.size):
            self.matrix.append([])
            for j in range(self.size):
                self.matrix[i].append(inf)
        for edge in self.edges:
            self.matrix[edge[0]][edge[1]]=edge[2]
 
    def dijkstra(self,startvertex,endvertex):
        #set distances
        self.distance=[]
        self.route=[]
        for i in range(self.size):
            self.distance.append(inf)
            self.route.append([])
        self.distance[startvertex]=0
        self.route[startvertex]=[startvertex,]
        #set visited
        self.visited=[]
        self.current=startvertex
        while self.current != None:
            self.checkunvisited()
            if endvertex in self.visited: break
        return self.distance[endvertex],self.route[endvertex]
 
    def checkunvisited(self):
        basedist=self.distance[self.current]
        self.visited.append(self.current)
        for vertex,dist in enumerate(self.matrix[self.current]):
            if vertex in self.visited: continue #only check unvisited
            #set the distance to the new distance
            if basedist+dist<self.distance[vertex]:
                self.distance[vertex]=basedist+dist
                self.route[vertex]=copy(self.route[self.current])
                self.route[vertex].append(vertex)
        #set next current node as one with smallest distance from initial
        self.current=None
        mindist=inf
        for vertex,dist in enumerate(self.distance):
            if vertex in self.visited: continue
            if dist<mindist:
                mindist=dist
                self.current=vertex