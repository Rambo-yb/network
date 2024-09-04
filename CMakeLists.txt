cmake_minimum_required(VERSION 2.8)
project(network)

add_definitions(-DUSE_EASYLOGGER)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/example MAIN_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/http SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list SRC)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/mongoose SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/easylogger SRC)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/http)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/mongoose)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/easylogger)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/curl/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/curl/lib)

#add_library(network ${SRC})
add_library(network SHARED ${SRC})

add_executable(network_test ${MAIN_SRC})
target_link_libraries(network_test network curl z pthread)
install(TARGETS network_test DESTINATION bin)
install(TARGETS network DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
