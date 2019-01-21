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
from utils import Container
from queue import Queue

# TODO: when everything is finished, change instantiations accordingly

#---------------
if __name__ == '__main__':

    stream = open('../config/config.yaml', 'r')
    parameters = Container(load(stream, Loader=Loader))
    queu_gui_to_master = Queue()
    queu_master_2_gui =  Queue()
    master = Master(queu_master_2_gui, queu_gui_to_master, parameters.shared, parameters.real_world)
    gui = GUI(queu_master_2_gui, queu_gui_to_master, parameters.shared, parameters.real_world)

    master.run()
    while gui.condition_loop:
        gui.check_user_event().update()
    gui.quit()









