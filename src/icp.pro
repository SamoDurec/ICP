QT       += core gui widgets
CONFIG   += c++17
TARGET    = ../icp-pn
TEMPLATE  = app
SOURCES  += main.cpp place.cpp transition.cpp petrinet.cpp parser.cpp
HEADERS  += place.hpp transition.hpp petrinet.hpp parser.hpp