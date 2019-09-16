
from queue import Queue
from master import NaiveMaster
from gui import GUI
from utils import Parameters, Container
# from robots import Robots
# from environment import Environment

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

    params = Parameters(path="../config.yaml")
    q = make_queues()
    # env = Environment().new()
    # robots = Robots(params, q.robots)
    master = NaiveMaster(params, q.master, lambda_=0.2)
    gui = GUI(params, q.gui, None)

    # Start child threads
    # robots.start()
    master.start()

    # Main thread
    while not gui.close:
        gui.check_user_event().update()







