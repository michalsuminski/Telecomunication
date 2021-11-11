cmake_minimum_required(VERSION 3.12)

project(telekom_zad2)

include_directories(include)

set(SOURCE_FILES
        src/checksum.cpp
		src/Getter.cpp
        src/Sender.cpp
		src/Configurator.cpp
        src/main.cpp)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main -static)
