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
    mcp/McpIO.cpp
)

# 4. Include Directories
# Tells the compiler where to look when it sees #include "Calculator.h" or #include "McpIO.h"
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mcp)

# 5. Cargo Build Step
# Defines where the compiled Rust library will be output
set(RUST_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/target/release")

# Tells CMake to run Cargo automatically
add_custom_target(cargo_build ALL
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building Rust API Shipper library..."
)

# Forces CMake to build the Rust code BEFORE trying to build the C++ app
add_dependencies(app cargo_build)

# 6. Link C++ Libraries
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json cpr::cpr)

# 7. Link Rust & OS System Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Link the Rust static library
    target_link_libraries(app PRIVATE "${RUST_LIB_DIR}/rust_api_shipper.lib")
    # Windows system dependencies for Rust and Tokio network/crypto
    target_link_libraries(app PRIVATE ws2_32 userenv bcrypt)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Link the Rust static library
    target_link_libraries(app PRIVATE "${RUST_LIB_DIR}/librust_api_shipper.a")
    # macOS system frameworks for networking/security
    find_library(SECURITY_FRAMEWORK Security)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    target_link_libraries(app PRIVATE ${SECURITY_FRAMEWORK} ${COREFOUNDATION_FRAMEWORK} pthread dl m)

else() # Linux
    # Link the Rust static library
    target_link_libraries(app PRIVATE "${RUST_LIB_DIR}/librust_api_shipper.a")
    # Linux system dependencies for threading and math
    target_link_libraries(app PRIVATE pthread dl m)
endif()