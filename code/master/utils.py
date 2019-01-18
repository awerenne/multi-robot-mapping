"""
    Description.
"""


import numpy as np


# ------------
class PriorityQueue():
    """
    Wrapper for heapq. Used for A* algorithm.
    """

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


#---------------
class Container(dict):
    """
    Dictionary whose keys can be accessed as attributes.
    """

    def __init__(self, *args, **kwargs):
        super(Container, self).__init__(*args, **kwargs)

    def __getattr__(self, item):
        if item not in self:
            return None
        if type(self[item]) is dict:
            self[item] = Container(self[item])
        return self[item]


#---------------
def xy2ij(xy, dimension):
    """ Function to transform xy (bottom left) to ij (top left) coordinates. """

    (x, y) = xy
    (w, h) = dimension
    i = -y + h
    j = x
    return (i, j)


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
    y -= y_center
    return (x, y)


#---------------
def center2xy(xy, dimension):
    """ Function to transform xy (centered) to xy (bottom left) coordinates. """

    (x, y) = xy
    (w, h) = dimension
    (w_center, h_center) = (int(w/2), int(h/2))
    x += w_center
    y += y_center
    return (x, y)





  





