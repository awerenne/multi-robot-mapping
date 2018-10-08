import sys
sys.path.append('../simulator/')


class Messenger:

    def __init__(self, master, worker):
        self.master = master
        self.master_id = self.master.get_id()
        self.worker = worker
        self.worker_id = self.worker.get_id()

        master.connect(self, self.worker_id)
        worker.connect(self, self.master_id)


    def send(self, receiver_id, type_message, content_message):
        if receiver_id == self.master_id:
            self.master.receive_msg(self.worker_id, type_message,
                content_message)
        else:
            self.worker.receive_msg(self.master_id, type_message, 
                content_message)