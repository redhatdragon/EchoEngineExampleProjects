cmake_minimum_required(VERSION 3.19)
project(app)

add_executable(app
	main.cpp
)

if (MSVC)
    set_target_properties(app PROPERTIES LINK_FLAGS "/PROFILE")
endif()

target_include_directories(app PRIVATE
	include
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine
	${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine/deps
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/EchoEngine)
target_link_libraries(app
	EchoEngine
)

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/deps/EchoEngine/${CMAKE_BUILD_TYPE}"
        $<TARGET_FILE_DIR:app>/../
)
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:app>/../data
)