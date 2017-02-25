cmake_minimum_required (VERSION 3.1)

project (fractOCL)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
    include_directories( ${OPENCL_INCLUDE_PATH})
else()
    message("OPENCL not found!")
endif()

find_package(PNG REQUIRED)
if(PNG_FOUND)
    include_directories( ${PNG_INCLUDE_PATH})
else()
    message("PNG not found!")
endif()


find_package(Qt5 COMPONENTS Quick Widgets Core Network)
#find_package(Qt5QuickCompiler)

#qtquick_compiler_add_resources(RESOURCES example.qrc)

FILE(
  GLOB_RECURSE
  SRC_CPP
  src/*.cpp
  )


add_executable(fract.bin ${SRC_CPP})
#add_executable(fract.bin ${SRC_LIST} ${RESOURCES))

target_link_libraries(fract.bin
	${OpenCL_LIBRARY}
  ${PNG_LIBRARY}
)

qt5_use_modules(fract.bin Quick Widgets Core Network)

