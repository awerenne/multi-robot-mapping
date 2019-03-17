"""
    Description.
"""


from threading import Thread
import time


#---------------
class Robots(Thread):
    """
    Single-agent. Multi-agent. One vector of states. One vector of actions.
    receives instruction from master -> acts on Environment -> Environment 
    simulates feedback sensors to Robots -> Robots sends information to
    Master. Loop again.
    """

    def __init__(self, params, queues):
        Thread.__init__(self)
        self.params = params
        self.q = queues

    def run(self):
        self.q.messenger2robots.get()  # Run instruction
        self.temp()
        while True: time.sleep(5)

    def temp(self):
        infos = ["4;40", "3;20", "6;20", "2;15", "4;25", "0;40", "4;45", "5;20",
                    "6;15"]

        for seq_number, info in enumerate(infos):
            msg = "<1/" + str(seq_number) + "/" + info + ">"
            self.q.robots2messenger.put(msg)
            time.sleep(1.5)


        













    