
import numpy as np
import heapq
from yaml import load
try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader


#---------------
class Parameters():

    def __init__(self, path="../config.yaml"):
        self.path = path
        self.load().build()

    #-------
    def __str__(self):
        self.container.__str__()

    #-------
    def __getattr__(self, name):
        if name not in self.container:
            return None
        if type(self.container[name]) is dict:
            return Container(self.container[name])
        return self.container[name]

    #-------
    def load(self):
        self.container = Container(load(open(self.path, 'r'),
                Loader=Loader))
        return self

    #-------
    def build(self):
        self.is_simulation = self.container.is_simulation
        assert self.is_simulation != None

        temp = {}
        if self.is_simulation: 
            temp = self.container.pop("simulation")
            del self.container["real"]
        else: 
            temp = self.container.pop("real")
            del self.container["simulation"]
        assert temp != None
        self.container.update(temp)
        self.container = Container(self.container)
        return self


#---------------
class PriorityQueue():
    """
    Wrapper for heapq. Used for A* algorithm.
    """

    def __init__(self): 
        self.queue = []
        heapq.heapify(self.queue) 
  
    #-------
    def __str__(self): 
        return self.queue.__str__() 
  
    #-------
    def size(self):
        return len(self.queue)
    
    #-------
    def is_empty(self): 
        return len(self.queue) == 0 

    #-------
    def put(self, node, f): 
        heapq.heappush(self.queue, (f, node))

    #-------
    def get(self): 
        return heapq.heappop(self.queue)[1] 


#---------------
class Container(dict):
    """
    Dictionary whose keys can be accessed as attributes.
    """

    def __init__(self, *args):
        super(Container, self).__init__(*args)

    #-------
    def __getattr__(self, key):
        if key not in self:
            return None
        if type(self[key]) is dict:
            return Container(self[key])
        return self[key]


#---------------
def manhattan_distance(a, b):
    return float(abs(a[0]-b[0]) + abs(a[1]-b[1]))


#---------------
def heuristic(robot, frontier, other_robot, lambda_):
    return manhattan_distance(robot, frontier) + lambda_*manhattan_distance(frontier, other_robot)


#---------------
def xy2ij(xy, dimension):
    """ Function to transform xy (bottom left) to ij (top left) coordinates. """
    (x, y) = xy
    (w, h) = dimension
    i = -y + h
    j = x
    return (j, i)  # Pygame axis


#---------------
def ij2xy(ij, dimension):
    """ Function to transform ij (top left) to xy (bottom left) coordinates. """
    (i, j) = ij
    (w, h) = dimension
    y = -i + h
    x = j
    return (x, y)


#---------------
def xy2center(xy, dimension):
    """ Function to transform xy (bottom left) to xy (centered) coordinates. """
    (x, y) = xy
    (w, h) = dimension
    (w_center, h_center) = (int(w/2), int(h/2))
    x -= w_center
    y -= h_center
    return (x, y)


#---------------
def center2xy(xy, dimension):
    """ Function to transform xy (centered) to xy (bottom left) coordinates. """
    (x, y) = xy
    (w, h) = dimension
    (w_center, h_center) = (int(w/2), int(h/2))
    x += w_center
    y += h_center
    return (x, y)





  





