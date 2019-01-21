"""
    Description.
"""


import heapq  

from utils import center2xy


# TODO: Convert weight to float
# TODO: Assert info (values, sizes, ...)
# TODO: Initialize node with type/orientation info


#---------------
class Graph():
    """
    Blabla.
    """

    def __init__(self):
        self.edges = {}
        self.nodes = {}
    

    #---------------
    @property
    def edges(self):
        return [edge.nodes for key, edge in self.edges.items()]


    #---------------
    def new_node(self, coords):
        node = self.nodes.get(coords)
        if node == None:
            node = Node(coords)
            self.nodes[coords] = node

    #---------------
    def new_edge(self, e):
        
        (coords_a, coords_b, weight_ab) = e

        node_a = self.new_node(coords_a)
        node_b = self.new_node(coords_b)

        edge = Edge(node_a, node_b, weight_ab)
        test = self.edges.get(edge.__str__())
        if test == None:
            node_a.connect(edge)
            node_b.connect(edge)
            self.edges[edge.__str__()] = edge

    #---------------
    def get_node(self, coords):
        return self.nodes.get(coords)


    #---------------      
    def reset(self):
        map(lambda node: node.reset(), self.nodes.itervalues())



#---------------
class Node():
    """
    Blabla.
    """

    def __init__(self, position, orientation, type_intersection):
        self._position = position
        self._visited = False
        self._parent = None
        self._cost = float('Inf')
        
        self.edges = {}
        # TODO
    

    #---------------
    @property
    def position(self):
        return self._position
 

    #---------------   
    @property
    def visited(self):
        return self._visited


    #---------------
    @visited.setter
    def visited(self, visited):
        self._visited = visited


    #---------------
    @property
    def parent(self):
        return self._parent


    #---------------
    @parent.setter
    def parent(self, parent):
        self._parent = parent

   
    #---------------
    @property
    def cost(self):
        return self._cost


    #---------------
    @cost.setter
    def cost(self, cost):
        self._cost = cost


    #---------------    
    @property
    def neighbors(self):
        neighbors = []
        for _, edge in edges.items():
            if edge != None:  
                neighbors.append(edge.connected_to(self.position), edge.weight)
        return neighbors


    #---------------    
    @property
    def unexplored_orientations(self):
        unexplored = []
        for orientation, edge in edges.items():
            if edge == None:  
                unexplored.append(orientation)
        return unexplored


    #---------------
    def connect(self, edge, orientation):
        self.edges[orientation] = edge


    #---------------
    def reset(self):
        self._visited = False
        self._parent = None
        self._cost = float('Inf')



#---------------
class Edge():
    """
    Blabla. Ordered to avoid duplicates
    """

    def __init__(self, a, b, weight):
        self._weight = weight
        self.a = a
        self.b = b


    #---------------
    def __str__(self): 
        position_a = center2xy(self.a.position, dimension)
        position_b = center2xy(self.b.position, dimension)

        def a_then_b(pos_a, pos_b):
            return (pos_a[0] + pos_a[1]) < (pos_b[0] + pos_b[1])

        if a_then_b(position_a, position_b):
            return str(position_a) + ";" + str(position_b) + \
                ";" + str(self._weight)
        else:
            return str(position_b) + ";" + str(position_a) + \
                ";" + str(self._weight)


    #---------------
    @property
    def weight(self):
        return self._weight


    #---------------
    @property
    def nodes(self):
        return (self.a, self.b)


    #---------------
    def connected_to(self, position):
        if self.a.position == position:
            return self.b
        if self.b.position == position:
            return self.a
        return None 































