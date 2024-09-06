#!/usr/bin/env python

import rospy
import cv2
import numpy as np
from grid_map_msgs.msg import GridMap
from grid_map_ros import GridMapRosConverter
from std_msgs.msg import ColorRGBA
from grid_map.core import GridMap as GM

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
        self.image = cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)

    def grid_map_callback(self, msg):
        # Convert ROS GridMap message to a GridMap object
        grid_map = GM()
        GridMapRosConverter.fromMessage(msg, grid_map)

        # Get the size and resolution of the GridMap
        resolution = grid_map.getResolution()
        size = grid_map.getSize()  # (rows, cols)
        rows, cols = size[0], size[1]

        # Ensure the image is the same size as the GridMap
        resized_image = cv2.resize(self.image, (cols, rows))

        # Modify the GridMap's "color" layer based on the image data
        for row in range(rows):
            for col in range(cols):
                color_value = resized_image[row, col] / 255.0  # Normalize the color values (0-1)
                grid_map.atPosition("color", (col * resolution, row * resolution)).r = color_value[0]
                grid_map.atPosition("color", (col * resolution, row * resolution)).g = color_value[1]
                grid_map.atPosition("color", (col * resolution, row * resolution)).b = color_value[2]
                grid_map.atPosition("color", (col * resolution, row * resolution)).a = 1.0  # Alpha set to 1 (opaque)

        # Convert back to ROS GridMap message and publish
        modified_grid_map_msg = GridMapRosConverter.toMessage(grid_map)
        self.grid_map_pub.publish(modified_grid_map_msg)


if __name__ == '__main__':
    try:
        # Initialize and spin the node
        node = ModifyGridMapNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
