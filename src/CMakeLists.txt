project(libnss_wrapper C)

include_directories(${CMAKE_BINARY_DIR})
add_library(nss_wrapper SHARED nss_wrapper.c)
target_compile_options(nss_wrapper
                       PRIVATE
                          ${DEFAULT_C_COMPILE_FLAGS})
if (BSD)
    target_compile_options(nss_wrapper
                           PRIVATE
                           -DBSD)
endif()
target_link_libraries(nss_wrapper ${NWRAP_REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(
  nss_wrapper
    PROPERTIES
      VERSION
        ${LIBRARY_VERSION}
      SOVERSION
        ${LIBRARY_SOVERSION}
)

install(
  TARGETS
    nss_wrapper
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

set(NSS_WRAPPER_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}nss_wrapper${CMAKE_SHARED_LIBRARY_SUFFIX}" PARENT_SCOPE)
