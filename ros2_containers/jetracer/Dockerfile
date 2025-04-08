FROM arm64v8/ros:humble

SHELL ["/bin/bash", "-c"]

# Set ROS Domain ID
ENV ROS_DOMAIN_ID=4

# Install Qt6 and multimedia dependencies
RUN apt-get update && apt-get install -y \
    ros-humble-demo-nodes-cpp \
    ros-humble-foxglove-bridge \
    ros-humble-tf2-ros \
    ros-humble-joy \
    qt6-base-dev \
    qt6-multimedia-dev \
    qt6-tools-dev \
    libqt6waylandclient6 \
    libqt6waylandcompositor6 \
    qt6-wayland \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6multimedia6 \
    libqt6multimediawidgets6 \
    libopengl-dev \
    libglu1-mesa-dev \
    libglx-dev \
    libglvnd-dev \
    libgl1-mesa-dev \
    git \
    build-essential \
    cmake \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install latest CMake (3.27 at the time of writing)
WORKDIR /opt
RUN wget -O cmake.tar.gz https://github.com/Kitware/CMake/releases/download/v3.27.6/cmake-3.27.6-linux-aarch64.tar.gz \
    && tar -xzf cmake.tar.gz \
    && mv cmake-3.27.6-linux-aarch64 cmake-latest \
    && rm cmake.tar.gz

# Set CMake path
ENV PATH="/opt/cmake-latest/bin:$PATH"
ENV QT_QPA_PLATFORM=wayland
ENV QT_OPENGL=es2

# Set up workspace directory
WORKDIR /root/ws

# Clone the GitHub repository
RUN git clone --recurse-submodules https://github.com/Xyckens/SEAME-Cluster-24-25.git /root/ws

# Build the project
WORKDIR /root/ws/JetRacer
RUN source /opt/ros/humble/setup.bash && colcon build --symlink-install --packages-skip carla_bridge

CMD ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && \
                         source /root/ws/JetRacer/install/setup.bash && \
                         ros2 launch launch/${LAUNCH_FILE}"]
