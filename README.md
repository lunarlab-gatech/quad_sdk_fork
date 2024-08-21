[![CircleCI](https://circleci.com/gh/robomechanics/quad-sdk/tree/main.svg?style=shield)](https://circleci.com/gh/robomechanics/quad-sdk/tree/main)
![Example image](doc/quad_sdk_promo.png)

## Quad-SDK Fork

This is a fork of the [Quad-SDK](https://github.com/robomechanics/quad-sdk) repository, used to generate simulated UniTree Go2 data for use with the [MI-HGNN](https://github.com/lunarlab-gatech/Morphology-Informed-HGNN) paper. For the main repository README.md or the documentation for how to use it, please see the original [Quad-SDK](https://github.com/robomechanics/quad-sdk) repository.

However, we made some alterations to the setup process in order to resolve issues or streamline the process. See the sections below for alterations to the install process, and for the list of changes, and the updated uses to replicate our dataset collection process:

## Install Alterations

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
Make sure to source the ROS Noetic install, and download the [Quad-SDK Message Synchronizer](https://github.com/lunarlab-gatech/quad_sdk_message_synchronizer) into the same ROS workspace `src` folder as this repository.

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
roslaunch quad_utils quad_plan.launch reference:=twist logging:=true
rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/robot_1/cmd_vel
```

Or alternatively, launch the stack with the Joystick input:
```
roslaunch quad_utils quad_plan.launch reference:=twist logging:=true

```

Run the stack with global planner:
```
roslaunch quad_utils quad_plan.launch reference:=gbpl logging:=true
```

### Logging the dataset values


The dataset values we are interested in (and that are logged) can be seen below:

