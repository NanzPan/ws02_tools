/*
需求：先发布laser到base_link的坐标关系，再发布camera到base_link的坐标系相对关系，
        坐标系变换
求解laser到camera的坐标系关系
步骤：
1.包含头文件；
2.初始化 ROS2 客户端；
3.定义节点类；
  3-1. 创建一个缓冲对象（buffer），形成坐标树
  3-2. 创建监听器，绑定缓存对象把所有广播写入缓存
  3-3. 编写一个定时器实现转换
4.调用spin函数,并传入节点对象指针;
5.释放资源。
*/
// 1.包含头文件;
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace ::std::chrono_literals;
// 3.定义节点类；
class TFListener : public rclcpp::Node {
   public:
    TFListener() : Node("tf_listener_node_cpp") {
        // 3-1. 创建一个缓冲对象（buffer），形成坐标树
        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        // 3-2. 创建监听器，绑定缓存对象把所有广播写入缓存
        listener_ =
            std::make_shared<tf2_ros::TransformListener>(*buffer_, this);
        // 3-3. 编写一个定时器实现转换
        timer_ = this->create_wall_timer(
            1s, std::bind(&TFListener::on_timer_, this));
    }

   private:
    void on_timer_() {
        try {
            auto ts =   // 返回值，新父坐标帧，/新坐标帧的父坐标系，/新坐标帧的子坐标系，
                        // 转换的时间点，一般为：tf2::TimePointZero，最新时刻
                        //当转换失败时，用try，catch处理
                buffer_->lookupTransform("camera", "laser", tf2::TimePointZero);
            RCLCPP_INFO(
                this->get_logger(),
                "----------------  转换完成的坐标帧信息  ------------------");
            RCLCPP_INFO( 
                this->get_logger(),
                "新坐标帧：父坐标系：%s,子坐标系：%s,偏移量为（%.2f,%.2f,%.2f)",
                ts.header.frame_id.c_str(),  // camera
                ts.child_frame_id.c_str(),   // laser
                ts.transform.translation.x, ts.transform.translation.y,
                ts.transform.translation.z);
        }

        catch (const tf2::LookupException &e) {
            RCLCPP_ERROR(this->get_logger(), "异常提示:%s", e.what());
        }
    }
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc, char const *argv[]) {
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数,并传入节点对象指针;
    rclcpp::spin(std::make_shared<TFListener>());
    // 5.释放资源.
    rclcpp::shutdown();

    return 0;
}
