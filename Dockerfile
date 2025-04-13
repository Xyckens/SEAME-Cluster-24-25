# Use an official Ubuntu base image
FROM ubuntu:24.04

# Set environment variables to avoid prompts during installation
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=jazzy

# Install dependencies
RUN apt-get update && apt-get install -y \
    clang \
    build-essential \
    cmake \
    git \
    qt6-base-dev \
    qt6-tools-dev-tools \
    qt6-multimedia-dev \
    software-properties-common \
    curl \
    gnupg2 \
    lsb-release \
    python3-pip \
    wget \
    unzip \
    && apt-get clean

# Add the ROS 2 repository for colcon
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key | apt-key add - && \
    sh -c 'echo "deb [arch=$(dpkg --print-architecture)] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" > /etc/apt/sources.list.d/ros2-latest.list' && \
    apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    ros-jazzy-joy \
    ros-jazzy-desktop \
    && apt-get clean

# Install rosdep
RUN apt-get update && apt-get install -y \
    python3-rosdep \
    python3-numpy \
    python3-scipy \
    portaudio19-dev \
    && pip3 install  --break-system-packages vosk sounddevice \
    && wget https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip \
    && unzip vosk-model-small-en-us-0.15.zip \
    && rosdep init \
    && rosdep update

# Initialize rosdep
RUN rosdep init || true && rosdep update

# Set up environment variables for ROS2
SHELL ["/bin/bash", "-c"]
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> ~/.bashrc

# Create workspace and set working directory
WORKDIR /workspace


# Default command
CMD ["bash", "-c", "source /opt/ros/$ROS_DISTRO/setup.bash \
&& cd /workspace \
&& rosdep install --from-paths . --ignore-src -r -y \
&& colcon build --packages-skip camera_ros carla_bridge --symlink-install \
&& source install/setup.bash && ros2 launch $LAUNCH_FILE"]
