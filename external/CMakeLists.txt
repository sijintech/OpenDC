add_subdirectory(eigen ${CMAKE_CURRENT_BINARY_DIR}/../eigen)

# find_package(Eigen3 REQUIRED)

# get_target_property(target_includes Eigen3::Eigen INCLUDE_DIRECTORIES)
# message("Target Eigen3::Eigen includes:")
# foreach(include_dir ${target_includes})
#     message("  ${include_dir}")
# endforeach()

add_subdirectory(nanoflann ${CMAKE_CURRENT_BINARY_DIR}/../nanoflann)
# set(nanoflann_DIR ${CMAKE_CURRENT_BINARY_DIR}/../nanoflann)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

