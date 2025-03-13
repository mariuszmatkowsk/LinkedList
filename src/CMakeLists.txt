add_library(
    linkedlist
    INTERFACE
    LinkedList/LinkedList.hpp
)

target_include_directories(
    linkedlist  
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
    TARGETS linkedlist 
    EXPORT ${CMAKE_PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/LinkedList/cmake
)

install(
    FILES LinkedList/LinkedList.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LinkedList
)

