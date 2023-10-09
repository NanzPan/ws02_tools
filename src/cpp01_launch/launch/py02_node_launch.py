from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类-----------
from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable
# 参数声明与获取--------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关----------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonIncludeLaunchDescriptionSource
# 分组相关-------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关-------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo
# 获取功能包下share目录路径-------------------
from ament_index_python.packages import get_package_share_directory
import os

""" 

    演示 Node 的使用.
        :param: package 包名；
        :param: executable 可执行程序；
        :param: name 节点名称；
        :param: namespace 命名空间；
        :param: exec_name 设置程序标签；
        :param: parameters 设置参数；
        :param: remappings 话题重映射；参一是原名，餐二是后名
        :param: ros_arguments 为节点传参
                这两者的区别就是--ros-args的位置
                前者在前，后者在后
        :param: arguments 为节点传参
        :param: respawn (bool)是否自动重启

 """

def generate_launch_description():
    t1 = Node(package="turtlesim",executable="turtlesim_node",
    exec_name="my_label",
    ros_arguments=["--remap","__ns:=/t2"])
    #ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t2

    t2 = Node(package="turtlesim",executable="turtlesim_node",
    name = "a666",
    # 第一种改参方式：直接设置
    # parameters= [{"background_r": 255,"background_b": 0,"background_g": 0}]
    # 第二种（更常用）：读取yaml文件,(动态获取)，在注释里
    parameters = [os.path.join(get_package_share_directory("cpp01_launch"),"config","haha.yaml")],
    # 静态
    # parameters = ["/home/pz10on/Work/ws02_tools/install/cpp01_launch/share/cpp01_launch/config/haha.yaml"]
    # respawn = True
    )
    # 可执行程序；
    cmd = ExecuteProcess(
        cmd = ["ros2 topic echo /turtle/pose"],
        output = "both",
        shell = True
    )
    
    return LaunchDescription([t2,cmd])