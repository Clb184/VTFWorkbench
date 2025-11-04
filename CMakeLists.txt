cmake_minimum_required(VERSION 3.5)
project(VTFThing LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

file(GLOB src src/*.cpp)
file(GLOB imgui_src deps/imgui/src/*.cpp)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glew/build/cmake)
add_executable(vtfthing ${src} ${imgui_src})
target_link_libraries(vtfthing
	PUBLIC glfw glew_s
)
set_target_properties(vtfthing
	PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC"
)

target_include_directories(vtfthing 
	PUBLIC
       		${imgui_src} 
		${CMAKE_HOME_DIRECTORY}/include
		${CMAKE_HOME_DIRECTORY}/deps/glew/include
		${CMAKE_HOME_DIRECTORY}/deps/imgui/include
)
