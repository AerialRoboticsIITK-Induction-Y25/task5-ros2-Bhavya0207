#pragma once
#include "mission_drone.hpp"

class AutonomousDrone : public MissionDrone {
private:
    std::string ai_mode;
    std::tuple<float,float,float> home_position;
    std::vector<std::string> obstacle_log;

public:
    AutonomousDrone(std::string name, float battery);

    void set_ai_mode(const std::string& mode);

    void detect_obstacle(std::tuple<float,float,float> pos,
                          const std::string& severity);

    std::vector<std::tuple<float,float,float>>
    auto_replan(const std::vector<std::tuple<float,float,float>>& obstacles);

    std::string get_info() const override;
};