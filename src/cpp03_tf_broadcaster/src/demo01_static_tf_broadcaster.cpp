      /*
需求：订阅发布方发布的消息，并输出到终端。
步骤：
   1.包含头文件；
   2.初始化 ROS2 客户端；
   3.定义节点类；
            
   4.调用spin函数,并传入节点对象指针;
   5.释放资源。
*/
// 1.包含头文件;
#include "rclcpp/rclcpp.hpp"

// 3.定义节点类；
class MyNode : public rclcpp::Node {
public:
    MyNode() : Node("mynode_node_cpp") {
 
 
}
};
int main(int argc, char const *argv[]) {
   // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
   // 4.调用spin函数,并传入节点对象指针;
   rclcpp::spin(std::make_shared<MyNode>());
   // 5.释放资源.
   rclcpp::shutdown();
 
    return 0;
}
 