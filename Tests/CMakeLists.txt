cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

get_filename_component(ROOT ${CMAKE_SOURCE_DIR} DIRECTORY)
include(${ROOT}/FilesList.cmake)

project(UmlToolTests)
