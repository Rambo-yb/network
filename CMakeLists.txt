cmake_minimum_required(VERSION 2.8)
project(network)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/example SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/http_server SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list SRC)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/mongoose SRC)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/http_server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/mongoose)

add_executable(network ${SRC})
target_link_libraries(network pthread)