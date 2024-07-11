cmake_minimum_required(VERSION 3.16.0)

include(FetchContent)
FetchContent_Declare(
  espidflib
  GIT_REPOSITORY https://github.com/UncleRus/esp-idf-lib.git
)
FetchContent_MakeAvailable(espidflib)
list(APPEND EXTRA_COMPONENT_DIRS ${espidflib_SOURCE_DIR}/components/esp_idf_lib_helpers)
list(APPEND EXTRA_COMPONENT_DIRS ${espidflib_SOURCE_DIR}/components/i2cdev)
list(APPEND EXTRA_COMPONENT_DIRS ${espidflib_SOURCE_DIR}/components/sht3x)
# set(EXTRA_COMPONENT_DIRS ${espidflib_SOURCE_DIR}/components)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(air-temperature-humidity-espidf)
