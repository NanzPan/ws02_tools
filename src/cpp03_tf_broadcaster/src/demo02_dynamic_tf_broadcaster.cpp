/*
需求：启动 turtlesim_node 节点，编写程序发布乌龟相对于窗体（world）的位姿
步骤：
1.包含头文件；
2.初始化 ROS2 客户端；
3.定义节点类；
  3-1， 创建一个动态广播器
  3-2， 创建一个乌龟位姿订阅方
  3-3， 回调函数中获取乌龟位姿并生成相对关系并发布
4.调用spin函数,并传入节点对象指针;
5.释放资源。
*/
// 1.包含头文件;
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"
// 3.定义节点类；
class TFDynamicBroadcaster : public rclcpp::Node {
   public:
    TFDynamicBroadcaster() : Node("tf_dynamic_broadcaster_node_cpp") {
        // 3-1， 创建一个动态广播器
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        // 3-2， 创建一个乌龟位姿订阅方
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
            "turtle1/pose", 10,
            std::bind(&TFDynamicBroadcaster::do_pose, this,
                      std::placeholders::_1));
    }

   private:
    // 3-3， 回调函数中获取乌龟位姿并生成相对关系并发布
    void do_pose(const turtlesim::msg::Pose &pose) {
        // 组织消息
        geometry_msgs::msg::TransformStamped ts;
        // 时间戳
        ts.header.stamp = this->now();
        // 坐标系的id
        ts.header.frame_id = "world";
        // 子坐标系的id
        ts.child_frame_id = "turtle1";
        // 平移量和旋转度
        ts.transform.translation.x = pose.x;
        ts.transform.translation.y = pose.y;
        ts.transform.translation.z = 0.0;
        // 从欧拉角转换为四元数
        //  乌龟的欧拉角只有yaw的取值
        tf2::Quaternion qtn;
        // 设置四元数的旋转分量。
        qtn.setRPY(0, 0, pose.theta);

        ts.transform.rotation.x = qtn.x();
        ts.transform.rotation.y = qtn.y();
        ts.transform.rotation.z = qtn.z();
        ts.transform.rotation.w = qtn.w();

        // 发布
        broadcaster_->sendTransform(ts);
    }
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};
int main(int argc, char const *argv[]) {
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数,并传入节点对象指针;
    rclcpp::spin(std::make_shared<TFDynamicBroadcaster>());
    // 5.释放资源.
    rclcpp::shutdown();

    return 0;
}
