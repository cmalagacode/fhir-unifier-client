cmake_minimum_required(VERSION 4.0)
project(fhir_unifier_client VERSION 0.1.0 DESCRIPTION "CLI" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ASM_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(fhir_unifier_client main.cpp)
find_package(CURL REQUIRED)
include(FetchContent)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
if (WIN32)
    find_package(CPR REQUIRED)
else()
    set(CPR_USE_SYSTEM_CURL ON)
    FetchContent_Declare(
            cpr
            GIT_REPOSITORY https://github.com/libcpr/cpr.git
            GIT_TAG 1.12.0
    )
    FetchContent_MakeAvailable(cpr)
endif()
FetchContent_MakeAvailable(catch2)
FetchContent_MakeAvailable(json)
add_subdirectory(src)
add_subdirectory(tests)
