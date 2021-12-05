add_executable(canvas canvas.cpp)
target_link_libraries(canvas GL glfw m GLEW nuklear)

add_executable(extended extended.cpp)
target_link_libraries(extended GL glfw m GLEW nuklear)

add_executable(file_browser file_browser.cpp)
target_link_libraries(file_browser GL glfw m GLEW nuklear)

add_executable(skinning skinning.cpp)
target_link_libraries(skinning GL glfw m GLEW nuklear)