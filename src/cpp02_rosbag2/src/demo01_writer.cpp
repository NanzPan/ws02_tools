/*
需求：控制乌龟运动的速度指令
步骤：
1.包含头文件；
2.初始化 ROS2 客户端；
3.定义节点类；
    3-1, 创建录制对象
    3-2, 设置磁盘文件
    3-3, 写数据（创建速度订阅方，函数中执行写出操作）
4.调用spin函数,并传入节点对象指针;
5.释放资源。
*/
// 1.包含头文件;
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/writer.hpp"

// using namespace::std::chrono_literals;
using std::placeholders::_1;
// 3.定义节点类；
class SimpleBagRecord : public rclcpp::Node {
   public:
    SimpleBagRecord() : Node("simple_bag_record_node_cpp") {
        RCLCPP_INFO(this->get_logger(), "消息录制对象创建！");
        // 3-1, 创建录制对象
        writer_ = std::make_unique<rosbag2_cpp::Writer>();
        // 3-2, 设置磁盘文件
        // 导出数据位置（相对路径）
        writer_->open("my_bag2");
        // 3-3, 写数据（创建速度订阅方，函数中执行写出操作)
        // writer_ -> write(,)
        sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "turtle1/cmd_vel", 10,
            std::bind(&SimpleBagRecord::do_write_msg, this, _1));
    }

   private:
    void do_write_msg(std::shared_ptr<rclcpp::SerializedMessage> msg) {
        /*
        write(std::shared_ptr<rclcpp::SerializedMessage> message, //消息
        const std::string &topic_name,   // 话题名称
        const std::string &type_name,     //消息类型
        const rclcpp::Time &time)       //时间戳
         */
        RCLCPP_INFO(this->get_logger(), "数据写出....");
        writer_->write(msg, "turtle1/cmd_vel", "geometry_msgs/msg/Twist",
                       this->now());
    }
    // 使用Writer时使用指针
    std::unique_ptr<rosbag2_cpp::Writer> writer_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
};
int main(int argc, char const *argv[]) {
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数,并传入节点对象指针;
    rclcpp::spin(std::make_shared<SimpleBagRecord>());
    // 5.释放资源.
    rclcpp::shutdown();

    return 0;
}
