#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <deque>
#include <string>
#include <algorithm>

class HealthMonitor : public rclcpp::Node {
public:
    HealthMonitor() : Node("health_monitor") {

        sub_ = this->create_subscription<std_msgs::msg::String>(
            "/drone/Alpha/telemetry", 10,
            std::bind(&HealthMonitor::callback, this, std::placeholders::_1)
        );

        // Faster timer for visible output (IMPORTANT FOR DEMO)
        timer_ = this->create_wall_timer(
            std::chrono::seconds(3),
            std::bind(&HealthMonitor::report, this)
        );

        RCLCPP_INFO(this->get_logger(), "Health Monitor started");
    }

private:
    std::deque<float> battery_history_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
    rclcpp::TimerBase::SharedPtr timer_;

    // ---------------- BATTERY PARSER ----------------
    float extract_battery(const std::string &data) {
    std::string key = "\"battery\":";
    size_t start = data.find(key);

    if (start == std::string::npos)
        return -1;

    start += key.size();

    size_t end = data.find(",", start);
    if (end == std::string::npos)
        end = data.find("}", start);

    std::string val = data.substr(start, end - start);

    // remove spaces
    val.erase(std::remove_if(val.begin(), val.end(),
        [](char c){ return std::isspace(c); }),
        val.end());

    try {
        return std::stof(val);
    } catch (...) {
        return -1;
    }
}

    // ---------------- CALLBACK ----------------
    void callback(const std_msgs::msg::String::SharedPtr msg) {

        float battery = extract_battery(msg->data);

        // DEBUG (VERY IMPORTANT — shows ROS is working)
        RCLCPP_INFO(this->get_logger(),
            "RAW MSG: %s | battery=%.2f",
            msg->data.c_str(), battery);

        if (battery < 0) {
            RCLCPP_WARN(this->get_logger(), "Invalid battery parsed");
            return;
        }

        battery_history_.push_back(battery);

        if (battery_history_.size() > 10)
            battery_history_.pop_front();

        RCLCPP_INFO(this->get_logger(),
            "History size: %ld", battery_history_.size());
    }

    // ---------------- REPORT ----------------
    void report() {

        if (battery_history_.empty()) {
            RCLCPP_INFO(this->get_logger(), "Waiting for data...");
            return;
        }

        float old = battery_history_.front();
        float now = battery_history_.back();

        float rate = old - now;

        RCLCPP_INFO(this->get_logger(),
            "Battery: %.2f -> %.2f | Drain rate: %.2f",
            old, now, rate);

        if (rate > 1.5) {
            RCLCPP_WARN(this->get_logger(), "HIGH DRAIN WARNING!");
        }
    }
};

// ---------------- MAIN ----------------
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthMonitor>());
    rclcpp::shutdown();
    return 0;
}