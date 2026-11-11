cmake_minimum_required(VERSION 3.21)
project(DynamicAnimationCasting VERSION 3.2.5
    DESCRIPTION "Dynamic Animation Casting NG Plus - STB resource HUD fix" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
option(DAC_BUILD_TESTS "Build resource-cost regression tests." ON)
# CommonLib's own integration tests are not part of this plugin's test suite.
set(BUILD_TESTS OFF CACHE BOOL "Build CommonLib tests." FORCE)
add_subdirectory(external/CommonLibSSE-NG)
include(external/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)
find_package(tomlplusplus CONFIG REQUIRED)

configure_file(cmake/version.rc.in version.rc @ONLY)
add_commonlibsse_plugin(${PROJECT_NAME}
    AUTHOR "ArcEarth / STB-Team"
    SOURCES src/main.cpp src/Framework.cpp src/Framework.h
            src/DynamicAnimationCasting.cpp src/DynamicAnimationCasting.h
            src/ResourceCost.h "${CMAKE_CURRENT_BINARY_DIR}/version.rc")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME loki_DynamicAnimationCasting)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /permissive- /Zc:preprocessor /Zi)
    target_link_options(${PROJECT_NAME} PRIVATE /DEBUG:FULL)
endif()

if(DAC_BUILD_TESTS)
    enable_testing()
    add_executable(ResourceCostTest test/ResourceCostTest.cpp)
    target_include_directories(ResourceCostTest PRIVATE src)
    add_test(NAME ResourceCost COMMAND ResourceCostTest)
endif()

# Copy only after an explicit deploy; configuring/building never replaces the game DLL.
set(DAC_DEPLOY_DIR "" CACHE PATH "Mod directory for explicit deploy target.")
if(DAC_DEPLOY_DIR)
    add_custom_target(deploy
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DAC_DEPLOY_DIR}/SKSE/Plugins"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>"
            "${DAC_DEPLOY_DIR}/SKSE/Plugins/loki_DynamicAnimationCasting.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
            "${DAC_DEPLOY_DIR}/SKSE/Plugins/loki_DynamicAnimationCasting.pdb"
        DEPENDS ${PROJECT_NAME} VERBATIM)
endif()
