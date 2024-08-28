#!/usr/bin/env python 
import rospy
import numpy as np
from gazebo_msgs.msg import LinkStates
from tf.transformations import quaternion_matrix, euler_from_quaternion

# Helper function to convert quaternion to a rotation matrix
def quaternion_to_rotation_matrix(quat):
    rotation_matrix = quaternion_matrix([quat.x, quat.y, quat.z, quat.w])
    return rotation_matrix[:3, :3]  # Extract the 3x3 rotation matrix

# Function to transform vectors to the body frame
def transform_to_body_frame(rotation_matrix, vector):
    return np.dot(rotation_matrix.T, np.array([vector.x, vector.y, vector.z]))

# Callback to process link states
def link_states_callback(data):
    robot_body_link = "robot_1::body"  # Replace with the name of the robot's body link
    toe_links = ["robot_1::toe0", "robot_1::toe1", "robot_1::toe2", "robot_1::toe3"]
    
    try:
        # Get the index of the robot body link to obtain the body's rotation matrix
        body_index = data.name.index(robot_body_link)
        body_orientation = data.pose[body_index].orientation
        body_rotation_matrix = quaternion_to_rotation_matrix(body_orientation)
        
        # Process each toe link
        for toe_link in toe_links:
            toe_index = data.name.index(toe_link)
            
            # Get position and orientation of the toe in the world frame
            toe_position_world = data.pose[toe_index].position
            toe_orientation_world = data.pose[toe_index].orientation
            
            # Transform position to the body frame
            toe_position_body = transform_to_body_frame(body_rotation_matrix, toe_position_world)
            
            # Convert quaternion to Euler angles for easier interpretation
            toe_orientation_quat = [toe_orientation_world.x, toe_orientation_world.y, toe_orientation_world.z, toe_orientation_world.w]
            toe_orientation_euler = euler_from_quaternion(toe_orientation_quat)  # This will give roll, pitch, yaw
            
            # Get linear and angular velocities of the toe in the world frame
            toe_linear_velocity_world = data.twist[toe_index].linear
            toe_angular_velocity_world = data.twist[toe_index].angular
            
            # Transform velocities to the body frame
            toe_linear_velocity_body = transform_to_body_frame(body_rotation_matrix, toe_linear_velocity_world)
            toe_angular_velocity_body = transform_to_body_frame(body_rotation_matrix, toe_angular_velocity_world)
            
            # Log the position, orientation, and velocities in the body frame
            rospy.loginfo(f"{toe_link}:")
            rospy.loginfo(f"  Position in Body Frame: {toe_position_body}")
            rospy.loginfo(f"  Orientation (Roll, Pitch, Yaw) in Body Frame: {toe_orientation_euler}")
            rospy.loginfo(f"  Linear Velocity in Body Frame: {toe_linear_velocity_body}")
            rospy.loginfo(f"  Angular Velocity in Body Frame: {toe_angular_velocity_body}")

    except ValueError as e:
        rospy.logwarn(f"Link name not found: {e}")

if __name__ == "__main__":
    rospy.init_node('body_frame_velocity_listener')
    rospy.Subscriber('/gazebo/link_states', LinkStates, link_states_callback)
    rospy.spin()
