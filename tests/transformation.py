import numpy as np

# Constants 
WALL_TYPE = np.int8
WALL = 0
EMPTY = 1
SLAVE = 2
UNEXPLORED = 3
PATH = 4
DIMENSION = 15

center = int(DIMENSION/2)

# Function to transform xy to ij coordinates
def get_coords_ij(x, y):
        (center_i, center_j) = (center,center)
        i = -y + center_i
        j = x + center_j
        return (int(i), int(j))

# Function to transform xy to ij coordinates
def get_coords_xy(i, j):
        (center_i, center_j) = (center,center)
        y = center_i - i
        x = j - center_j
        return (int(x), int(y))

# Initialize reference grid and metadata
ref_grid = np.zeros((DIMENSION, DIMENSION))
ref_grid[center-2:center+1, center] = EMPTY
ref_grid[center-2, center:center+2] = EMPTY

ref_slave_xy = np.asarray([1,2])
ref_other_slave_xy = np.asarray([2,2])
ref_slave_ij = get_coords_ij(ref_slave_xy[0], ref_slave_xy[1])
ref_other_slave_ij = get_coords_ij(ref_other_slave_xy[0], ref_other_slave_xy[1])
ref_grid[ref_slave_ij[0], ref_slave_ij[1]] = SLAVE
ref_slave_orientation = 'E'
ref_other_slave_orientation = 'W'
ref_vertices_i2c = {
    0: (0,0),
    1: (0,2)
}
ref_vertices_c2i = {
    "0,0": 0,
    "0,2": 1
}
ref_edges = {
    "0,1": 2
}


# Initialize reference grid and metadata
ext_grid = np.zeros((DIMENSION, DIMENSION))
ext_grid[center-4:center+1, center] = EMPTY
ext_grid[center-4, center:center+3] = EMPTY
ext_grid[center-4:center-1, center+2] = EMPTY
ext_grid[center-2, center+2:center+5] = EMPTY

ext_other_slave_xy = np.asarray([4,2])
ext_other_slave_ij = get_coords_ij(ext_other_slave_xy[0], ext_other_slave_xy[1])
ext_grid[ext_other_slave_ij[0], ext_other_slave_ij[1]] = SLAVE
ext_other_slave_orientation = 'E'
ext_vertices_i2c = {
    0: (0,0),
    1: (0,4),
    2: (2,4),
    3: (2,2)}
ext_vertices_c2i = {
    "0,0": 0,
    "0,4": 1,
    "2,4": 2,
    "2,2": 3 
}
ext_edges = {
    "0,1": 4,
    "1,2": 2,
    "2,3": 2,
}


# Compute number of times to rotate (one rotation is 90° clockwise)
orientations = {'N':0, 'E':1, 'S':2, 'W':3}
rot = orientations[ext_other_slave_orientation] - orientations[ref_other_slave_orientation]
if (rot < 0):
    rot += 4

# Apply rotation (while keeping track of robot position)
for i in range(rot):
    ext_grid = np.rot90(ext_grid)
ext_other_slave_ij = np.argwhere(ext_grid == SLAVE)[0]


# Compute translation vector
v = ref_other_slave_ij - ext_other_slave_ij
ext_pts = np.argwhere(ext_grid > 0)
for ext_pt in ext_pts:
    ref_pt = ext_pt + v
    # TODO priority!!
    ref_grid[ref_pt[0], ref_pt[1]] = ext_grid[ext_pt[0], ext_pt[1]]

print()
print(ref_grid)
print()




# Helper grid
new_v_i2c = {}
helper_grid = np.full(ref_grid.shape, -1)
for v_id, coords in ext_vertices_i2c.items():
    (i, j) = get_coords_ij(coords[0], coords[1])
    helper_grid[i, j] = v_id

for i in range(rot):
    helper_grid = np.rot90(helper_grid)
helper_pts = np.argwhere(helper_grid >= 0)
for helper_pt in helper_pts:
    ref_pt = helper_pt + v
    ref_pt_xy = get_coords_xy(ref_pt[0], ref_pt[1])
    v_id = helper_grid[helper_pt[0], helper_pt[1]]
    new_v_i2c[v_id] = (ref_pt_xy)

# new_ext_edges = {}
# for key, distance in ext_edges.items():
#     (a,b) = key.split(",")
#     (a,b) = (int(a), int(b))
#     a_coords = new_v_i2c[a]
#     b_coords = new_v_i2c[b]
#     new_key = str(a_coords[0])+","+str(a_coords[1])+";"+str(b_coords[0])+","+str(b_coords[1])
#     new_ext_edges[new_key] = distance
# print(new_ext_edges)

oi2ni = {}
first_not_used = 2  
for old_id, v_coords in new_v_i2c.items():
    v_coords_key = str(v_coords[0]) +","+ str(v_coords[1])
    if v_coords_key in ref_vertices_c2i:
        oi2ni[old_id] = ref_vertices_c2i[v_coords_key]
    else:
        new_id = first_not_used
        first_not_used += 1
        ref_vertices_c2i[v_coords_key]=new_id
        ref_vertices_i2c[new_id]=v_coords_key
        oi2ni[old_id]=new_id
# print(oi2ni)
# print(ref_vertices_c2i)

for key, distance in ext_edges.items():
    (a,b) = key.split(",")
    (a,b) = (int(a), int(b))
    a = oi2ni[a]
    b = oi2ni[b]
    key_ref = str(a) + "," + str(b)
    if key_ref not in ref_edges:
        ref_edges[key_ref] = distance

print(ref_vertices_c2i)
print(ref_edges)


# Add new edge
ref_slave_coming_from = (0,2)
key = str(ref_slave_coming_from[0]) + "," + str(ref_slave_coming_from[1])
my_id = ref_vertices_c2i[key]
ext_slave_coming_from = (2,2)
key = str(ext_slave_coming_from[0]) + "," + str(ext_slave_coming_from[1])
old_id = ext_vertices_c2i[key]
new_id = oi2ni[old_id]
ref_other_slave_coming_from = ref_vertices_i2c[new_id]
key = str(my_id)+","+str(new_id)
ref_edges[key]=4
print(ref_edges)






  





