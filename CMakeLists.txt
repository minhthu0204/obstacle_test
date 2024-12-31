cmake_minimum_required(VERSION 3.16)

project(obstacle_avoidance LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Tìm kiếm thư viện Qt Core và WebSockets
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core WebSockets)
# Tìm kiếm thư viện OpenCV
find_package(OpenCV REQUIRED)

# Tìm kiếm thư viện DepthAI (Cập nhật đường dẫn cài đặt nếu cần thiết)
find_package(depthai CONFIG REQUIRED PATHS /home/xb/depthai-core/build/install/lib/cmake/depthai)

add_executable(obstacle_avoidance
  main.cpp
  ObstacleAvoidance.h ObstacleAvoidance.cpp
  MovingLogic.h MovingLogic.cpp
  PipelineManager.h PipelineManager.cpp

)
# Liên kết các thư viện cần thiết
target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets  # Thêm Qt WebSockets
  PRIVATE ${OpenCV_LIBS}
  PRIVATE depthai::opencv
)

include(GNUInstallDirs)
install(TARGETS obstacle_avoidance
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
