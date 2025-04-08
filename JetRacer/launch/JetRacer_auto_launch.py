from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node (
            package='i2c_interface',
            executable='i2c_interface_node',
            name='i2c_interface',
            respawn=True,
            respawn_delay=2.0,
            arguments=['--ros-args', '--log-level', 'fatal'],
        ),
        Node (
            package='can_interface',
            executable='can_interface_node',
            name='can_interface',
            respawn=True,
            respawn_delay=2.0,
            # arguments=['--ros-args', '--log-level', 'debug'],
        ),
        Node (
            package='speed_sensor',
            executable='speed_sensor_node',
            name='speed_sensor'
        ),
        Node (
            package='blinkers',
            executable='blinkers_node',
            name='blinkers'
        ),
        Node (
            package='oled_display',
            executable='oled_display_node',
            name='oled_display'
        ),
        Node (
            package='dc_motors',
            executable='dc_motors_node',
            name='dc_motors'
        ),
        Node (
            package='servo_motor',
            executable='servo_motor_node',
            name='servo_motor',
            # arguments=['--ros-args', '--log-level', 'debug'],
        ),
        Node (
            package='battery',
            executable='battery_node',
            name='battery'
        ),
        Node (
            package='display_routine',
            executable='display_routine_node',
            name='display_routine'
        ),
        Node (
            package='cluster',
            executable='cluster_node',
            name='cluster',
            respawn=True,
            respawn_delay=2.0
        ),
    ])
