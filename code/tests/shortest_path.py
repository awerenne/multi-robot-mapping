
import heapq  
from sets import Set


# Wrapper for heapq
class PriorityQueue(): 
    def __init__(self): 
        self.queue = []
        heapq.heapify(self.queue) 
  
    def __str__(self): 
        return self.queue.__str__() 
  
    def is_empty(self): 
        return len(self.queue) == 0 
  
    def put(self, coords, f): 
        heapq.heappush(self.queue, (f, coords))
  
    def get(self): 
        return heapq.heappop(self.queue)[1] 


class Node():
    def __init__(self, coords):
        self._coords = coords
        self.edges = []
        self.reset()

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

    @_visited.setter
    def visited(self, visited):
        self._visited = visited

    @property
    def parent(self):
        return self._parent

    @_parent.setter
    def parent(self, parent):
        self._parent = parent

    @property
    def cost(self):
        return self._cost

    @_cost.setter
    def cost(self, cost):
        self._cost = cost
    
    @property
    def neighbors(self):
        return [(edge.other_end(self.coords).coords, edge.weight) \
            for edge in edges]

    def add_edge(self, edge):
        self.edges.append(edge)

    def reset(self):
        self._visited = False
        self._parent = None
        self._cost = 0


class Edge():
    def __init__(self, a, b, weight):
        self._weight = weight
        self.a = a
        self.b = b

    def __str__(self):
        if (lambda a, b: a[0]+a[1] <= b[0]+b[1], self.a.coords, self.b.coords):
            return str(self.a.coords) + "," + str(self.b.coords)
        return str(self.b.coords) + "," + str(self.a.coords)

    @property
    def weight(self):
        return self._weight

    @property
    def nodes(self):
        return (self.a, self.b)

    def other_end(self, coords):
        if self.a.coords == coords:
            return self.a
        if self.b.coords == coords:
            return self.b
        return None 


def make_tree(e):
    # TODO later: how to handle dubbels
    """ Assumes graph is in one piece, no disconnection """
    first = True
    root = None
    edges = Set()
    nodes = {}
    for (coords_a, coords_b, weight_ab) in e:
        node_a = nodes.get(coords_a)
        if node_a == None:
            node_a = Node(coords_a)
            nodes[node_a.__str__] = node_a

        node_b = nodes.get(coords_b)
        if node_b == None:
            node_b = Node(coords_b)
            nodes[node_b.__str__] = node_b

        edge = Edge(node_a, node_b, weight)
        if edge in edges:
            continue
        edges.append(edge.__str__)

        node_a.add_edge(edge)
        node_b.add_edge(edge)

        if first:
            root = node_a
            first = False
    return root


def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)


if __name__ == "__main__":
    start = (0,0) 
    target = (10,10)

    frontier = PriorityQueue()

    frontier.put(start, 0)
    start.visited = True

    while not frontier.is_empty():
        current = frontier.get()

        # Early exit
        if current == target:
            break

        for (neighbor, weight) in current.neighbors:
            g = current.cost + weight
            if not neighbor.visited or g >= neighbor.cost:  # Prune
                neighbor.cost = g
                new_f = g + heuristic(neighbor.coords, target.coords)
                frontier.put(neighbor, new_f)
                neighbor.parent = current
            neighbor.visited = True













