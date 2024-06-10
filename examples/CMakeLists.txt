# cmake version requirement
cmake_minimum_required(VERSION 3.20)

# project name
project(opencoor_samples)

# configuration
set(CMAKE_CXX_STANDARD 14)

file(GLOB OPENCORR_CPP
     "../src/*.cpp"
)

set(SOURCES 
    ${OPENCORR_CPP}
    test_2d_dic_sift_icgn2.cpp
    ${FFT_INTERFACE}
)

add_executable(sift_dic ${SOURCES})

# configure Eigen
# set(EIGEN_DIR "/usr/include/eigen3/Eigen/")
# INCLUDE_DIRECTORIES (${EIGEN_DIR})

# configure OpenCV
# find_package( OpenCV REQUIRED )
# include_directories( ${OpenCV_INCLUDE_DIRS} )


# .h files
target_include_directories(sift_dic PUBLIC ../src)
target_include_directories(sift_dic PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(sift_dic PUBLIC ${INC})
target_link_libraries(sift_dic PUBLIC ${LINK})
# target_link_libraries(sift_dic PUBLIC MKL::MKL)
target_link_libraries(sift_dic PUBLIC ${OpenCV_LIBS})
# target_link_libraries(sift_dic fftw3)
# target_link_libraries(sift_dic fftw3f)
# target_link_libraries(sift_dic fftw3l)

target_link_libraries(sift_dic PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(sift_dic PUBLIC Eigen3::Eigen)
target_link_libraries(sift_dic PUBLIC nanoflann::nanoflann)
set_target_properties(
  sift_dic
  PROPERTIES C_STANDARD 99
             C_STANDARD_REQUIRED YES
             C_EXTENSIONS NO)
target_compile_options(
  sift_dic
  PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
