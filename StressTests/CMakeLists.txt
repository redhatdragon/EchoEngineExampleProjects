cmake_minimum_required(VERSION 3.9)

project(testPhysicsAABB2D)

add_executable(testPhysicsAABB2D
	testPhysicsAABB2D.cpp
)
target_include_directories(testPhysicsAABB2D PRIVATE
	include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine/deps
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine)
target_link_libraries(testPhysicsAABB2D
	EchoEngine
)

add_custom_command(TARGET testPhysicsAABB2D POST_BUILD
    COMMAND echo Starting copy......
)

IF (WIN32)
	add_custom_command(TARGET testPhysicsAABB2D POST_BUILD
		COMMAND xcopy /s /d /y 
			"\"${CMAKE_CURRENT_SOURCE_DIR}/data\""
			"\"${CMAKE_BINARY_DIR}/data/*\""
	)
ELSE()
	add_custom_command(TARGET testPhysicsAABB2D POST_BUILD
		COMMAND cp -u 
			"\"${CMAKE_CURRENT_SOURCE_DIR}/data\""
			"\"${CMAKE_BINARY_DIR}/data/*\""
	)
ENDIF()

add_custom_command(TARGET testPhysicsAABB2D POST_BUILD
    COMMAND echo Finished copy......
)



project(testPhysicsChipmunk2d)

add_executable(testPhysicsChipmunk2d
	testPhysicsChipmunk2d.cpp
)
target_include_directories(testPhysicsChipmunk2d PRIVATE
	include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine/deps
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine)
target_link_libraries(testPhysicsChipmunk2d
	EchoEngine
)