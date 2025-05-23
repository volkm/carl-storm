# Add all targets to the build-tree export set
export(EXPORT carl_Targets FILE "${PROJECT_BINARY_DIR}/carlTargets.cmake")


# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
if(CARL_EXPORT_TO_CMAKE)
	message(STATUS "Registered with cmake")
	set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
	export(PACKAGE carl)
endif()

set(DEP_TARGETS "")
if(USE_BLISS)
	export_target(DEP_TARGETS BLISS_SHARED)
	export_target(DEP_TARGETS BLISS_STATIC)
endif()
if(USE_CLN_NUMBERS)
	export_target(DEP_TARGETS CLN_SHARED)
	export_target(DEP_TARGETS CLN_STATIC GMP_STATIC)
endif()
if(USE_GINAC)
	export_target(DEP_TARGETS GINAC_SHARED CLN_SHARED)
#	export_target(DEP_TARGETS GINAC_STATIC CLN_STATIC)
endif()
if(USE_COCOA)
	export_target(DEP_TARGETS COCOA_SHARED GMPXX_SHARED GMP_SHARED pthread)
	export_target(DEP_TARGETS COCOA_STATIC GMPXX_STATIC GMP_STATIC pthread)
endif()
export_target(DEP_TARGETS GMP_SHARED)
export_target(DEP_TARGETS GMP_STATIC)
export_target(DEP_TARGETS GMPXX_SHARED)
export_target(DEP_TARGETS GMPXX_STATIC GMP_STATIC)
export_target(DEP_TARGETS EIGEN3)
export_target(DEP_TARGETS Boost::headers)

export_target(DEP_TARGETS Doxygen::doxygen)

set(EXP_OPTIONS "")
foreach(option ${EXPORTED_OPTIONS})
	set(EXP_OPTIONS "${EXP_OPTIONS}\nset(CARL_${option} \"${${option}}\")")
endforeach()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/carlConfigVersion.cmake
	COMPATIBILITY AnyNewerVersion
)

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src") # TODO looks not correct, this would be hypro_INCLUDE_DIR

configure_package_config_file(
	cmake/carlConfig.cmake.in
	${PROJECT_BINARY_DIR}/carlConfig.cmake
	INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
	PATH_VARS CARL_INCLUDE_INSTALL_DIR #SYSCONFIG_INSTALL_DIR
)
 # ... for the install tree
set(CONF_INCLUDE_DIRS "\${CMAKE_INSTALL_DIR}/${CARL_INCLUDE_INSTALL_DIR}")

configure_package_config_file(
	cmake/carlConfig.cmake.in
	${PROJECT_BINARY_DIR}/carlConfig.install.cmake
	INSTALL_DESTINATION ${CARL_CMAKE_INSTALL_DIR}
	PATH_VARS CARL_INCLUDE_INSTALL_DIR #SYSCONFIG_INSTALL_DIR
)
