"""
    Description.
"""


import heapq  


# TODO: online metod
# TODO later: how to handle dubbels


# ------------
class Graph():
    """
    Blabla.
    """

    def __init__(self):
        self.edges = {}
        self.nodes = {}
        
    # ------------
    def add_node(self, coords):
        node = self.nodes.get(coords)
        if node == None:
            node = Node(coords)
            self.nodes[coords] = node
        return node

    # ------------
    def add_edge(self, e):
        # TODO: Assert size tuple edge (because info can come from robot/user)
        (coords_a, coords_b, weight_ab) = e

        node_a = self.add_node(coords_a)
        node_b = self.add_node(coords_b)

        edge = Edge(node_a, node_b, weight_ab)
        test = self.edges.get(edge.__str__())
        if test == None:
            node_a.connect(edge)
            node_b.connect(edge)
            self.edges[edge.__str__()] = edge

    # ------------
    def get_node(self, coords):
        return self.nodes.get(coords)


# ------------
class Node():
    """
    Blabla.
    """

    def __init__(self, coords):
        self._coords = coords
        self._visited = False
        self._parent = None
        self._cost = float('Inf')
        self.edges = []

    # ------------
    def __eq__(self, x):
        return self._coords == x

    # ------------
    def __str__(self):
        return str(self.coords)

    # ------------
    @property
    def coords(self):
        return self._coords
 
    # ------------   
    @property
    def visited(self):
        return self._visited

    # ------------
    @visited.setter
    def visited(self, visited):
        self._visited = visited

    # ------------
    @property
    def parent(self):
        return self._parent

    # ------------
    @parent.setter
    def parent(self, parent):
        self._parent = parent

    # ------------
    @property
    def cost(self):
        return self._cost

    # ------------
    @cost.setter
    def cost(self, cost):
        self._cost = cost

    # ------------    
    @property
    def neighbors(self):
        return [(edge.connected_to(self.coords), edge.weight) \
            for edge in self.edges]

    # ------------
    def connect(self, edge):
        self.edges.append(edge)

    # ------------
    def reset(self):
        self._visited = False
        self._parent = None
        self._cost = 100000


# ------------
class Edge():
    """
    Blabla.
    """

    def __init__(self, a, b, weight):
        self._weight = weight
        self.a = a
        self.b = b

    # ------------
    def __str__(self):  # positive basis is important!
        def order_to_bottom_right(coords_a, coords_b):
            return coords_a[0]+coords_a[1] < coords_b[0] + coords_b[1]
        if order_to_bottom_right(self.a.coords, self.b.coords):
            return str(self.a) + ";" + str(self.b) + ";" + str(self._weight)
        else:
            return str(self.b) + ";" + str(self.a) + ";" + str(self._weight)

    # ------------
    def __eq__(self, x):
        pair_node_condition = (self.a == x.a and self.b == x.b) or \
                                (self.b == x.a and self.a == x.b) 
        return (x.weight == self.weight and pair_node_condition)

    # ------------
    @property
    def weight(self):
        return self._weight

    # ------------
    @property
    def nodes(self):
        return (self.a, self.b)

    # ------------
    def connected_to(self, coords):
        if self.a.coords == coords:
            return self.b
        if self.b.coords == coords:
            return self.a
        return None 































