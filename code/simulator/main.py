"""
    Description.
"""


from queue import Queue

import sys
sys.path.append('../master/')
from master import NaiveMaster
from gui import GUI
from utils import Parameters, Container
from robots import Robot
from environment import Environment


#---------------
def make_queues():
    q_master = {
        "master2gui": Queue(),
        "gui2master": Queue(),
        "robots2messenger": Queue(),
        "messenger2robots": Queue()
    }
    q_gui = {
        "master2gui": q_master["master2gui"],
        "gui2master": q_master["gui2master"]
    }
    q_robots = {
        "robots2messenger": q_master["robots2messenger"],
        "messenger2robots": q_master["messenger2robots"]
    }
    q = {
        "master": q_master,
        "robots": q_robots,
        "gui": q_gui
    } 
    return Container(q)


#---------------
if __name__ == '__main__':

    params = Parameters(path="../config/config.yaml")
    q = make_queues()
    env = Environment()
    robot1 = Robot(1, q.robots, env)
    robot2 = Robot(2, q.robots, env)
    master = NaiveMaster(params, q.master)
    gui = GUI(params, q.gui, env)

    # Start child threads
    robot1.start()
    robot2.start()
    master.start()

    # Main thread
    while not gui.close:
        gui.check_user_event().update()

    # Quit properly threads TODO






