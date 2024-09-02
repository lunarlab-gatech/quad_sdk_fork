[![CircleCI](https://circleci.com/gh/robomechanics/quad-sdk/tree/main.svg?style=shield)](https://circleci.com/gh/robomechanics/quad-sdk/tree/main)
![Example image](doc/quad_sdk_promo.png)

## Quad-SDK Fork

This is a fork of the [Quad-SDK](https://github.com/robomechanics/quad-sdk) repository, used to generate simulated UniTree Go2 data for use with the [MI-HGNN](https://github.com/lunarlab-gatech/Morphology-Informed-HGNN) paper. For the main repository README.md or the documentation for how to use it, please see the original [Quad-SDK](https://github.com/robomechanics/quad-sdk) repository.

However, we made some alterations to the setup process in order to resolve issues or streamline the process. See the sections below for alterations to the install process, the list of changes, and the updated usage to replicate our dataset collection process:

## Install Alterations

## Add the quad_sdk_message_synchronizer repository to your ROS workspace

You'll need to install the [quad_sdk_message_synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer) library to your workspace, as it takes all of the messages and combines them into a time-synchronized message. You can do this by downloading the repository into the `src` folder, and then building the ros workspace as normal.

### Get HSL solver for IPOPT

On the "1. Getting Started with Quad SDK" page of the [Quad-SDK](https://github.com/robomechanics/quad-sdk) Wiki, it mentions "Get HSL solver for IPOPT". However, you can't install the latest version of the solver, as it has build errors (see Issue #[425](https://github.com/robomechanics/quad-sdk/issues/425)). As of the time of writing, it's unclear whether this is solved, but we worked around it by getting the 2022.11.09 version of the solver.

## Changelog
- Enabled the "ma57" version of the HSL Solver for faster solving.
- Slowed Simulation speed down to 1/5 of real-world speed in order to prevent dropped ROS messages.
- The 'A1' robot is used by default instead of the 'Spirit' robot.
- Update IMU ROS topic from '<robot>/trunk_imu' to '<robot>/state/imu'.
- Set initialOrientationAsReference=false for IMU.
- Fix bug causing contact sensor information not to be properly reported.
- Increased the IMU update rate maximum to 1000 hz (which ends up being 750 hz in practice).
- Wrote the [Quad-SDK Message Synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer) in order to synchronize the three messages that we need.
- Logger now logs the ROS messages output from the [Quad-SDK Message Synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer).
- Fixed the teleop_twist_joy package so Joysticks can be used to control the robot.
- Added Nintendo Switch Pro Controller Config file for Joystick input.

## Updated Usage
Make sure to source the ROS Noetic install, and download the [Quad-SDK Message Synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer) into the same ROS workspace `src` folder as this repository, and then rebuild and re-source the ros workspace.

Launch the simulation with:
```
roslaunch quad_utils quad_gazebo.launch
```

Stand the robot with:
```
rostopic pub /robot_1/control/mode std_msgs/UInt8 "data: 1"
```

### Controlling the robot

There are two options, either a user can control it directly, or use the global planner to control the robot for them.

To control the robot with keyboard input:
```
roslaunch quad_utils quad_plan.launch reference:=twist
rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/robot_1/cmd_vel
```

Or to control the robot with Joystick input:
```
roslaunch quad_utils quad_plan.launch reference:=twist
roslaunch teleop_twist_joy teleop.launch
```
To edit the speed that the joystick sends, see "external/teleop_twist_joy/config/pro_controller.config.yaml". Changing `scale_linear` and `scale_angular` changes the forward and turning speeds, respectively.

Or, to have the global planner drive the robot:
```
roslaunch quad_utils quad_plan.launch reference:=gbpl
```

### Logging the dataset values

To log the dataset, run the following commands
```
python <ros_ws_path>/src/quad_sdk_message_synchronizer/scripts/pos_vel_body_frame.py
roslaunch quad_sdk_message_synchronizer synch_log_messages.launch
roslaunch quad_utils logging.launch
```

The timestamped file will be saved in the `quad_logger/bags/archive/` folder.

The dataset values we are interested in (and that are logged) can be seen below:

| Data                     | Source                          | Frame | Units          | ROS Topic                   |
| ------------------------ | ------------------------------- | ----- | -------------- | --------------------------- |
| Joint Position           | quad_simulator/gazebo_scripts/src/estimator_plugin.cpp         | N/A                    | Radians          | /robot_1/state/ground_truth |
| Joint Velocity           | quad_simulator/gazebo_scripts/src/estimator_plugin.cpp         | N/A                    | Radians/sec      | /robot_1/state/ground_truth |
| Joint Feedback Torque    | quad_simulator/gazebo_scripts/src/estimator_plugin.cpp         | N/A                    | Newton-meters    | /robot_1/state/ground_truth |
| Linear Acceleration      | quad_simulator/go2_description/sdf_mesh/go2.sdf                | IMU Frame (Body Frame) | Meters/(sec^2)   | /robot_1/state/imu          |
| Angular Velocity         | quad_simulator/go2_description/sdf_mesh/go2.sdf                | IMU Frame (Body Frame) | Radians/sec      | /robot_1/state/imu          |
| Robot Position           | quad_simulator/gazebo_scripts/src/estimator_plugin.cpp         | World Frame            | Meters           | /robot_1/state/ground_truth |
| Robot Orientation        | quad_simulator/gazebo_scripts/src/estimator_plugin.cpp         | World Frame            | N/A (Quaternion) | /robot_1/state/ground_truth |
| Foot Position            | [quad_sdk_message_synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer)/scripts/pos_vel_body_frame.py | Body Frame | Meters     | /foot_states |
| Foot Velocity            | [quad_sdk_message_synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer)/scripts/pos_vel_body_frame.py | Body Frame | Meters/sec | /foot_states |
| Ground Reaction Forces   | quad_simulator/gazebo_scripts/src/contact_state_publisher.cpp  | World Frame            | Newtons          | /robot_1/state/grfs         |

#### Justification for Frames and Units

Gazebo uses ODE, and this link specifies that they use SI units (https://ode.org/wiki/index.php/FAQ#What_units_should_I_use_with_ODE.3F).

Joint Position:

- Specified that unit is radians for rotational axes (https://osrf-distributions.s3.amazonaws.com/gazebo/api/11.0.0/classgazebo_1_1physics_1_1Joint.html#a1e48139677005b6702c8c00832710da2).

Joint Velocity:

- Assuming it uses the same units as Joint Position, and the SI units from ODE, this gives us Radians/sec.

Joint Feedback Torque:

- GetForceTorque() method called (https://osrf-distributions.s3.amazonaws.com/gazebo/api/11.0.0/classgazebo_1_1physics_1_1Joint.html#a85f6b25f1d0d6451a84875c18c57535d). 

Linear Acceleration / Angular Velocity:

- We're using the GazeboRosImuSensor, which means it uses the ImuSensor built in with Gazebo (https://classic.gazebosim.org/tutorials?tut=ros_gzplugins).
- ImuSensor returns data in its local frame (https://osrf-distributions.s3.amazonaws.com/gazebo/api/11.0.0/classgazebo_1_1sensors_1_1ImuSensor.html#ab9bd72d686c13f991688b8775fefa238 , https://osrf-distributions.s3.amazonaws.com/gazebo/api/11.0.0/classgazebo_1_1sensors_1_1ImuSensor.html#a764d447b7ec876fdfe70d846510fd6a1)
- The IMU Sensor is attached to the "body" link with no pose offset, so the Imu Frame and Body Frame are synonymous.
- SI unit for angular velocity is radians/sec.

Robot Position / Robot Orientation:

- Calls a function called "WorldPose()".

Foot Position / Foot Velocity

- Gazebo publishes link information on '/gazebo/link_states' (https://classic.gazebosim.org/tutorials?tut=ros_comm&cat=connect_ros#GazeboPublishedTopics)
- This rostopic contains the following Link States message (https://docs.ros.org/en/jade/api/gazebo_msgs/html/msg/LinkStates.html), which has Pose and Twist in World Frame. However, in `pos_vel_body_frame.py`, we switch this to Body Frame.

Ground Reaction Forces:

- "quad_simulator/gazebo_scripts/src/contact_state_publisher.cpp" specified that they rotate the forces into the world frame.
-  Newtons is the official unit for SI (https://en.wikipedia.org/wiki/International_System_of_Units).
 
### Debugging

If you have any issues with the simulator, closing all terminal tabs and restarting has been found to resolve some problems. 

## Important Resources

These following websites have been useful for determining how Gazebo and Quad-SDK works:

### Quad-SDK Tutorials
- Creating Custom Terrain Files (https://github.com/robomechanics/quad-sdk/wiki/Tutorial:-Creating-Custom-Terrain-Map-Files)
- Adding a new type of Robot (https://github.com/robomechanics/quad-sdk/wiki/Tutorial:-Adding-a-New-Type-of-Robot-to-Quad-SDK)

### Gazebo Documentation
- Gazebo Classic Documentation - This repository uses version 11.14.0 (https://classic.gazebosim.org/tutorials)
- Setting Friction parameters in Gazebo (https://classic.gazebosim.org/tutorials?tut=friction&cat=physics)
- Gazebo Physics parameters (https://classic.gazebosim.org/tutorials?tut=physics_params&cat=physics)
- Gazebo ROS Plugin Tutorial - Also contains ROS Plugin for IMU (https://classic.gazebosim.org/tutorials?tut=ros_gzplugins)
- Sensor Noise models (https://classic.gazebosim.org/tutorials?tut=sensor_noise#Introduction)

### API (Version 11)
- Gazebo API Reference (https://osrf-distributions.s3.amazonaws.com/gazebo/api/11.0.0/index.html)

### API (Unknown Version)
- Gazebo API Reference - Great for understanding Gazebo Sensor implementations, functions, etc. (https://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/index.html)
- Contact Sensor Class Reference (https://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1sensors_1_1ContactSensor.html)
- Joint Class Reference (https://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1physics_1_1Joint.html#a85f6b25f1d0d6451a84875c18c57535d)
