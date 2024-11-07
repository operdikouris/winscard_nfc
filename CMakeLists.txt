cmake_minimum_required(VERSION 3.22)

#regarding versioning x.y.z is (x = major version no. The developments using previous versions may not be complatible with this)
#                               (y = minor version no. Indicates new features)
#                               (z = patch version no . Bug fixes)
project(Bus_Ticket 
        VERSION 1.0.0 
        LANGUAGES C)


#Overwrite some default already defined variables
set(CMAKE_C_STANDARD          99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        OFF)


set(SOURCES ${PROJECT_SOURCE_DIR}/acr122u.c)

# configure_file(configure.h.in ${PROJECT_SOURCE_DIR}/inc/configure.h @ONLY)


add_executable(crack_ticket ${SOURCES})

target_link_libraries(crack_ticket PRIVATE Winscard.lib)


# add_subdirectory(src)
