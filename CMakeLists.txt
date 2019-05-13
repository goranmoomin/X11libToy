find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
add_library(Toy STATIC box.cpp canvas.cpp clip.cpp graph.cpp viewport.cpp color.cpp)
target_link_libraries(Toy ${X11_LIBRARIES})
