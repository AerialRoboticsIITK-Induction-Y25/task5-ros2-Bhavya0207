#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <unordered_map>
#include <string>
#include <iostream>
#include <mutex>
#include <algorithm>

struct DroneState {
    std::string battery;
    std::string altitude;
    std::string status;
    std::string waypoint;
};

class FleetManager : public rclcpp::Node {
public:
    FleetManager() : Node("fleet_manager") {

        sub_alpha_ = this->create_subscription<std_msgs::msg::String>(
            "/drone/Alpha/status", 10,
            std::bind(&FleetManager::callback, this, std::placeholders::_1));

        sub_beta_ = this->create_subscription<std_msgs::msg::String>(
            "/drone/Beta/status", 10,
            std::bind(&FleetManager::callback, this, std::placeholders::_1));

        sub_gamma_ = this->create_subscription<std_msgs::msg::String>(
            "/drone/Gamma/status", 10,
            std::bind(&FleetManager::callback, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(
            std::chrono::seconds(5),
            std::bind(&FleetManager::print_fleet, this));

        RCLCPP_INFO(this->get_logger(), "Fleet Manager started");
    }

private:
    std::unordered_map<std::string, DroneState> fleet_;
    std::mutex mtx_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_alpha_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_beta_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_gamma_;

    rclcpp::TimerBase::SharedPtr timer_;

    // ---------------- SAFE EXTRACT ----------------
    std::string extract(const std::string &msg, const std::string &key)
    {
        size_t pos = msg.find(key);
        if (pos == std::string::npos) return "N/A";

        pos += key.size();

        size_t end = msg.find('|', pos);
        std::string val = (end == std::string::npos)
            ? msg.substr(pos)
            : msg.substr(pos, end - pos);

        // clean whitespace + hidden chars
        val.erase(std::remove_if(val.begin(), val.end(),
            [](char c){ return std::isspace(c) || c == '\r' || c == '\n'; }),
            val.end());

        return val;
    }

    // ---------------- CALLBACK ----------------
    void callback(const std_msgs::msg::String::SharedPtr msg)
    {
        std::string data = msg->data;

        // DEBUG RAW MESSAGE
        RCLCPP_INFO(this->get_logger(), "RAW: %s", data.c_str());

        // remove "data:" prefix if present
        size_t p = data.find("data:");
        if (p != std::string::npos) {
            data = data.substr(p + 5);
        }

        std::string name = extract(data, "name:");

        // CLEAN NAME (IMPORTANT FIX)
        name.erase(std::remove_if(name.begin(), name.end(),
            [](char c){ return std::isspace(c) || c == '\r'; }),
            name.end());

        if (name.empty() || name == "N/A") {
            RCLCPP_WARN(this->get_logger(), "Invalid name parsed");
            return;
        }

        RCLCPP_INFO(this->get_logger(),
            "Parsed drone: [%s]", name.c_str());

        DroneState state;
        state.battery  = extract(data, "battery:");
        state.altitude = extract(data, "altitude:");
        state.status   = extract(data, "status:");
        state.waypoint = extract(data, "waypoint:");

        std::lock_guard<std::mutex> lock(mtx_);
        fleet_[name] = state;
    }

    // ---------------- PRINT ----------------
    void print_fleet()
    {
        std::lock_guard<std::mutex> lock(mtx_);

        std::cout << "\n================ FLEET STATUS ================\n";
        std::cout << "Drone\tBattery\tAltitude\tStatus\tWaypoint\n";
        std::cout << "------------------------------------------------\n";

        if (fleet_.empty()) {
            std::cout << "Waiting for drone data...\n";
            return;
        }

        for (auto &d : fleet_) {
            std::cout << d.first << "\t"
                      << d.second.battery << "\t"
                      << d.second.altitude << "\t\t"
                      << d.second.status << "\t"
                      << d.second.waypoint << "\n";
        }

        std::cout << "==============================================\n";
    }
};

// ---------------- MAIN ----------------
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FleetManager>());
    rclcpp::shutdown();
    return 0;
}