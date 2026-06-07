//
//  McpIO.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-06-05.
//
#include <vector>
#include <iostream>
#include <string>
#include "../include/McpIO.hpp"
#include "../include/LogSB.hpp"

extern "C" {
    char* return_to_main(const char* raw_input);
    void free_rust_string(const char* raw_input);
}
//main handler for GeminiAPI requests.
std::string McpIO::handleRequest(std::string& input){
    //conver input to c_str so rust can handle it.
    const char* to_send = input.c_str();

    //call rust FFI function for MCP server and LLM items. handle memory
    char* ret_val = return_to_main(to_send);
    if (ret_val != nullptr) {
        std::string response(ret_val);
        free_rust_string(ret_val);

        return response;
    } else {
        return "error in MCP Communication";
    }


}


