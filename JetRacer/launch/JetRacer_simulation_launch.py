from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node (
            package='joy',
            executable='joy_node',
            name='joy',
             parameters=[{
                'device_id': 0,                # Set the device ID for your joystick
            }]
        ),
        Node (
            package='teleop',
            executable='teleop_node',
            name='teleop'
        ),
    ])
