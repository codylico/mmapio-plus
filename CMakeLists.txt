
cmake_minimum_required(VERSION 3.0)

project(mmapio_plus CXX)

option(BUILD_TESTING "Enable testing.")
option(BUILD_SHARED_LIBS "Enable shared library construction.")
set(MMAPIO_PLUS_OS CACHE STRING "Target memory mapping API.")

add_library(mmapio_plus "mmapio.cpp" "mmapio.hpp")
target_compile_features(mmapio_plus PRIVATE
    cxx_override cxx_nullptr cxx_constexpr cxx_noexcept
  )
if (MMAPIO_PLUS_OS GREATER -1)
  target_compile_definitions(mmapio_plus
    PRIVATE "MMAPIO_PLUS_OS=${MMAPIO_PLUS_OS}")
endif (MMAPIO_PLUS_OS GREATER -1)
if (WIN32 AND BUILD_SHARED_LIBS)
  target_compile_definitions(mmapio_plus
    PUBLIC "MMAPIO_PLUS_WIN32_DLL")
endif(WIN32 AND BUILD_SHARED_LIBS)

if (BUILD_TESTING)
  add_executable(mmapio_plus_dump "tests/dump.cpp")
  target_link_libraries(mmapio_plus_dump mmapio_plus)

  add_executable(mmapio_plus_config "tests/config.cpp")
  target_link_libraries(mmapio_plus_config mmapio_plus)
endif (BUILD_TESTING)

