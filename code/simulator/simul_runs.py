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
import numpy as np
import time


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

    mean_time = []
    std_time = []
    for lambda_ in np.linspace(0,2,21):
        times = []
        n_failures = 0
        for i in range(10):
            q = make_queues()
            env = Environment()
            gt = set(env.ground_truth)
            robot1 = Robot(1, q.robots, env)
            robot2 = Robot(2, q.robots, env)
            master = NaiveMaster(params, q.master, lambda_=lambda_)

            robot1.start()
            robot2.start()
            master.start()
            directive = {"type_directive": "request_run"}
            q.gui.gui2master.put(Container(directive))
            edges = {}
            start_time = time.time()
            while True:
                if not q.gui.master2gui.empty():
                    directive = q.gui.master2gui.get_nowait()
                    edges = set(directive.summary.edges)
                common = gt.intersection(edges)
                overlap = float(len(common))/float(len(gt))
                time.sleep(0.05)
                current_time = time.time()
                if current_time - start_time > 60 or master.finished:
                    break
            if master.finished:
                times.append((current_time - start_time)/len(gt))
            else:
                n_failures += 1
        print()
        print(n_failures)
        times = np.asarray(times)
        mean = np.mean(times)
        mean_time.append(mean)
        std = np.std(times)
        std_time.append(std)
        print(lambda_)
        print(mean)
        print(std)
        print()
    print(np.linspace(0.125,0.875,4))
    print(mean_time)
    print(std_time)








