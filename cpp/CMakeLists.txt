cmake_minimum_required(VERSION 3.16)
project("Faceswap_func")
set(CMAKE_CXX_STANDARD 17)

set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/src/faceswap_func")
set(INCLUDES "${PROJECT_SOURCE_DIR}/includes")

set(LIB_DIR "../../lib")
SET(LIBRARY_OUTPUT_PATH ${LIB_DIR}/cpp)
# pybind11
add_subdirectory(${INCLUDES}/pybind11)

# module
pybind11_add_module (
    faceswap_func
    ${MODULE_SOURCE}/homographie.cpp
    ${MODULE_SOURCE}/faceswap_func.cpp 
    ${MODULE_SOURCE}/wrapper.cpp
)

target_include_directories (
    faceswap_func
    PRIVATE ${MODULE_SOURCE}
)
