cmake_minimum_required(VERSION 2.6)
project(interpolate2)

option(ENABLE_SDL "Use SDL2 (Xlib will be used otherwise)" ON)
option(ENABLE_GLUT "Use GLUT (required to display text)" ON)

add_executable(interpolate2
	functional.cxx
	interpolate.cxx
	main.cxx
	surface.cxx
	text.cxx
	user.cxx
	visual.cxx
)

find_package(OpenGL REQUIRED)
target_include_directories(interpolate2 PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(interpolate2 PUBLIC ${OPENGL_LIBRARIES})

if(ENABLE_SDL)
	find_package(SDL2 REQUIRED)
	target_include_directories(interpolate2 PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(interpolate2 PUBLIC ${SDL2_LIBRARIES})
	target_compile_definitions(interpolate2 PRIVATE HAVE_SDL)
else()
	find_package(X11 REQUIRED)
	target_include_directories(interpolate2 PUBLIC ${X11_X11_INCLUDE_PATH})
	target_link_libraries(interpolate2 PUBLIC ${X11_X11_LIB})
	target_compile_definitions(interpolate2 PRIVATE HAVE_X11)
endif()

if(ENABLE_GLUT)
	find_package(GLUT REQUIRED)
	target_include_directories(interpolate2 PUBLIC ${GLUT_INCLUDE_DIR})
	target_link_libraries(interpolate2 PUBLIC ${GLUT_glut_LIBRARY})
	target_compile_definitions(interpolate2 PRIVATE HAVE_GLUT)
endif()

target_compile_options(interpolate2 PUBLIC
	-std=c++11
	-Wall -Wextra -pedantic
)
