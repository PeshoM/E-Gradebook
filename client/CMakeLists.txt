cmake_minimum_required(VERSION 3.10)
project(e_gradebook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

find_package(ODBC REQUIRED)
include_directories(${ODBC_INCLUDE_DIR})

set(NANODBC_DISABLE_LIBCXX ON CACHE BOOL "Disable libc++" FORCE)
set(NANODBC_DISABLE_EXAMPLES ON CACHE BOOL "Disable examples" FORCE)
set(NANODBC_DISABLE_TESTS ON CACHE BOOL "Disable tests" FORCE)

add_subdirectory(libs/nanodbc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/nanodbc/nanodbc)

add_executable(e_gradebook
    main.cpp
    components/Button.cpp
    components/BaseWindow.cpp
    views/MenuWindow.cpp
    database/Database.cpp
    views/students/AddStudentWindow.cpp
    views/students/ViewStudentsWindow.cpp
    views/subjects/AddSubjectWindow.cpp
    )

target_link_libraries(e_gradebook 
    sfml-graphics 
    sfml-window 
    sfml-system
    ${ODBC_LIBRARY}
    nanodbc) 