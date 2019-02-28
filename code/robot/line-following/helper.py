"""
    Description.
"""


import serial
import time


# serial_port = '/dev/cu.DSDTECHHC-05-DevB';
# baud_rate = 9600; 
# ser = serial.Serial(serial_port, baud_rate)

import pylab
import matplotlib
matplotlib.use("Agg")
import matplotlib.backends.backend_agg as agg
import matplotlib.pyplot as plt
import matplotlib.animation as animation
plt.style.use('seaborn-whitegrid')




fig = plt.figure(figsize=[4, 4], # Inches
                   dpi=100,        # 100 dots per inch, so the resulting buffer is 400x400 pixels
                   )
ax = fig.gca()
ax.plot([])

canvas = agg.FigureCanvasAgg(fig)
canvas.draw()
renderer = canvas.get_renderer()
raw_data = renderer.tostring_rgb()




import pygame
from pygame.locals import *

pygame.init()

window = pygame.display.set_mode((900, 400), DOUBLEBUF)
screen = pygame.display.get_surface()

size = canvas.get_width_height()

while True:
    time.sleep(0.01)
    ax.plot([1])
    canvas = agg.FigureCanvasAgg(fig)
    canvas.draw()
    renderer = canvas.get_renderer()
    raw_data = renderer.tostring_rgb()
    surf = pygame.image.fromstring(raw_data, size, "RGB")
    screen.blit(surf, (0,0))
    pygame.display.flip()
    #plt.pause(0.001)

    crashed = False
    while not crashed:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                crashed = True
            
