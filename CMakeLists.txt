project(PixelWars)
cmake_minimum_required(VERSION 2.8)
set(EXECUTABLE_OUTPUT_PATH build/)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ggdb ")


include_directories(src/ )
file(
	GLOB_RECURSE
	source_files
	src/*
	)
add_executable(
	meteor-falls
	${source_files}
	)

target_link_libraries(meteor-falls
	sfml-graphics
	sfml-audio
	sfml-network
	sfml-window
	sfml-system
	noise
	Box2D
	tinyxml
	)

