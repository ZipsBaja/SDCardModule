# Set this to your module name!
set(MODULE_NAME ZipsSDCardModule)

add_library(${MODULE_NAME} STATIC
    src/SDCardModule.cpp
)

set(MAINLIB_DIR lib/zipslib)
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${MAINLIB_DIR})
    add_subdirectory(lib/zipslib)
endif()

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(lib/fatfs/FatFs_SPI)

target_link_libraries(${MODULE_NAME}
    ZipsPicoLib
    FatFs_SPI
)
