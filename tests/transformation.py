import numpy as np

DIMENSION = 11
center = int(DIMENSION/2)

def get_coords(x, y):
        (center_i, center_j) = (center,center)
        i = -y + center_i
        j = x + center_j
        return (int(i), int(j))

map_1 = np.zeros((DIMENSION, DIMENSION))
map_2 = np.zeros((DIMENSION, DIMENSION))

# Fill with path
map_1[center-2:center+1, center] = 1
map_1[center-2,center-2:center] = 1
map_2[center-3:center+1, center] = 1
map_2[center-3, center:center+2] = 1

print()
print(map_1)
print()

print()
print(map_2)
print()

# Transform
absolute_pos = np.asarray([-3,2])
absolute_pos_ij = get_coords(absolute_pos[0], absolute_pos[1])
abs_or = 'E'
relative_pos = np.asarray([1,3])
relative_pos_ij = get_coords(relative_pos[0], relative_pos[1])
rel_or = 'E'

map_2[relative_pos_ij]=5
orientations = {'N':0, 'E':1, 'S':2, 'W':3}
rot = orientations[rel_or] - orientations[abs_or]
if (rot < 0):
    rot += 4
for i in range(rot):
    map_2 = np.rot90(map_2)
relative_pos_ij = np.argwhere(map_2 == 5)[0]
map_2[relative_pos_ij[0], relative_pos_ij[1]] = 1

print()
print(map_2)
print()


v = absolute_pos_ij - relative_pos_ij
print(v)
relative_pts = np.argwhere(map_2 > 0)
absolute_pts = relative_pts + v
for pt in absolute_pts:
    map_1[pt[0], pt[1]] = 2

print()
print(map_1)
print()