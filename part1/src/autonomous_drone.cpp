#include "autonomous_drone.hpp"
#include <sstream>
#include <chrono>

AutonomousDrone::AutonomousDrone(std::string name, float battery)
    : MissionDrone(std::move(name), battery),
      ai_mode("manual"),
      home_position({0,0,0}) {}

void AutonomousDrone::set_ai_mode(const std::string& mode) {
    ai_mode = mode;
}

void AutonomousDrone::detect_obstacle(std::tuple<float,float,float> pos,
                                       const std::string& severity) {
    obstacle_log.push_back(severity);

    if (severity == "high") {
        emergency_stop();
    }
}

std::vector<std::tuple<float,float,float>>
AutonomousDrone::auto_replan(const std::vector<std::tuple<float,float,float>>& obstacles) {
    return waypoints; // placeholder safe version
}

std::string AutonomousDrone::get_info() const {
    std::ostringstream oss;
    oss << "AutonomousDrone | Battery: " << get_battery()
        << " | AI Mode: " << ai_mode;
    return oss.str();
}