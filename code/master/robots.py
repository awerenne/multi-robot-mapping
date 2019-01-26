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
        while True:
            self.temp()
            time.sleep(5)

    def temp(self):

        self.q.robots2messenger.put("<1/1/0;50>")
        self.q.messenger2robots.get()
        time.sleep(5)

        self.q.robots2messenger.put("<2/1/1;100>")
        self.q.messenger2robots.get()
        time.sleep(10)

        self.q.robots2messenger.put("<1/2/3;80>")
        self.q.messenger2robots.get()
        time.sleep(10)

        self.q.robots2messenger.put("<1/2/0;35>")
        self.q.messenger2robots.get()
        time.sleep(10)


        













    