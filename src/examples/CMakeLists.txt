include_directories(${PROJECT_SOURCE_DIR}/src ${include_dirs})

configure_file( ${PROJECT_SOURCE_DIR}/src/examples/config.h.in
			   ${PROJECT_SOURCE_DIR}/src/examples/config.h
)

add_custom_target(all-examples)

file(GLOB_RECURSE example_sources LIST_DIRECTORIES false RELATIVE ${PROJECT_SOURCE_DIR}/src/examples/ "*.cpp")

foreach(src ${example_sources})
	string(REGEX REPLACE ".cpp$" "" name ${src})
	string(REGEX REPLACE "/" "-" name "example-${name}")
	add_executable(${name} ${src})
	target_link_libraries(${name} lib_carl)
	add_dependencies(all-examples ${name})
endforeach()
