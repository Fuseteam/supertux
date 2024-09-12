# Taken from https://github.com/Microsoft/vcpkg/blob/2ad118a2532e99a78c41dffa1b9648e7eca20a95/ports/sdl2-ttf/CMakeLists.txt

cmake_minimum_required(VERSION 3.0)
project(SDL2_TTF C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/mk/cmake)

option(SDL2TTF_INSTALL "Install target for SDL2_ttf." ON)

IF(MIR_BUILD)
message("ITS MIR BUILD")
find_package(SDL2 REQUIRED)
set(SDL_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
set(SDL_LIBRARY ${SDL2_LIBRARIES})
find_path(SDL_INCLUDE_DIR SDL2/SDL.h)
message(${SDL_INCLUDE_DIR})
find_library(SDL_LIBRARY NAMES SDL2d SDL2)
else()
find_path(SDL_INCLUDE_DIR SDL2/SDL.h)
find_library(SDL_LIBRARY NAMES SDL2d SDL2)

endif()
find_package(Freetype REQUIRED)

add_library(SDL2_ttf SDL_ttf.c version.rc)

set_target_properties(SDL2_ttf PROPERTIES DEFINE_SYMBOL SDL2_EXPORTS)
IF(MIR_BUILD)
target_include_directories(SDL2_ttf PRIVATE ${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR_ft2build} ${PROJECT_SOURCE_DIR})
target_include_directories(SDL2_ttf PUBLIC ${SDL_INCLUDE_DIR})
else()
target_include_directories(SDL2_ttf PRIVATE ${SDL_INCLUDE_DIR}/SDL2 ${FREETYPE_INCLUDE_DIR_ft2build} ${PROJECT_SOURCE_DIR})
target_include_directories(SDL2_ttf PUBLIC $<INSTALL_INTERFACE:include/SDL2>)
endif()
if(WITH_RAQM)
  find_package(RAQM REQUIRED)
  add_definitions(-DHAVE_RAQM=1)
  target_include_directories(SDL2_ttf PRIVATE ${RAQM_INCLUDE_DIR})
  target_link_libraries(SDL2_ttf PUBLIC ${RAQM_LIBRARY})
endif()
target_link_libraries(SDL2_ttf PRIVATE ${SDL_LIBRARY} ${FREETYPE_LIBRARY})

if(SDL2TTF_INSTALL)
  include(GNUInstallDirs)
  install(TARGETS SDL2_ttf
    EXPORT sdl2_ttf-targets
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
  
  if(NOT DEFINED SDL_TTF_SKIP_HEADERS)
      install(FILES SDL_ttf.h DESTINATION include/SDL2)
  endif()
  
  # CMake integration
  configure_file(sdl2_ttf-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/sdl2_ttf-config.cmake" @ONLY)
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/sdl2_ttf-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SDL2_ttf/")
  export(TARGETS SDL2_ttf
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/sdl2_ttf-targets.cmake")
  export(PACKAGE SDL2_ttf)
  install(EXPORT sdl2_ttf-targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SDL2_ttf/")
endif()

message(STATUS "Link-time dependencies:")
message(STATUS "  " ${SDL_LIBRARY})
message(STATUS "  " ${FREETYPE_LIBRARY})
