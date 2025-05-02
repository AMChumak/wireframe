cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME wireframe)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/home/anton/Qt/6.8.1/gcc_64/lib/cmake")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui)

add_subdirectory(widgets)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
        WIN32 MACOSX_BUNDLE
        main.cpp
)


target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ui_lib
        PUBLIC
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})