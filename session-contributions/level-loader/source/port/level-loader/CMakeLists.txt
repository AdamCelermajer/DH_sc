cmake_minimum_required(VERSION 3.16)
project(dh2_level_loader LANGUAGES CXX)
# Standalone candidate parser audit. No integration/factory ABI is selected yet.
add_library(dh2_loader_xml_reference STATIC
 vendor/tinyxml/tinyxml.cpp vendor/tinyxml/tinyxmlparser.cpp
 vendor/tinyxml/tinyxmlerror.cpp vendor/tinyxml/tinystr.cpp)
target_compile_definitions(dh2_loader_xml_reference PUBLIC TIXML_USE_STL)
target_include_directories(dh2_loader_xml_reference PUBLIC vendor/tinyxml)
set_target_properties(dh2_loader_xml_reference PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_executable(dh2_loader_xml_probe tests/xml_probe.cpp)
target_compile_features(dh2_loader_xml_probe PRIVATE cxx_std_17)
target_compile_options(dh2_loader_xml_probe PRIVATE -Wall -Wextra -Werror)
target_link_libraries(dh2_loader_xml_probe PRIVATE dh2_loader_xml_reference)
