# Multi-robot environment mapping 

The goal of this project was to construct two robots, able to map unknown environments as quickly as possible. The considered environments are exclusively composed of straight black lines on a white background, intersecting each other with right angles. The robots being only allowed to move along those black lines during the exploration phase.

The developed solution follows a master-slave architecture. Each robot (slave) follows a black line while keeping track of the travelled distance. Once an intersection detected, the robot stops and sends an information message to the master. The message contains the type of intersection encountered and the travelled distance since the previous intersection. This information is then used by a program (master), running locally on a computer, in order to update its internal representation of the environment (i.e. map). The next instruction (stop, straight, turn left or right, uturn) for the robot is then computed and send back to the rboto. This procedure is repeated until the environment is entirely explored.

<p align="center">
    <img width="400" src="gifs/gif-left.gif">&nbsp;&nbsp;&nbsp;
    <img width="400" src="gifs/gif-right.gif">
</p>

The left-part of the animation above shows a live screen recording of the master constructing the map, corresponding to the robot exploration performed (displayed in the right-part). A video of the complete demonstration can be found [here](https://www.youtube.com/watch?v=w8zaz-Xr6bw&t=6s). The implementation and results are described in the [report](https://awerenne.github.io/files/pp-report.pdf). The different steps of the construction of the robots are documented [here](https://github.com/awerenne/multi-robot-mapping/blob/master/construction/README.md).

## Simulator
Testing the mapping algorithm with the exploration being performed by physical robots would have been time-consuming. Instead, the exploration was simulated on random environments. 

<p align="center">
    <img width="500" src="gifs/gif-simul.gif">
</p>

The algorithm of the master is explained in the [report](report/personal-project.pdf)) (pages 8-11). A simulation can be lauched with <code>python main.py</code> in the folder *code/simulator/*. The flag <code>is_simulation</code> needs to be set to true in the config file.

## Robot
The control of the robot was tested with a simple challenge: following a curved line at a constant speed. To this end, a speed and steering controller were developed (described in the [report](report/personal-project.pdf), pages 20-24). The result is shown in the animation below.

<p align="center">
    <img width="500" src="gifs/gif-control.gif">
</p>

The robot was then further augmented to loop through a serie of steps during the exploration phase. The high-level view of the main algorithm (serie of steps) is implemented in the file *code/robot/mapping/mapping.ino*, which is based on low-level features implemented in the files located in the folder *code/robot/NewCOEX/*. The acronym COEX stands for cooperative explorer. 

<p align="center">
    <img width="500" src="gifs/gif-steps.gif">
</p>

## Usage
The simulator can be launched as follows:
1. Set the flag <code>is_simulation</code> to True in the config file.
2. Run the command <code>python main.py</code> (in the folder *code/simulator/*).

The physical exploration can be launched as follows:
1. Copy the folder *code/robot/NewCoex* in your Arduino folder, and load it as a library ([how to load a library](https://www.arduino.cc/en/guide/libraries)).
2. Install the library corresponding to the [QTR-X sensors](https://www.pololu.com/docs/0J19/2).
3. Upload the program *code/robot/mapping/mapping.ino* to the micro-controller.
4. Set the flag <code>is_simulation</code> to False in the config file.
5. Check that the ports in the config files are correctly configured with the names of the bluetooth modules.
5. Switch the power of the robots on and calibrate the line sensors.
6. Run the command <code>python main.py</code> (in the folder *code/master/*).





