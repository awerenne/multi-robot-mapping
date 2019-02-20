"""
    Description.
"""

import sys
sys.path.append('../master/')
from utils import Parameters, Container



if __name__ == "__main__":
    params = Parameters()
    print(type(params))
    print(type(params.robots))
    id_ = 1
    print(type(params.robots.id_))

    print(params.real)
    print(params.is_simulation)
    print(params.n_robots)
    print(params.robots.r1.pose)
    

    params.container["new_item"] = 5
    print(params.new_item)
