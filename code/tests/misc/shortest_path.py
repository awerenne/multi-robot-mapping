"""
    Description.
"""


import heapq  


# ------------
# Wrapper for heapq
class PriorityQueue(): 
    def __init__(self): 
        self.queue = []
        heapq.heapify(self.queue) 
  
    def __str__(self): 
        return self.queue.__str__() 
  
    def is_empty(self): 
        return len(self.queue) == 0 
  
    def put(self, node, f): 
        heapq.heappush(self.queue, (f, node))
  
    def get(self): 
        return heapq.heappop(self.queue)[1] 


# ------------
class Node():
    def __init__(self, coords):
        self._coords = coords
        self._visited = False
        self._parent = None
        self._cost = float('Inf')
        self.edges = []

    def __eq__(self, x):
        return self._coords == x

    def __str__(self):
        return str(self.coords)

    @property
    def coords(self):
        return self._coords
    
    @property
    def visited(self):
        return self._visited

    @visited.setter
    def visited(self, visited):
        self._visited = visited

    @property
    def parent(self):
        return self._parent

    @parent.setter
    def parent(self, parent):
        self._parent = parent

    @property
    def cost(self):
        return self._cost

    @cost.setter
    def cost(self, cost):
        self._cost = cost
    
    @property
    def neighbors(self):
        return [(edge.connected_to(self.coords), edge.weight) \
            for edge in self.edges]

    def connect(self, edge):
        self.edges.append(edge)

    def reset(self):
        self._visited = False
        self._parent = None
        self._cost = 100000


# ------------
class Edge():
    def __init__(self, a, b, weight):
        self._weight = weight
        self.a = a
        self.b = b

    def __str__(self):  # positive basis is important!
        def order_to_bottom_right(coords_a, coords_b):
            return coords_a[0]+coords_a[1] < coords_b[0] + coords_b[1]
        if order_to_bottom_right(self.a.coords, self.b.coords):
            return str(self.a) + ";" + str(self.b) + ";" + str(self._weight)
        else:
            return str(self.b) + ";" + str(self.a) + ";" + str(self._weight)

    def __eq__(self, x):
        pair_node_condition = (self.a == x.a and self.b == x.b) or \
                                (self.b == x.a and self.a == x.b) 
        return (x.weight == self.weight and pair_node_condition)

    @property
    def weight(self):
        return self._weight

    @property
    def nodes(self):
        return (self.a, self.b)

    def connected_to(self, coords):
        if self.a.coords == coords:
            return self.b
        if self.b.coords == coords:
            return self.a
        return None 


# ------------
# TODO: online metod
# TODO later: how to handle dubbels
# Assumes graph is in one piece, no disconnection 
class Graph():

    def __init__(self):
        self.edges = {}
        self.nodes = {}
        
    def add_node(self, coords):
        node = self.nodes.get(coords)
        if node == None:
            node = Node(coords)
            self.nodes[coords] = node
        return node

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

    def get_node(self, coords):
        return self.nodes.get(coords)


# ------------
def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)


# ------------
if __name__ == "__main__":
    start_coords = (0,0) 
    target_coords = (10,10)

    # Edges
    e1 = (start_coords, target_coords, 50)
    e2 = (start_coords, (5, 3), 10)
    e3 = ((5, 3), target_coords, 20)

    graph = Graph()
    graph.add_edge(e1)
    graph.add_edge(e2)
    graph.add_edge(e3)

    frontier = PriorityQueue()

    start = graph.get_node(start_coords)
    target = graph.get_node(target_coords)
    start.visited = True
    start.cost = 0
    frontier.put(start, 0)

    # TODO: Assert if no path 

    while not frontier.is_empty():
        current = frontier.get()

        # Early exit
        if current == target:
            break

        for (neighbor, weight) in current.neighbors:
            g = current.cost + weight
            if not neighbor.visited or g <= neighbor.cost:  # Prune
                neighbor.cost = g
                neighbor.parent = current

                new_f = g + heuristic(neighbor.coords, target.coords)
                frontier.put(neighbor, new_f)
                
            neighbor.visited = True

    def unrol(node):  # Write as lambda function
        path = []
        while (node != None):
            path.append(node.coords)
            node = node.parent
        return path

    def print_path(path):
        s = ""
        while len(path) > 0:
            s += str(path.pop())
            if len(path) >=  1:
                s += " --> "
        print(s)


    path = print_path(unrol(target))











