#!/usr/bin/env python

import rospy
import cv2
import numpy as np
from grid_map_msgs.msg import GridMap
from std_msgs.msg import Float32MultiArray

class ModifyGridMapNode:
    def __init__(self):
        # Initialize the ROS node
        rospy.init_node('modify_gridmap_node')

        # Ensure image_loaded is initialized before anything else
        self.image_loaded = False  # Initialize the image_loaded flag

        # Load your custom image
        self.image = cv2.imread('/home/lunarlab/Desktop/tarmac.png')
        if self.image is None:
            rospy.logerr("Failed to load image. Callback will not be executed.")
        else:
            self.image = cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)
            self.image_loaded = True  # Set a flag indicating the image was successfully loaded

        # Subscriber for the original GridMap
        self.grid_map_sub = rospy.Subscriber('/terrain_map', GridMap, self.grid_map_callback)

        # Publisher for the modified GridMap
        self.grid_map_pub = rospy.Publisher('/modified_grid_map', GridMap, queue_size=10)

    def grid_map_callback(self, msg):
        # Check if the image was loaded successfully
        if not self.image_loaded:
            rospy.logwarn("Image not loaded, skipping grid map modification.")
            return

        # Get the grid map resolution and size in meters
        resolution = msg.info.resolution
        length_x = msg.info.length_x
        length_y = msg.info.length_y

        # Calculate the number of rows and columns (grid cells)
        rows = int(length_y / resolution)  # Number of rows (y-direction)
        cols = int(length_x / resolution)  # Number of columns (x-direction)

        # Ensure the image is the same size as the GridMap (resized based on grid size)
        resized_image = cv2.resize(self.image, (cols, rows))

        # Modify the first layer of the GridMap's data based on the image
        if len(msg.layers) > 0 and len(msg.data) > 0:
            layer_index = 0  # Modify the first layer (adjust if necessary)
            layer_data = list(msg.data[layer_index].data)  # Convert the data to a mutable list

            for row in range(rows):
                for col in range(cols):
                    # Normalize the color values (0-1)
                    color_value = resized_image[row, col] / 255.0
                    # Calculate the flat index for the data array (row-major order)
                    index = row * cols + col
                    # Modify the red channel (or any other channel as needed)
                    layer_data[index] = color_value[0]  # Modify with red channel

            # Convert the list back to a tuple or set it back as Float32MultiArray
            msg.data[layer_index].data = layer_data  # Assign the updated data back
            self.grid_map_pub.publish(msg)
        else:
            rospy.logwarn("No layers or data found in the GridMap message")

if __name__ == '__main__':
    try:
        # Initialize and spin the node
        node = ModifyGridMapNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
