cmake_minimum_required(VERSION 3.15)
project(SupabaseCalc)

# Enforce C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# 2. Fetch CPR (C++ Requests)
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)
FetchContent_MakeAvailable(cpr)

# 3. Gather your source files
add_executable(app 
    src/main.cpp 
    src/Calculator.cpp 
    src/Parser.cpp 
    src/Evaluator.cpp 
    src/LogSB.cpp
)

# 4. Link both libraries to your executable
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json cpr::cpr)