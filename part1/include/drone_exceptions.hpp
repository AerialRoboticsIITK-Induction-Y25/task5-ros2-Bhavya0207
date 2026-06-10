#pragma once
#include <exception>
#include <string>

class DroneException : public std::exception {
protected:
    std::string msg;

public:
    explicit DroneException(std::string m) : msg(std::move(m)) {}
    const char* what() const noexcept override {
        return msg.c_str();
    }
};

class BatteryDepletedError : public DroneException {
public:
    BatteryDepletedError()
        : DroneException("Battery is fully depleted") {}
};

class InvalidStateError : public DroneException {
public:
    InvalidStateError()
        : DroneException("Invalid drone state operation") {}
};

class AltitudeError : public DroneException {
public:
    AltitudeError()
        : DroneException("Altitude limit exceeded") {}
};