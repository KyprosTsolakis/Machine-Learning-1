TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

HEADERS += \
    headers/dataFiles.h \
    headers/earth.h \
    headers/geometry.h \
    headers/position.h \
    headers/types.h \
    headers/waypoints.h \
    headers/analysis/analysis-route.h \
    headers/analysis/analysis-track.h \
    headers/gpx/gpx-parser.h \
    headers/gridworld/gridworld-model.h \
    headers/gridworld/gridworld-route.h \
    headers/gridworld/gridworld-track.h \
    headers/xml/xml-element.h \
    headers/xml/xml-generator.h \
    headers/xml/xml-parser.h

SOURCES += \
    src/dataFiles.cpp \
    src/earth.cpp \
    src/geometry.cpp \
    src/position.cpp \
    src/analysis/analysis-route.cpp \
    src/analysis/analysis-track.cpp \
    src/gpx/gpx-parser.cpp \
    src/gridworld/gridworld-model.cpp \
    src/gridworld/gridworld-route.cpp \
    src/gridworld/gridworld-track.cpp \
    src/xml/xml-element.cpp \
    src/xml/xml-generator.cpp \
    src/xml/xml-parser.cpp \
    tests/analysis/TestingTotalLength.cpp


SOURCES += \
    tests/BoostUTF-main.cpp \
    tests/geometry-tests.cpp \
    tests/xml/xml-parser-tests.cpp \
    tests/gpx/gpx-parseRoute-tests.cpp \
    tests/gpx/gpx-parseTrack-tests.cpp \
    tests/analysis/numpoints.cpp \
    tests/analysis/indexing.cpp \
    tests/analysis/totaltime.cpp

INCLUDEPATH += headers/ headers/analysis/ headers/gpx/ headers/gridworld/ headers/xml/

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = analysis-tests

LIBS += -lboost_unit_test_framework
