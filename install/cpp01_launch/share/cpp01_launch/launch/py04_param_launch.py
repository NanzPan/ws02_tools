from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类-----------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取--------------
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
# 文件包含相关----------------
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关-------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关-------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo
# 获取功能包下share目录路径-------------------
# from ament_index_python.packages import get_package_share_directory


""" 

    动态修改背景色
    实现：
        1.声明参数
        2.调用参数
        3.执行launch时动态导入参数

 """
def generate_launch_description():
        # 1.声明参数
        bg_r = DeclareLaunchArgument(name="backg_r",default_value="255")
        # 2.调用参数
        turtle = Node(package="turtlesim",executable="turtlesim_node",
        parameters=[{"background_r": LaunchConfiguration("backg_r")}])
        # 3.执行launch时动态导入参数
        return LaunchDescription([bg_r,turtle])