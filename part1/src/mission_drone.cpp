#include "mission_drone.hpp"
#include <sstream>
#include <chrono>

MissionDrone::MissionDrone(std::string name, float battery)
    : Drone(std::move(name), battery), current_waypoint_index(0) {}

std::tuple<float,float,float> MissionDrone::next_waypoint() {
    if (current_waypoint_index >= (int)waypoints.size())
        return {0,0,0};

    auto wp = waypoints[current_waypoint_index++];
    drain_battery(1.5);

    visited.push_back({wp, "visited"});
    return wp;
}

void MissionDrone::skip_waypoint(const std::string& reason) {
    if (current_waypoint_index < (int)waypoints.size()) {
        visited.push_back({waypoints[current_waypoint_index], reason});
        current_waypoint_index++;
    }
}

bool MissionDrone::mission_complete() const {
    return current_waypoint_index >= (int)waypoints.size();
}

std::string MissionDrone::mission_summary() const {
    std::ostringstream oss;
    oss << "Mission: " << mission_name << "\n";
    oss << "Visited: " << visited.size() << " waypoints\n";
    return oss.str();
}

std::string MissionDrone::get_info() const {
    std::ostringstream oss;
    oss << "MissionDrone | Battery: " << get_battery()
        << " | Waypoint: " << current_waypoint_index;
    return oss.str();
}