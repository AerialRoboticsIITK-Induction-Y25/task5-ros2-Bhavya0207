# ---------------- STAGE 1: BUILDER ----------------
FROM ros:jazzy AS builder

SHELL ["/bin/bash", "-c"]

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    python3-colcon-common-extensions \
    && rm -rf /var/lib/apt/lists/*

# Create workspace
WORKDIR /ws
COPY . /ws/src

# Source ROS and build
RUN source /opt/ros/jazzy/setup.bash && \
    colcon build

# ---------------- STAGE 2: RUNTIME ----------------
FROM ros:jazzy

SHELL ["/bin/bash", "-c"]

# Copy built workspace only
WORKDIR /ws
COPY --from=builder /ws/install /ws/install

# Source automatically on container start
RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc && \
    echo "source /ws/install/setup.bash" >> ~/.bashrc

