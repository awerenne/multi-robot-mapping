import numpy as np
from constants import *

opposite_orientation = {
    'N': 'S',
    'S': 'N',
    'W': 'E',
    'E': 'W'
}

# TODO!!
# Non encountered slave how to compute new position and orientation 



# Function to transform xy to ij coordinates
def get_coords_ij(xy, center):
        (x, y) = xy
        (c_x, c_y) = center
        i = -y + c_y
        j = x + c_x
        return (int(i), int(j))


# Function to transform xy to ij coordinates
def get_coords_xy(ij, center):
        (i, j) = ij
        (c_x, c_y) = center
        y = c_y - i
        x = j - c_x
        return (int(x), int(y))

def other_position(xy, orientation):
    (x, y) = xy
    if orientation == 'N':
        y += 1
    elif orientation == 'S':
        y -= 1
    elif orientation == 'E':
        x += 1
    else:
        x -= 1
    return (x,y)

def transform(ref_slave_id, ref_distance, ref_parameters, ext_other_slave_id, ext_distance, ext_parameters):
    (dimension_x, dimension_y) = ref_parameters["dimension"]
    center = (int(dimension_x/2), int(dimension_y/2))

    ref_slaves = ref_parameters["slaves"]
    ref_slave = ref_slaves[ref_slave_id]
    ref_slave_xy = (ref_slave["x"], ref_slave["y"])
    ref_slave_orientation = ref_slave["orientation"]
    ref_other_slave_xy = other_position(ref_slave_xy, ref_slave_orientation)
    ref_other_slave_ij = get_coords_ij(ref_other_slave_xy, center)
    ref_other_slave_orientation = opposite_orientation[ref_slave_orientation]
    ref_first_not_used_id = ref_parameters["first_not_used_id"]
    ref_v_c2i = ref_parameters["vertices_coords_to_id"]
    ref_v_i2c = ref_parameters["vertices_id_to_coords"]
    ref_edges = ref_parameters["edges"]
    ref_grid = ref_parameters["grid"]

    ext_slaves = ext_parameters["slaves"]
    ext_other_slave = ext_slaves[ext_other_slave_id]
    ext_other_slave_xy = (ext_other_slave["x"], ext_other_slave["y"])
    ext_other_slave_ij = get_coords_ij(ext_other_slave_xy, center)
    ext_v_c2i = ext_parameters["vertices_coords_to_id"]
    ext_v_i2c = ext_parameters["vertices_id_to_coords"]
    ext_edges = ext_parameters["edges"]
    ext_grid = ext_parameters["grid"]

    # Compute number of times to rotate (one rotation is 90° clockwise)
    orientations = {'N':0, 'E':1, 'S':2, 'W':3}
    orientation_a = ext_other_slave["orientation"]
    orientation_b = ref_other_slave_orientation
    n_rot = orientations[orientation_a] - orientations[orientation_b]
    if (n_rot < 0):
        n_rot += 4

    # Apply rotation (while keeping track of robot position)
    ext_grid[ext_other_slave_ij[0], ext_other_slave_ij[1]] = -1
    for i in range(n_rot):
        ext_grid = np.rot90(ext_grid)
    ext_other_slave_ij = np.argwhere(ext_grid == -1)[0]
    ext_grid[ext_other_slave_ij[0], ext_other_slave_ij[1]] = SLAVE

    # Compute translation vector
    v = ref_other_slave_ij - ext_other_slave_ij
    ext_pts = np.argwhere(ext_grid > 0)
    for ext_pt in ext_pts:
        ref_pt = ext_pt + v
        value_a = ref_grid[ref_pt[0], ref_pt[1]]
        value_b = ext_grid[ext_pt[0], ext_pt[1]]
        ref_grid[ref_pt[0], ref_pt[1]] = max(value_a, value_b)

    # Helper grid
    new_v_i2c = {}
    helper_grid = np.full(ref_grid.shape, -1)
    for v_id, v_coords in ext_v_i2c.items():
        (i, j) = get_coords_ij(v_coords, center)
        helper_grid[i, j] = v_id

    # Coordinates of new vertices
    for i in range(n_rot):
        helper_grid = np.rot90(helper_grid)
    helper_pts = np.argwhere(helper_grid >= 0)
    for helper_pt in helper_pts:
        ref_pt = helper_pt + v
        ref_pt_xy = get_coords_xy(ref_pt, center)
        v_id = helper_grid[helper_pt[0], helper_pt[1]]
        new_v_i2c[v_id] = (ref_pt_xy)

    # Merge new vertices and get new ids
    oi2ni = {}
    for old_id, v_coords in new_v_i2c.items():
        v_coords_key = str(v_coords[0]) +","+ str(v_coords[1])
        if v_coords_key in ref_v_c2i:
            oi2ni[old_id] = ref_v_c2i[v_coords_key]
        else:
            new_id = ref_first_not_used_id
            ref_first_not_used_id += 1
            ref_v_c2i[v_coords_key] = new_id
            ref_v_i2c[new_id] = v_coords
            oi2ni[old_id] = new_id
    
    # Modify edges to match new ids
    for key, distance in ext_edges.items():
        (a,b) = key.split(",")
        (a,b) = (int(a), int(b))
        a = oi2ni[a]
        b = oi2ni[b]
        key_ref = str(a) + "," + str(b)
        if key_ref not in ref_edges:
            ref_edges[key_ref] = distance

    # Add the new edge of encounter
    key = str(ref_slave["coming_from_x"]) + "," + str(ref_slave["coming_from_y"])
    ref_a_id = ref_v_c2i[key]
    key = str(ext_other_slave["coming_from_x"]) + "," + str(ext_other_slave["coming_from_y"])
    ext_b_id = ext_v_c2i[key]
    ref_b_id = oi2ni[ext_b_id]
    key = str(ref_a_id) + "," + str(ref_b_id)
    inversed_key = str(ref_b_id) + "," + str(ref_a_id)
    ref_edges[key] = ref_distance + ext_distance
    ref_edges[inversed_key] = ref_distance + ext_distance

    ref_other_slave = {
        "x": ref_other_slave_xy[1],
        "coming_from_x": ref_other_slave_xy[1],
        "y": ref_other_slave_xy[1],
        "coming_from_y": ref_other_slave_xy[1],
        "orientation": ref_other_slave_orientation, 
        "slave_meeting": True
    }
    ref_slave["slave_meeting"] = True
    parameters = {}
    parameters["slaves"] = {ref_slave_id: ref_slave, ext_other_slave_id: ref_other_slave}
    parameters["first_not_used_id"] = ref_first_not_used_id
    parameters["vertices_coords_to_id"] = ref_v_c2i 
    parameters["vertices_id_to_coords"] = ref_v_i2c
    parameters["edges"] = ref_edges 
    parameters["grid"] = ref_grid 
    return parameters


#---------------
class Container(dict):
    """
    Dictionary whose keys can be accessed as attributes.
    """

    def __init__(self, *args, **kwargs):
        super(Parameters, self).__init__(*args, **kwargs)

    def __getattr__(self, item):
        if item not in self:
            return None
        if type(self[item]) is dict:
            self[item] = Parameters(self[item])
        return self[item]



  





