"""
    Description.
"""

import numpy as np
from generation.main import generate


#---------------
class Environment():
    """
    ...
    """

    def __init__(self):
        self._ground_truth = [((0, 0), (0, 40))] + generate()
        if not ((0, 40), (0, 60)) in self._ground_truth:
            self._ground_truth += [((0, 40), (0, 60))]
        # self._ground_truth = [((0, 0), (0, 40)),((-40, 80), (-20, 80)), ((-40, 100), (-20, 100)), ((-40, 140), (-20, 140)), ((-20, 40), (-20, 60)), ((-20, 60), (-20, 80)), ((-20, 60), (0, 60)), ((-20, 100), (0, 100)), ((-20, 120), (-20, 140)), ((-20, 120), (0, 120)), ((-20, 140), (0, 140)), ((0, 40), (0, 60)), ((0, 60), (20, 60)), ((0, 80), (0, 100)), ((0, 120), (0, 140)), ((0, 120), (20, 120)), ((0, 140), (0, 160)), ((20, 40), (20, 60)), ((20, 60), (40, 60)), ((20, 80), (20, 100)), ((20, 100), (20, 120)), ((20, 100), (40, 100)), ((20, 140), (20, 160)), ((20, 140), (40, 140)), ((-40, 40), (-40, 60)), ((-40, 40), (-20, 40)), ((-40, 60), (-40, 80)), ((-40, 80), (-40, 100)), ((-40, 100), (-40, 120)), ((-40, 120), (-40, 140)), ((-40, 140), (-40, 160)), ((-40, 160), (-20, 160)), ((-20, 40), (0, 40)), ((-20, 160), (0, 160)), ((0, 40), (20, 40)), ((0, 160), (20, 160)), ((20, 40), (40, 40)), ((20, 160), (40, 160)), ((40, 40), (40, 60)), ((40, 60), (40, 80)), ((40, 80), (40, 100)), ((40, 100), (40, 120)), ((40, 120), (40, 140)), ((40, 140), (40, 160))]
        self.grid = Grid(self.ground_truth)
        self.robots = {1: (0,20,'north'), 2: (0,0,'north')}

    @property
    def ground_truth(self):
        return self._ground_truth
    
    # -------
    def get_robots(self):
        myrobots = []
        for key, value in self.robots.items():
            x, y, orientation = value
            temp = {'north': 0, 'east':1, 'south':2, 'west':3}
            myrobots.append((x, y, temp[orientation]))
        return myrobots

    # -------
    def move(self, id_robot):
        x, y, orientation = self.robots[id_robot]
        if id_robot == 1:
            x_other, y_other, _ = self.robots[2]
        else:
            x_other, y_other, _ = self.robots[1]
        if orientation == 'north' and self.grid.is_path(x,y+5):
            self.robots[id_robot] = (x, y+5, orientation)
        elif orientation == 'south' and self.grid.is_path(x,y-5):
            self.robots[id_robot] = (x, y-5, orientation)
        elif orientation == 'east' and self.grid.is_path(x+5,y):
            self.robots[id_robot] = (x+5, y, orientation)
        elif orientation == 'west' and self.grid.is_path(x-5,y):
            self.robots[id_robot] = (x-5, y, orientation)
        # if orientation == 'north' and self.grid.is_path(x,y+10) and (x==x_other) and y+10 != y_other:
        #     self.robots[id_robot] = (x, y+10, orientation)
        # elif orientation == 'south' and self.grid.is_path(x,y-10) and (x==x_other) and y-10 != y_other:
        #     self.robots[id_robot] = (x, y-10, orientation)
        # elif orientation == 'east' and self.grid.is_path(x+10,y) and (y==y_other) and x+10 != x_other:
        #     self.robots[id_robot] = (x+10, y, orientation)
        # elif orientation == 'west' and self.grid.is_path(x-10,y) and (y==y_other) and x-10 != x_other:
        #     self.robots[id_robot] = (x-10, y, orientation)
        return self

    # -------
    def turn_left(self, id_robot):
        x, y, orientation = self.robots[id_robot]
        if orientation == 'north':
            self.robots[id_robot] = (x, y, 'west')
        elif orientation == 'south':
            self.robots[id_robot] = (x, y, 'east')
        elif orientation == 'east':
            self.robots[id_robot] = (x, y, 'north')
        elif orientation == 'west':
            self.robots[id_robot] = (x, y, 'south')
        return self

    # -------
    def turn_right(self, id_robot):
        x, y, orientation = self.robots[id_robot]
        if orientation == 'north':
            self.robots[id_robot] = (x, y, 'east')
        elif orientation == 'south':
            self.robots[id_robot] = (x, y, 'west')
        elif orientation == 'east':
            self.robots[id_robot] = (x, y, 'south')
        elif orientation == 'west':
            self.robots[id_robot] = (x, y, 'north')
        return self

    # -------
    def uturn(self, id_robot):
        x, y, orientation = self.robots[id_robot]
        if orientation == 'north':
            self.robots[id_robot] = (x, y, 'south')
        elif orientation == 'south':
            self.robots[id_robot] = (x, y, 'north')
        elif orientation == 'east':
            self.robots[id_robot] = (x, y, 'west')
        elif orientation == 'west':
            self.robots[id_robot] = (x, y, 'east')
        return self

    # -------
    def sensor_read(self, id_robot):
        x, y, orientation = self.robots[id_robot]
        if id_robot == 1:
            x_other, y_other, _ =  self.robots[2]
        else: 
            x_other, y_other, _ =  self.robots[1]
        if orientation == 'north':
            readings = (self.grid.is_path(x-5,y), self.grid.is_path(x,y+5), 
                    self.grid.is_path(x+5,y))
            if x == x_other and y+5 == y_other:
                return (-1, -1, -1)
        elif orientation == 'south':
            readings = (self.grid.is_path(x+5,y), self.grid.is_path(x,y-5), 
                    self.grid.is_path(x-5,y))
            if x == x_other and y-5 == y_other:
                return (-1, -1, -1)
        elif orientation == 'east':
            readings = (self.grid.is_path(x,y+5), self.grid.is_path(x+5,y), 
                    self.grid.is_path(x,y-5))
            if y == y_other and x+5 == x_other:
                return (-1, -1, -1)
        elif orientation == 'west':
            readings = (self.grid.is_path(x,y-5), self.grid.is_path(x-5,y), 
                    self.grid.is_path(x,y+5))
            if y == y_other and x-5 == x_other:
                return (-1, -1, -1)
        # print()
        # print(self.robots[id_robot])
        # print(readings)
        return readings


#---------------
class Grid():
    """
    ...
    """

    def __init__(self, edges):
        self.edges = edges
        w, h = 160, 160
        self.dimension = (w, h)
        self.m = np.zeros((int(h/5)+1, int(w/5)+1))
        self.fill()
    
    # -------
    def fill(self):
        for edge in self.edges:
            a, b = edge
            a, b = self.xy2ij(a[0], a[1]), self.xy2ij(b[0], b[1])
            min_i = min(a[0], b[0])
            max_i = max(a[0], b[0])
            min_j = min(a[1], b[1])
            max_j = max(a[1], b[1])
            if min_i == max_i:
                self.m[min_i, min_j:max_j+1] = 1
            else:
                self.m[min_i:max_i+1, min_j] = 1
        return self

    # -------
    def is_path(self, x, y):
        if x < -self.dimension[0] or x > self.dimension[0] or y < 0 or y > self.dimension[1]:
            return False
        i, j = self.xy2ij(x, y)
        return self.m[i,j] == 1

    # -------
    def xy2ij(self, x, y):
        (w, h) = self.dimension
        (w_center, h_center) = (int(w/2), int(h/2))
        i = int((h-y)/5)
        j = int((x+w_center)/5)
        return (i, j)

   









    