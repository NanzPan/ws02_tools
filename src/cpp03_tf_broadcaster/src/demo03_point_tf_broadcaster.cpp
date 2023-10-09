/*
需求：发布相对于laser坐标系的坐标点数据
假设障碍物在动
步骤：
1.包含头文件；
2.初始化 ROS2 客户端；
3.定义节点类；
3-1， 创建发布方
3-2， 创建定时器
3-3， 回调函数并发布信息
4.调用spin函数,并传入节点对象指针;
5.释放资源。
*/
// 1.包含头文件;
#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
// 3.定义节点类；
class PointBroadcaster : public rclcpp::Node {
   public:
    PointBroadcaster() : Node("point_broadcaster_node_cpp"), x(0.0) {
        // 3-1， 创建发布方
        pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point",
                                                                        10);
        // 3-2， 创建定时器
        timer_ = this->create_wall_timer(
            1s, std::bind(&PointBroadcaster::on_timer, this));
    }

   private:
    // 3-3， 回调函数并发布信息
    void on_timer() {
        // 组织消息
        geometry_msgs::msg::PointStamped ps;
        // 时间戳
        ps.header.stamp = this->now();
        // 现在我们要的是laser的坐标系
        ps.header.frame_id = "laser";
        // 假设障碍物每秒前进5cm
        x += 0.05;
        ps.point.x = x;
        ps.point.y = 0.0;
        ps.point.z = -0.1;
        // 发布消息
        pub_->publish(ps);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    // 通常 _t 是一种命名约定
    double_t x;
};
int main(int argc, char const *argv[]) {
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数,并传入节点对象指针;
    rclcpp::spin(std::make_shared<PointBroadcaster>());
    // 5.释放资源.
    rclcpp::shutdown();

    return 0;
}
