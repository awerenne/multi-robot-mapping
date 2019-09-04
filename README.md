# Multi-robot environment mapping #

The objective is to construct two robots that cooperate to map an unknown environment as quickly as possible. The environments considered are exclusively composed of straight paths intersecting each other with right angles, where the paths are modelled by black lines on a white background. Furthermore, the robots are only allowed to move along those black lines. 

<p align="center">
    <img width="460" height="300" src="gifs/gif-exploration.gif">
</p>

The left-part of the animation above shows a robot exploring the environment, whereas the right-part is the live construction of the map using information gathered by this robot. A video of the complete demonstration can be found [here](https://drive.google.com/open?id=1ME8fCJpkcRnWXxbQvov_Zuj6nKWd1-8V). The [report](report/personal-project.pdf), on the other hand, contains more details about the implementation and results.

### Simulator ###
It would be costly in time to design and debug the mapping algorithm if ran with the physical robots. Thus, a simulator was first constructed emulating environments and robots. 

<p align="center">
    <img width="460" height="300" src="gifs/gif-simul.gif">
</p>

### Robot ###
In order to obtain a performant and robust robot, a controller for speed and steering was developed (see page 20-24 of [report](report/personal-project.pdf)).

<p align="center">
    <img width="460" height="300" src="gifs/gif-control.gif">
</p>

Finally, each robot was designed to follow a serie of different steps during the exploration/mapping:

<p align="center">
    <img width="460" height="300" src="gifs/gif-steps.gif">
</p>
