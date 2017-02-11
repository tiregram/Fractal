cmake_minimum_required (VERSION 2.6)

project (fractOCL)

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


FILE(
  GLOB_RECURSE
  src_CPP
  src/*
  )



message( " lapin " ${OpenCL_LIBRARY} " lion ")
add_executable(fract.bin ${src_CPP})

target_link_libraries(fract.bin
	${OpenCL_LIBRARY}
  ${PNG_LIBRARY}
)

