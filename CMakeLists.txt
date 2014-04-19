CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(XMLUtilities CXX)

INSTALL(DIRECTORY include/ DESTINATION include/)

message (STATUS "${PROJECT_NAME} will be installed to ${CMAKE_INSTALL_PREFIX} (give -DCMAKE_INSTALL_PREFIX=path to cmake to modify this)")
