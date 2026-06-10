#pragma once
#include <string>
#include <vector>

class Vehicle {
protected:
    std::string name;

private:
    float battery_level;
    std::string status;
    std::vector<std::string> flight_log;

protected:
    void log(const std::string& entry);

public:
    Vehicle(std::string name, float battery);

    virtual ~Vehicle() = default;

    virtual std::string get_info() const = 0;

    float get_battery() const;
    std::string get_status() const;

    std::string get_flight_log() const;

    void set_status(const std::string& s);

    void drain_battery(float amount);
    void charge_battery(float amount, int duration_seconds);

    bool is_critical() const;
};