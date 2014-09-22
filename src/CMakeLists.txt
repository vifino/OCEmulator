project(OCEmulator)

find_package(Qt4 REQUIRED)

set(OCEmulator_Sources main.cpp compinstance.cpp component.cpp emuscreen.cpp filesystemcomponent.cpp mainwindow.cpp screencomponent.cpp)
set(OCEmulator_Headers compinstance.h component.h emuscreen.h filesystemcomponent.h mainwindow.h screencomponent.h)
set(QT_USE_QTOPENGL true)

QT4_WRAP_CPP(OCEmulator_Headers_MOC ${OCEmulator_Headers})

include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(OCEmulator ${OCEmulator_Sources} ${OCEmulator_Headers_MOC})
TARGET_LINK_LIBRARIES(OCEmulator ${QT_LIBRARIES})
