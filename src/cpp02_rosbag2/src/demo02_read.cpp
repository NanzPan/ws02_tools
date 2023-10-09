      /*
需求：bag文件数据，并经过数据输出到终端
步骤：
   1.包含头文件；
   2.初始化 ROS2 客户端；
   3.定义节点类；
        3-1， 创建回访对象
        3-2， 设置被读取的文件
        3-3， 读消息
        3-4， 关闭文件
   4.调用spin函数,并传入节点对象指针;
   5.释放资源。
*/
// 1.包含头文件;
#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/reader.hpp"
#include "geometry_msgs/msg/twist.hpp"

// 3.定义节点类；
class SimpleBagPlay : public rclcpp::Node {
public:
    SimpleBagPlay() : Node("simple_bag_play_node_cpp") {
        RCLCPP_INFO(this->get_logger(),"消息录制对象创建！");
        // 3-1， 创建回访对象
        reader_ = std::make_unique<rosbag2_cpp::Reader>();
        // 3-2， 设置被读取的文件
        reader_-> open("my_bag2");
        // 3-3， 读消息
        while (reader_->has_next()) {
            auto read_twist = reader_->read_next<geometry_msgs::msg::Twist>();
            RCLCPP_INFO(this->get_logger(), "线速度 %.2f ,角速度 %.2f ",read_twist.linear.x,read_twist.linear.y);

        }
        // 3-4， 关闭文件
        reader_->close();
 
}
private:
    std::unique_ptr<rosbag2_cpp::Reader> reader_;
};
int main(int argc, char const *argv[]) {
   // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
   // 4.调用spin函数,并传入节点对象指针;
   rclcpp::spin(std::make_shared<SimpleBagPlay>());
   // 5.释放资源.
   rclcpp::shutdown();
 
    return 0;
}
