"""
    Description.
"""


from yaml import load
try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader

from master import Master
from gui import GUI
from untils import Container
from queue import Queue


# ------------
if __name__ == '__main__':

    stream = open('../config/config.yaml', 'r')
    parameters = Container(load(stream, Loader=Loader))
    q = Queue()  # Queue shared by Master & GUI
    master = Master(q, parameters.shared, parameters.real_world)
    gui = GUI(q, parameters.shared, parameters.real_world)

    master.run()
    while gui.condition_loop:
        gui.check_user_event().update()
    gui.quit()









