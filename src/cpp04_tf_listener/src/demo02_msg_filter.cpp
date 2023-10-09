/*
需求：广播laser->base_link的坐标关系，然后发布 point->laser 的坐标，
  求解point->base_link 的坐标
步骤：
1.包含头文件；
2.初始化 ROS2 客户端；
3.定义节点类；
3-1.创建坐标变换监听器
3-2.创建坐标点消息订阅方
3-3.创建过滤器，解析数据
4.调用spin函数,并传入节点对象指针;
5.释放资源。
*/
// 1.包含头文件;
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
// 关于订阅的
#include "tf2_ros/transform_listener.h"
// 关于buffer里面的定时器的(可能就在里面有用)
#include "tf2_ros/create_timer_ros.h"
// 创建订阅方坐标变换的专门的类
#include "geometry_msgs/msg/point_stamped.hpp"
#include "message_filters/subscriber.h"
// 过滤器的头文件
#include "tf2_ros/message_filter.h"
// 执行tf2里面的transform函数的包
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

using namespace ::std::chrono_literals;
// 3.定义节点类；
class TfPointListener : public rclcpp::Node {
   public:
    TfPointListener() : Node("tf_point_listener_node_cpp") {
        // 3-1.创建坐标变换监听器
        buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        // 定时器
        timer_ = std::make_shared<tf2_ros::CreateTimerROS>(
            this->get_node_base_interface(), this->get_node_timers_interface());
        // 创建定时器的接口
        buffer_->setCreateTimerInterface(timer_);
        // 创建监听器对象
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);
        // 3-2.创建坐标点消息订阅方
        point_sub.subscribe(this, "point");
        // 3-3.创建过滤器，解析数据
        // 参一订阅对象，餐二缓存，惨三目标坐标系，嘇四消息队列长度（不是qos），参五日志接口，参六时钟，参七超出时间（可忽略）
        filter_ = std::make_shared<
            tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(
            point_sub, *buffer_, "base_link", 10,
            this->get_node_logging_interface(),
            this->get_node_clock_interface(), 1s);
        // 解析数据
        filter_->registerCallback(&TfPointListener::transform_point, this);
    }

   private:
    // 参数就是订阅到的点的数据
    void transform_point(const geometry_msgs::msg::PointStamped &ps) {
        // 实现目标点的转换
        // 必须包含头文件tf2_geometry_msgs
        // 转换前要得到坐标点和相对坐标关系发布
        auto out = buffer_->transform(ps, "base_link");
        RCLCPP_INFO(this->get_logger(), "父坐标系：%s, 坐标：%.2f, %.2f, %.2f",
                    out.header.frame_id.c_str(), out.point.x, out.point.y,
                    out.point.z);
    };
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    std::shared_ptr<tf2_ros::CreateTimerROS> timer_;
    message_filters::Subscriber<geometry_msgs::msg::PointStamped> point_sub;
    std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>
        filter_;
};
int main(int argc, char const *argv[]) {
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数,并传入节点对象指针;
    rclcpp::spin(std::make_shared<TfPointListener>());
    // 5.释放资源.
    rclcpp::shutdown();

    return 0;
}
