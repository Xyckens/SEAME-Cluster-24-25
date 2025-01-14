rm -rf log/ install/ build/
colcon build --symlink-install
bass source install/setup.bash
