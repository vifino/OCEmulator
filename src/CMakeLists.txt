project(OCEmulator)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Lua 5.2 REQUIRED)
find_package(ICU REQUIRED)
find_package(SDL2 REQUIRED)

set(OCEmulator_Sources main.cpp compinstance.cpp component.cpp filesystemcomponent.cpp mainwindow.cpp screencomponent.cpp componentgpu.cpp)
set(OCEmulator_Headers mainwindow.h)
set(OCEmulator_Forms mainwindow.ui)
set(OCEmulator_Resources fonts.qrc)

#set(QT_USE_QTOPENGL true)

QT4_WRAP_CPP(OCEmulator_Headers_MOC ${OCEmulator_Headers})
QT4_WRAP_UI(OCEmulator_Forms_Headers ${OCEmulator_Forms})
QT4_ADD_RESOURCES(OCEmulator_Resources_RRC ${OCEmulator_Resources})

include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(OCEmulator ${OCEmulator_Sources} ${OCEmulator_Headers_MOC} ${OCEmulator_Forms_Headers} ${OCEmulator_Resources_RRC})
TARGET_LINK_LIBRARIES(OCEmulator ${QT_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${LUA_LIBRARIES} ${ICU_LIBRARIES} SDL2 SDL2_ttf)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
