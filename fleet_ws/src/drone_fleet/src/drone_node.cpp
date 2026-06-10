#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <string>
#include <sstream>

using namespace std::chrono_literals;

class DroneNode : public rclcpp::Node {
public:
    DroneNode() : Node("drone_node") {

        this->declare_parameter<std::string>("drone_name", "Alpha");
        this->declare_parameter<double>("initial_battery", 100.0);

        drone_name_ = this->get_parameter("drone_name").as_string();
        battery_ = this->get_parameter("initial_battery").as_double();

        publisher_ = this->create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name_ + "/status", 10);

        timer_ = this->create_wall_timer(
            2s,
            std::bind(&DroneNode::publish_status, this));

        RCLCPP_INFO(this->get_logger(),
            "Drone Node %s started", drone_name_.c_str());
    }

private:
    std::string drone_name_;
    double battery_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    int waypoint_ = 0;

    // ---------------- PUBLISH LOOP ----------------
    void publish_status()
    {
        // simulate battery drain
        battery_ -= 0.5;
        if (battery_ < 0) battery_ = 0;

        waypoint_++;

        std_msgs::msg::String msg;

        std::ostringstream ss;

        ss << "data: "
           << "name:" << drone_name_ << "|"
           << "battery:" << battery_ << "|"
           << "altitude:" << 20 + waypoint_ << "|"
           << "status:" << (battery_ < 20 ? "critical" : "flying") << "|"
           << "waypoint:" << waypoint_ << "/5";

        msg.data = ss.str();

        publisher_->publish(msg);

        RCLCPP_INFO(this->get_logger(),
            "Published: %s", msg.data.c_str());
    }
};

// ---------------- MAIN ----------------
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DroneNode>());
    rclcpp::shutdown();
    return 0;
}