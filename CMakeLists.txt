cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(CNH1x)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

include_directories("${PROJECT_SOURCE_DIR}/headers")

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/headers/*.hh)

add_executable(CNH1 CNH1.cc ${sources} ${headers})
target_link_libraries(CNH1 ${Geant4_LIBRARIES})

add_custom_target(CNH1x DEPENDS CNH1)