cmake_minimum_required(VERSION 2.8)

project(master-pc)

find_package(Boost COMPONENTS program_options REQUIRED system)
find_package(Threads)

set(EXE_SOURCES
    main.cpp
    serial-port.cpp
    ssp-master.cpp
    signal-handler.cpp
    main-loop.cpp
	../smart-sensor/lasertag-ssp/ssp/src/ssp-master-part.c
	../smart-sensor/lasertag-ssp/ssp/src/ssp.c
)

include_directories(
    ../smart-sensor/lasertag-ssp/ssp/Inc
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries (${PROJECT_NAME}
    ${Boost_LIBRARIES} # For command line options parsing for example
    ${CMAKE_THREAD_LIBS_INIT}
)

