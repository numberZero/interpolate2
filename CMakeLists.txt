cmake_minimum_required(VERSION 2.6)
project(interpolate2)

find_package(OpenGL)
find_package(SDL2)

add_executable(interpolate2
	functional.cxx
	interpolate.cxx
	main.cxx
	surface.cxx
)

target_include_directories(interpolate2 PUBLIC
	${OPENGL_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
)

target_link_libraries(interpolate2 PUBLIC
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARIES}
)

target_compile_options(interpolate2 PUBLIC
	-std=c++11
)
