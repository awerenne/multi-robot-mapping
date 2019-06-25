"""
    Description.
"""


from random import sample 




# Return all edges with simplification

N_ROWS = 4
N_COLS = 6

#---------------
class Node:
    def __init__(self, coords):
        self._neighbors = []
        self.flag = False
        self._coords = coords

    #-------
    @property
    def coords(self):
        return self._coords

    #-------
    @property
    def neighbors(self):
        return self._neighbors

    #-------
    def __str__(self):
        return str(self.coords)

    #-------
    def get_flag(self):
        return self.flag

    #-------
    def set_flag(self, flag):
        self.flag = flag
        return self

    #-------
    def connect(self, neigbor):
        for other in self._neighbors:
            if other == neigbor: return self
        assert len(self._neighbors) <= 3
        self._neighbors.append(neigbor)

    #-------
    def disconnect(self, neigbor):
        self._neighbors.remove(neigbor)
        return self


#---------------
def idx2coord(idx):
    i, j = idx
    return (i*20, j*20)


#---------------
def coord2idx(coord):
    a, b = coord
    return (floor(a/20), floor(b/20))


#---------------
def initialization():
    all_nodes = {}
    for i in range(N_ROWS):
        for j in range(N_COLS):
            coords = idx2coord((i,j))
            all_nodes[coords] = Node(coords)
    return all_nodes


#---------------
def construction(all_nodes):
    """ Connect nodes horizontally """
    for i in range(N_ROWS):
        prev_node = all_nodes[idx2coord((i,0))]
        for j in range(1, N_COLS):
            coords = idx2coord((i,j))
            node = all_nodes[coords]
            node.connect(prev_node.coords)
            prev_node.connect(node.coords)
            prev_node = node
    """ Connect nodes vertically """
    for j in range(N_COLS):
        prev_node = all_nodes[idx2coord((0,j))]
        for i in range(1, N_ROWS):
            coords = idx2coord((i,j))
            node = all_nodes[coords]
            node.connect(prev_node.coords)
            prev_node.connect(node.coords)
            prev_node = node


#---------------
def random_destruction(all_nodes, inside_edges):
    to_remove = sample(inside_edges, int(0.8*len(inside_edges)))
    for edge in to_remove:
        coords_1, coords_2 = edge
        node_1, node_2 = all_nodes[coords_1], all_nodes[coords_2]
        node_1.disconnect(coords_2)
        if len(node_1.neighbors) == 0: del all_nodes[coords_1]
        node_2.disconnect(coords_1)
        if len(node_2.neighbors) == 0: del all_nodes[coords_2]


#---------------
def recursion(all_nodes, node):
    node.set_flag(True)
    for neigbor in node.neighbors:
        new_node = all_nodes[neigbor]
        if not new_node.get_flag():
            recursion(all_nodes, new_node)

#---------------
def connectivity(all_nodes):
    reset(all_nodes)
    recursion(all_nodes, all_nodes[(0,0)])
    for coords, node in all_nodes.items():
            if not node.get_flag():
                return False
    return True


#---------------
def fixing(all_nodes):
    while not connectivity(all_nodes):
        not_connected = []
        for coords, node in all_nodes.items():
            if not node.get_flag():
                not_connected.append(coords)
        node = all_nodes[sample(not_connected, 1)[0]]
        remaining_directions = ["up", "down", "left", "right"]
        for neigbor in node.neighbors:
            if node.coords[1] < neigbor[1]:
                remaining_directions.remove("right")
            elif node.coords[1] > neigbor[1]:
                remaining_directions.remove("left")
            if node.coords[0] < neigbor[0]:
                remaining_directions.remove("down")
            elif node.coords[0] > neigbor[0]:
                remaining_directions.remove("up")
        direction = sample(remaining_directions, 1)[0]
        if direction == "up":
            new_neighbor = (node.coords[0]-20, node.coords[1])
        elif direction == "down":
            new_neighbor = (node.coords[0]+20, node.coords[1])
        elif direction == "left":
            new_neighbor = (node.coords[0], node.coords[1]-20)
        elif direction == "right":
            new_neighbor = (node.coords[0], node.coords[1]+20)
        if new_neighbor not in all_nodes:
            all_nodes[new_neighbor] = Node(new_neighbor)
        all_nodes[new_neighbor].connect(node.coords)
        node.connect(new_neighbor)
        print(node.coords, new_neighbor)

#---------------
def reset(all_nodes):
    for coords, node in all_nodes.items():
            node.set_flag(False)

#---------------
def get_edges(all_nodes):
    reset(all_nodes)
    inside_edges = []
    exter_edges = []
    for i in range(N_ROWS):
        for j in range(N_COLS):
            coords = idx2coord((i,j))
            if coords not in all_nodes:
                continue
            node = all_nodes[coords]
            if node.get_flag():
                continue
            neighbors = node.neighbors
            for neigbor in neighbors:
                if neigbor < node.coords: 
                    continue
                # First row
                if node.coords[0] == 0 and neigbor[0] == 0:
                    exter_edges.append((node.coords, neigbor))
                # Last row
                elif node.coords[0] == N_ROWS-1 and neigbor[0] == N_ROWS-1:
                    exter_edges.append((node.coords, neigbor))
                # First column
                elif node.coords[1] == 0 and neigbor[1] == 0:
                    exter_edges.append((node.coords, neigbor))
                # Last column
                elif node.coords[1] == N_COLS-1 and neigbor[1] == N_COLS-1:
                    exter_edges.append((node.coords, neigbor))
                else:
                    inside_edges.append((node.coords, neigbor))
            node.set_flag(True)
    reset(all_nodes)
    return inside_edges, exter_edges


#---------------
if __name__ == "__main__":
    all_nodes = initialization()
    construction(all_nodes)

    inside_edges, exter_edges = get_edges(all_nodes)
    print(inside_edges)
    print()

    random_destruction(all_nodes, inside_edges)
    inside_edges, exter_edges = get_edges(all_nodes)
    print(inside_edges)
    print()

    fixing(all_nodes)
    inside_edges, exter_edges = get_edges(all_nodes)
    print(inside_edges)
    print()













