import threading  
import time
from queue import Queue

q = Queue()

def f_1():
    for i in range(1000):
        print("** Put new element: ", i)
        q.put(i)
        time.sleep(0.8)

def f_2():
    while True:
        while not q.empty():
            print("++ Found new element: ", q.get())
        time.sleep(0.2)


# Create two threads: one for the user interface and the second for reception 
# messages
try:
    t1=threading.Thread(target=f_1).start()
    t2=threading.Thread(target=f_2).start()

except:
    print("Error: unable to start threads")
