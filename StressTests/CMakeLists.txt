cmake_minimum_required(VERSION 3.19)
project(testPhysics)

add_executable(testPhysics
	testPhysics.cpp
)

if (MSVC)
    set_target_properties(testPhysics PROPERTIES LINK_FLAGS "/PROFILE")
endif()

target_include_directories(testPhysics PRIVATE
	include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine/deps
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine)
target_link_libraries(testPhysics
	EchoEngine
)

add_custom_command(TARGET testPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/deps/EchoEngine/deps/SDL2/${CMAKE_BUILD_TYPE}"
        $<TARGET_FILE_DIR:testPhysics>/../
)
add_custom_command(TARGET testPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/deps/EchoEngine/deps/SDL2_image/${CMAKE_BUILD_TYPE}"
        $<TARGET_FILE_DIR:testPhysics>/../
)
add_custom_command(TARGET testPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/deps/EchoEngine/deps/SDL2_ttf/${CMAKE_BUILD_TYPE}"
        $<TARGET_FILE_DIR:testPhysics>/../
)
add_custom_command(TARGET testPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:testPhysics>/../data
)