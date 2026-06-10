#include "autonomous_drone.hpp"
#include <iostream>
#include <vector>

int main() {
    Drone d("Beta", 80);
    MissionDrone m("Gamma", 60);
    AutonomousDrone a("Alpha", 100);

    std::vector<Vehicle*> fleet = {&d, &m, &a};

    for (auto* v : fleet)
        std::cout << v->get_info() << "\n";

    try {
        d.drain_battery(200);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}