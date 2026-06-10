#include "vehicle.hpp"
#include "drone_exceptions.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>

Vehicle::Vehicle(std::string name, float battery)
    : name(std::move(name)), battery_level(battery), status("idle") {}

void Vehicle::log(const std::string& entry) {
    flight_log.push_back(entry);
}

float Vehicle::get_battery() const {
    return battery_level;
}

std::string Vehicle::get_status() const {
    return status;
}

std::string Vehicle::get_flight_log() const {
    std::ostringstream oss;
    for (const auto& l : flight_log)
        oss << l << "\n";
    return oss.str();
}

void Vehicle::set_status(const std::string& s) {
    status = s;
    log("Status changed to: " + s);
}

void Vehicle::drain_battery(float amount) {
    if (battery_level <= 0)
        throw BatteryDepletedError();

    battery_level -= amount;
    if (battery_level < 0)
        battery_level = 0;
}

void Vehicle::charge_battery(float amount, int) {
    if (status != "charging")
        throw InvalidStateError();

    battery_level += amount;
    if (battery_level > 100)
        battery_level = 100;
}

bool Vehicle::is_critical() const {
    return battery_level < 20;
}