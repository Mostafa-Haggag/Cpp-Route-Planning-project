cmake_minimum_required(VERSION 3.11.3)

# Set the C++ standard we will use which is c++ 17
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER  "/usr/bin/g++-7")
set(CMAKE_CXX_FLAGS "-std=c++1z")
# Add the path of the cmake files to the CMAKE_MODULE_PATH. we are setting the path to
#CppND-Route-Planning-Project/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
#This is the name of the project
project(OSM_A_star_search)
#this is the directory that will have all of the libararies
# Set library output path to /lib
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

# Locate project prerequisites they are required to be installed first
find_package(io2d REQUIRED)
find_package(Cairo)
find_package(GraphicsMagick)

# Set IO2D flags they will help in the compiling process
set(IO2D_WITHOUT_SAMPLES 1)
set(IO2D_WITHOUT_TESTS 1)

# Add the pugixml and GoogleTest library subdirectories. They will be used in our project
add_subdirectory(thirdparty/pugixml)
add_subdirectory(thirdparty/googletest)

# Add project executable kda enta 3andak el exacutable esmoh OSM a star w dh el ha runoh
add_executable(OSM_A_star_search src/main.cpp src/model.cpp src/render.cpp src/route_model.cpp src/route_planner.cpp)
# the libaries to be linked to the above extuable
target_link_libraries(OSM_A_star_search
    PRIVATE io2d::io2d
    PUBLIC pugixml
)

# Add the testing executable for testing. we have specfic libaries to be linked to this executable
add_executable(test test/utest_rp_a_star_search.cpp src/route_planner.cpp src/model.cpp src/route_model.cpp)

target_link_libraries(test 
    gtest_main 
    pugixml
)

# Set options for Linux or Microsoft Visual C++. we check on the system and accordingly choose a specific setting.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    target_link_libraries(OSM_A_star_search PUBLIC pthread)
endif()

if(MSVC)
	target_compile_options(OSM_A_star_search PUBLIC /D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING /wd4459)
endif()
