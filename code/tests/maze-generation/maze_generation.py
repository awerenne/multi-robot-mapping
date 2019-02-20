# Don't forget with mask -> cool!

import random
import numpy as np
from math import floor

dimension = 20
WALL = 0
EMPTY = 1
d_i = dimension
d_j = floor(0.7*dimension)
grid = np.zeros((d_i, d_j))


def sample(neighbors):
    if len(neighbors) == 0:
        return None
    return random.choice(neighbors)

def neighbors(cell):
    n = []
    (i, j) = cell
    if i > 1: n.append((i-2,j)) 
    if i < d_i-2: n.append((i+2,j)) 
    if j > 1: n.append((i,j-2)) 
    if j < d_j-2: n.append((i,j+2))
    return n

def link(current_cell, next_cell):
    (a_i, a_j) = current_cell
    (b_i, b_j) = next_cell
    if (a_i != b_i):
        grid[int((a_i+b_i)/2), a_j] = EMPTY
    else:
        grid[a_i, int((a_j+b_j)/2)] = EMPTY
    

# print()
# print(grid)
# print

stack = []
# Choose a starting cell at random
current_cell = (random.randint(0, d_i-1), random.randint(0, d_j-1))
grid[current_cell[0], current_cell[1]] = EMPTY
stack.append(current_cell)

visited = set()
visited.add(current_cell)

while len(stack) > 0:
    # Choose an unvisited neighbor at random
    n = [cell for cell in neighbors(current_cell) if cell not in visited]
    
    # If no unvisited neighbor then we are done with this current cell
    # and need to backtrack to the previously discivered cell
    if len(n) == 0:
        current_cell = stack.pop()
    else:
        # Found an visited neighbor. Move to that cell and mark is
        # current. Also, link the current cell with the neighbor
        neighbor_cell = sample(n)
        visited.add(neighbor_cell)
        link(current_cell, neighbor_cell)
        stack.append(neighbor_cell)
        current_cell = neighbor_cell
        grid[current_cell[0], current_cell[1]] = EMPTY

print()
print(grid)
print

from PIL import Image
import PIL.ImageOps

img = Image.fromarray(np.uint8(255*grid))
img = PIL.ImageOps.invert(img)
img.save('my.png')
img.show()








