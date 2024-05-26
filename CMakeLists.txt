cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(CNH1x)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB include_headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES 
    "*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})


add_executable(CNH1 CNH1.cc ${sources} ${include_headers})
target_link_libraries(CNH1 ${Geant4_LIBRARIES})

add_custom_target(CNH1x DEPENDS CNH1)