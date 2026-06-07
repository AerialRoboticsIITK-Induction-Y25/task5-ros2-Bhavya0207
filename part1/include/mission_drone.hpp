#pragma once
#include "drone.hpp"
#include <tuple>
#include <vector>
#include <string>

class MissionDrone : public Drone {
protected:
    std::string mission_name;
    std::vector<std::tuple<float,float,float>> waypoints;
    int current_waypoint_index;

private:
    std::vector<std::pair<std::tuple<float,float,float>, std::string>> visited;

public:
    MissionDrone(std::string name, float battery);

    std::tuple<float,float,float> next_waypoint();
    void skip_waypoint(const std::string& reason);
    bool mission_complete() const;
    std::string mission_summary() const;

    std::string get_info() const override;
};