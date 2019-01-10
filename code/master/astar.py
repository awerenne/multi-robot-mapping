# !!!!!!!!!!!!!!
# Copied from: https://bytes.com/topic/python/insights/877227-dijkstras-algorithm-finding-shortest-route
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

    def __init__(self,vertices,edges):
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