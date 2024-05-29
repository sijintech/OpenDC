cmake_minimum_required(VERSION 3.20)


project(OPENCORR
        DESCRIPTION "test find necessary pacakge"
        VERSION 0.1.0
        LANGUAGES C CXX)


add_subdirectory(external)

# configure eigen
# find_package(Eigen3 REQUIRED)

# configure nanoflann
# find_package(nanoflann REQUIRED)

# configure Opencv
find_package(OpenCV REQUIRED)

# configure Open MP
find_package(OpenMP REQUIRED)

# Configure MKL fftw
set(MKL_INTERFACE "lp64")
set(MKL_LINK "static")
# find_package(MPI REQUIRED)
if(NOT TARGET MKL::MKL)
  find_package(MKL CONFIG REQUIRED)
endif()

string(REGEX MATCH "(20[0-9][0-9])" YEAR_MATCH "${MKL_VERSION}")
message("Year: ${YEAR_MATCH} from |${MKL_VERSION}|")

if(YEAR_MATCH)
    set(YEAR_NUMBER ${CMAKE_MATCH_1})
else()
    message(FATAL_ERROR "Failed to extract year from MKL_VERSION")
endif()

set(INTERFACE_LOCATION interfaces)
if(YEAR_NUMBER LESS 2023)
  message("MKL_VERSION year is less than 2023")
else()
  message("MKL_VERSION year is greater or equal to 2024")
  set(INTERFACE_LOCATION share/mkl/interfaces)
endif()
message("Interface location is at ${INTERFACE_LOCATION}")
message(STATUS "mkl_root: ${MKL_ROOT}")
#  /opt/intel/oneapi/mkl/2024.0
if(YEAR_NUMBER LESS 2024)
  file(GLOB FFT_INTERFACE
     "${MKL_ROOT}/interfaces/fftw3xc/wrappers/*.c"
  )
  # set(FFT_INTERFACE
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/init_clean.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/execute.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/local_size.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/wisdom.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/wrap-f03.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/plan.c
  #     ${MKL_ROOT}/interfaces/fftw3x_cdft/wrappers/mpi_transpose.c)
else()
  file(GLOB FFT_INTERFACE
     "{MKL_ROOT}/${INTERFACE_LOCATION}/fftw3xc/wrappers/*.c"
  )
  # set(FFT_INTERFACE
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/init_clean.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/execute.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/local_size.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/wisdom.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/wrap-f03.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/plan.c
  #     ${MKL_ROOT}/${INTERFACE_LOCATION}/fftw3x_cdft/wrappers/mpi_transpose.c)
endif()

set(INC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    # ${MPI_CXX_INCLUDE_PATH}
    # ${MPI_C_INCLUDE_PATH}
    $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>
    ${MKL_INCLUDE}/fftw)

set(LINK $<LINK_ONLY:MKL::MKL>)
# set(LINK MPI::MPI_CXX MPI::MPI_C MKL::MKL PUBLIC $<LINK_ONLY:${MKL_LIBRARIES} mkl_intel_lp64 mkl_core pthread m>)

add_subdirectory(examples)