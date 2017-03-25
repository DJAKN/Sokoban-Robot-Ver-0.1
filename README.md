Sokoban Robot Ver 0.1
----
Sokoban Robot is a small project relevant to wheeled robot control. The main purpose of this project is to make the wheeled robot identify specialized things in a room and push them to expected positions (This process has some similarities with the game Sokoban so I give it the name). It's a primary version with the version number 0.1. 

### Operating environment

The project uses Linux as the operating environment.

### Wheeled robot platform

The robot platform we use is a four-wheel drive robot car with a series of sensors and encoders. There are two computers on it to run the control algorithm and process returned data. Researchers from Machine Vision Laboratory, Peking University, assembled the platform and provided it to us for the project. Data acquisition units include encoders which report travelled distance and other real-time parameters, a Huawei Xtion set which provides images and depth data, and a laser sensor which provides laser ranging data in 270 degrees.

### Code framework

We use RobotSDK as the development framework. RobotSDK is a robotics framework developed by students from Beihang University and Peking University for robotics development and application. To test this framework, you need to first install RobotSDK on your Linux OS. Please refer to README.md in `/Robot`.

Previous work has provided interfaces to the sensors. We use them directly in the control module. This module is located in `/Robot/Processor/Action/ProcessorMulti/Edit`. Besides, we also use APIs from OpenCV to support the object identification function.

This project also consists of a visualization module. It uses a series of position, speed and angle data to generate a traveling track, which helps to debug. It locates in '/Robot/Processor/Action/VisualizationMono/Edit'.

### Function realization

Sokoban Robot 0.1 aims to be employed in the following simple scenario: There's only one target object and it is in the robot's sight at the beginning. The robot needs to catch and approch it and push it to the scheduled position. To make the robot run like a Sokoban player, there are two main function modules: object identification and action planning.

#### Object identification

To make a basis for the action module, the robot needs to seperate the target object from background, and calculate a proper point as the target for action (we call it "point of impact").

We use a home-made green cylinder as the target object. Reasons for using it include that we want a regular geometry and cylinder is the most ideal one because it doesn't have lateral edges that will influence the pushing process and it won't roll like a ball, and green is a color that has high contrast in our laboratory, which reduces the difficulty for identifying.

When the robot returns an RGB image, we use the following principles to determine whether it catches the object: if the GREEN value is bigger than RED and BLUE value, and both RED and BLUE value is no more than 100, then it belongs to the box's color. To prevent the situation that the box is not in the sight but some other small green objects are caught, we only regard images that correct green pixels are more than 3% of all pixels as successfully catching the box.

After the robot catches the object, it calculates the geomatric average point of all correct green pixels as the point of impact. The following images show the result of object identification:

![]()

#### Action planning

Here we use an algorithm which aims to plan the action in feedback way. We segment the robot's sight to eight uniform blocks, and if the point of impact falls into one of them, computers on the robot will give the motor an angle and a speed for next move, which will make the point of impact more closed to the center of the robot's sight. Mapping of blocks of sight and action angles are:

![]()

All numbers are direct input numbers to the motor. The numbers are asymmetric because the motor has some system errors and many environmental factors will affect the actual running result. We test for a long time and these numbers can perform acceptable results. (Actually parameter estimation costs us lots of efforts even before we start the project, and please remember you need to change the parameters when you run this project on your own robot).

Angle estimation is on the tangential dimension, and on the normal dimesion, we need to calculate the real-time speed data. In `/Robot/Processor/Action/ProcessorMulti/Edit/ProcessorMulti_Processor_Action_Vars.h` we set a boolean variable `get_box` whose initial value is false to mark whether the robot has approached the object. When the distance of the robot and object is no more than 0.1 (relative value), set `get_box` as true and set `speed` and `steer` as 0. Then the purpose is to push the box to the destination. We keep `speed` even and set `steer` as:

-480 + orientation + atan((y - DESTINATION_Y) / (x - DESTINATION_X))

where -480 is a relative value for 90 degrees to the right. Then, when the distance between the robot and the destination point is no more the the radius of object, stop the robot.

### Future development

Sokoban Robot 0.1 is only a primary test version of the system and there are still many functions to add:

+ identify objects with less contrastive appearance and irregular shapes.
+ catch the object when it is not in the sight at the beginning.
+ push multiple objects to their respective destination points.
+ obstacle avoidance
