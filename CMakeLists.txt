cmake_minimum_required(VERSION 3.28)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3
        main.cpp
        Tests.cpp
        Student.cpp
        GUI.cpp
)

target_include_directories(lab3 PUBLIC
        Sequences
        PTRs
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Charts)

target_link_libraries(lab3 PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Charts)
