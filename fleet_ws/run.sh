#!/bin/bash

set -e

docker build -t drone_fleet_ws .

docker run -it --rm \
    --net=host \
    -e ROS_DOMAIN_ID=42 \
    drone_fleet_ws \
    bash -c "source /ws/install/setup.bash && ros2 launch drone_fleet fleet.launch.py"