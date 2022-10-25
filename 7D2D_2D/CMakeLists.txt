cmake_minimum_required(VERSION 3.19)
project(7D2D_2D)


add_executable(7D2D_2D
	main.cpp
)
target_include_directories(7D2D_2D PRIVATE
	include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine/deps
	#${CMAKE_CURRENT_SOURCE_DIR}/../EchoEngine
	#${CMAKE_CURRENT_SOURCE_DIR}/../EchoEngine/deps
	#${CMAKE_CURRENT_SOURCE_DIR}/../EchoEngine/deps/SDL2/include
)# check if these extra includes are necessary if this directory just had a full copy of EchoEngine

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../EchoEngine ${CMAKE_CURRENT_SOURCE_DIR}/EchoEngine)
target_link_libraries(7D2D_2D
	EchoEngine
)