# Multi-robot environment mapping 

The objective of this project was to construct two robots, mapping together an unknown environment as quickly as possible. The environments considered are exclusively composed of straight paths intersecting each other with right angles, where the paths are modelled by black lines on a white background. Furthermore, the robots are only allowed to move along those black lines during the exploration.

The principle of the developed solution follows a master-slave architecture. Each robot (slave) follows a black line while measuring the travelled distance. Once an intersection detected, the robot is stopped and sends an information message to the master. The information message contains the type of intersection encountered and the travelled distance since the previous intersection. This information is used by the master (a program running locally on the computer) to update the map if necessary, and compute the next instruction for the robot (stop, straight, turn left or right, uturn). 

<p align="center">
    <img width="400" src="gifs/gif-exploration.gif">&nbsp;&nbsp;&nbsp;
    <img width="400" src="gifs/gif-exploration.gif">
</p>

The left-part of the animation above shows a live screen recording of the constructed map in the master corresponding to the exploration in the right-part. A video of the complete demonstration can be found [here](https://drive.google.com/open?id=1ME8fCJpkcRnWXxbQvov_Zuj6nKWd1-8V). The report containing more details about the implementation and results can be found [here](report/personal-project.pdf).

## Simulator
As it would be costly in time to design and debug the mapping algorithm of the master if ran with the physical robots, a simulator was first implemented emulating random environments and robots. 

<p align="center">
    <img width="500" src="gifs/gif-simul.gif">
</p>

The algorithm of the master is explained in the [report](report/personal-project.pdf)) (pages 8-11). A simulation can be lauched by executing <code>python main.py</code> in the folder *code/simulator/*. Notice that the flag <code>is_simulation</code> needs to be set to true in the config file.

## Robot
The first challenge to make a robust robot was to enable it to follow a line at a constant speed. To this end, a controller for speed and steering was developed (described in the [report](report/personal-project.pdf), pages 20-24). The result on a curved line is shown below.

<p align="center">
    <img width="500" src="gifs/gif-control.gif">
</p>

From there, the robot was further designed to loop through a serie of steps during the exploration. This algorithm is implemented in the file *code/robot/mapping/mapping.ino*, and is based on low-level features located in the folder *code/robot/NewCOEX/* where COEX stands for cooperative explorer. 

<p align="center">
    <img width="500" src="gifs/gif-steps.gif">
</p>

## Usage
The physical exploration and mapping process can be executed following the desribed steps:
1. Copy the folder *code/robot/NewCoex* in your Arduino folder, and load as explained [here](https://www.arduino.cc/en/guide/libraries)
2. Upload the program *code/robot/mapping/mapping.ino* to the micro-controller
3. Verify in the config file that <code>is_simulation = False</code>, and the port names correspond to the names of yours bluetooth modules
4. Switch the robots on and calibrate the line sensors
5. Launch the exploration with <code>python main.py</code> in *code/master/*





