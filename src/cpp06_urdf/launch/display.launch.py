from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类-----------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取--------------
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
# 文件包含相关----------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关-------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关-------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo
# 获取功能包下share目录路径-------------------
from ament_index_python.packages import get_package_share_directory

from launch_ros.parameter_descriptions import ParameterValue
# command 专门封装指令执行
from launch.substitutions import Command

def generate_launch_description():
    """ 
        加载urdf文件,并在rviz2中显示模型
        核心：
            1. 启动robot_state_publisher节点,该节点要以参数的形式加载urdf文件里的内容
            2. 启动rviz2节点
        优化：
            1. 添加 joint_state_publisher 节点（非固定关节必须包含）
            2. 设置 rviz2 的默认配置文件
            3. 动态传入urdf文件,把urdf文件封装为参数
     """
    #   1. 启动robot_state_publisher节点,该节点要以参数的形式加载urdf文件里的内容
    # 无优化
    # p_value = ParameterValue(Command(["xacro ",get_package_share_directory("cpp06_urdf") + "/urdf/urdf/demo01_helloworld.urdf"]))
    # 有第三优化动态传参，把urdf文件封装为参数，导包（参数声明与获取）
    # 改参命令 ros2 launch cpp06_urdf display.launch.py model(名称):=`ros2 pkg prefix --share cpp06_urdf` /urdf/urdf/xxxx.urdf
    # 在改参命令中，`ros2 pkg prefix --share cpp06_urdf`效果等价于 get_package_share_directory("cpp06_urdf")
    model = DeclareLaunchArgument(name="model",default_value=get_package_share_directory("cpp06_urdf") + "/urdf/urdf/pz_car.urdf")
    p_value = ParameterValue(Command(["xacro ",LaunchConfiguration("model")]))

    robot_state_pub = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description":p_value}]
    )
    # 试着添加 joint_state_publisher 节点（非固定关节必须包含）
    # 第一个优化
    joint_state_pub = Node(
        package="joint_state_publisher",
        executable = "joint_state_publisher"
    )
    #   2. 启动rviz2节点
    # 第二个优化
    rviz2 = Node(package="rviz2",
    executable="rviz2",
    arguments=["-d",get_package_share_directory("cpp06_urdf") + "/rviz/urdf.rviz"]
    )
    return LaunchDescription([model,joint_state_pub,robot_state_pub,rviz2])