cmake_minimum_required(VERSION 3.8)
project(cpp02_rosbag2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosbag2_cpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(demo01_writer.cpp src/demo01_writer.cpp)
add_executable(demo02_read.cpp src/demo02_read.cpp)
target_include_directories(demo01_writer.cpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(demo01_writer.cpp PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  demo01_writer.cpp
  "rclcpp"
  "rosbag2_cpp"
  "turtlesim"
  "geometry_msgs"
)
ament_target_dependencies(
  demo02_read.cpp
  "rclcpp"
  "rosbag2_cpp"
  "turtlesim"
  "geometry_msgs"
)

install(TARGETS 
demo01_writer.cpp
demo02_read.cpp
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
