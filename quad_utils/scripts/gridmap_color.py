#!/usr/bin/env python

import rospy
import cv2
import numpy as np
from grid_map_msgs.msg import GridMap
from std_msgs.msg import ColorRGBA

class ModifyGridMapNode:
    def __init__(self):
        # Initialize the ROS node
        rospy.init_node('modify_gridmap_node')

        # Subscriber for the original GridMap
        self.grid_map_sub = rospy.Subscriber('/terrain_map', GridMap, self.grid_map_callback)

        # Publisher for the modified GridMap
        self.grid_map_pub = rospy.Publisher('/modified_grid_map', GridMap, queue_size=10)

        # Load your custom image
        self.image = cv2.imread('/home/lunarlab/Desktop/tarmac.png')
        if self.image is None:
            rospy.logerr("Failed to load image")
            return
        self.image = cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)

    def grid_map_callback(self, msg):
        # Get the grid map dimensions
        rows = len(msg.data[0].data)  # Number of rows in the GridMap
        cols = len(msg.data[0].data[0])  # Number of columns in the GridMap

        # Ensure the image is the same size as the GridMap
        resized_image = cv2.resize(self.image, (cols, rows))

        # Modify the GridMap's color data based on the image
        # Assuming the first layer is the one we are going to modify with color
        color_layer = msg.data[0]  # Modify this based on your actual layer

        for row in range(rows):
            for col in range(cols):
                # Normalize the color values (0-1)
                color_value = resized_image[row, col] / 255.0
                # Assuming msg.data[0].data contains the grid map data
                color_layer.data[row * cols + col] = ColorRGBA(
                    r=color_value[0],
                    g=color_value[1],
                    b=color_value[2],
                    a=1.0  # Fully opaque
                )

        # Publish the modified GridMap
        self.grid_map_pub.publish(msg)


if __name__ == '__main__':
    try:
        # Initialize and spin the node
        node = ModifyGridMapNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
