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
    //send input string to API throught Rust handler.
    //when it returns check for tool handlings
    //if needs to use exposed endpoint, handle and make query.
    //return to API
    //if it needs more tool handlings, loop. else return string response.
 
    //for now just pass through to rust and back to main as proof of concept.

    const char* to_send = input.c_str();

    char* ret_val = return_to_main(to_send);
    if (ret_val != nullptr) {
        std::string response(ret_val);
        free_rust_string(ret_val);

        return response;
    } else {
        return "error in MCP Communication";
    }


}


