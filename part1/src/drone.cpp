#include "drone.hpp"
#include <sstream>

Drone::Drone(std::string name, float battery)
    : Vehicle(std::move(name), battery),
      altitude(0),
      max_altitude(120),
      speed(0) {}

void Drone::take_off(float target_altitude) {
    if (target_altitude > max_altitude)
        throw AltitudeError();

    altitude = target_altitude;
    set_status("flying");
}

void Drone::land() {
    altitude = 0;
    set_status("idle");
}

void Drone::emergency_stop() {
    drain_battery(30);
    land();
}

std::string Drone::get_info() const {
    std::ostringstream oss;
    oss << "Drone Info | Battery: " << get_battery()
        << " | Status: " << get_status()
        << " | Altitude: " << altitude;
    return oss.str();
}