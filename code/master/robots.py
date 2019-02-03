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

        self.q.robots2messenger.put("<1/1/0;20>")
        self.q.messenger2robots.get()
        time.sleep(3)

        self.q.robots2messenger.put("<1/2/5;10>")
        self.q.messenger2robots.get()
        time.sleep(3)

        self.q.robots2messenger.put("<1/3/5;10>")
        self.q.messenger2robots.get()
        time.sleep(3)

        self.q.robots2messenger.put("<1/4/1;15>")
        self.q.messenger2robots.get()
        time.sleep(3)


        













    