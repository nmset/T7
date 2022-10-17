cmake_minimum_required(VERSION 3.0)

project(T7) 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(wxWidgets
        CONFIG
        REQUIRED
        )

find_package(Hidapi REQUIRED)

add_executable(T7
            PedalManager.h          PedalManager.cpp
            KeyboardSimulation.h    KeyboardSimulation.cpp
            XT7Main.h               XT7Main.cpp
            Resources/UI/t7app.h    Resources/UI/t7app.cpp
            Resources/UI/t7main.h   Resources/UI/t7main.cpp
            )

install(TARGETS T7 RUNTIME DESTINATION bin) 

target_link_libraries(T7 ${wxWidgets_LIBRARIES}
                        ${HIDAPI_LIBRARIES}
                        )
