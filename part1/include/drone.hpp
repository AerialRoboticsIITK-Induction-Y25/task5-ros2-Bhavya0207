#pragma once
#include "vehicle.hpp"
#include "drone_exceptions.hpp"

class Drone : public Vehicle {
protected:
    float altitude;
    float max_altitude;

private:
    float speed;

public:
    Drone(std::string name, float battery);

    virtual void take_off(float target_altitude);
    virtual void land();
    virtual void emergency_stop();

    std::string get_info() const override;
};