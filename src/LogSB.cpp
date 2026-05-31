//
//  Evaluator.cpp
//  calc
//
//  Created by Griffin McDonald on 2026-05-10.
//
#include "../include/LogSB.hpp"
#include <iostream>
#include <format>
#include <sstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string escapeJSONString(const std::string& input) {
    std::ostringstream ss;
    for (char c : input) {
        switch (c) {
            case '"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:
                //Only allow standard printable ASCII characters through
                if (c >= '\x20' && c <= '\x7e') {
                    ss << c;
                }
        }
    }
    return ss.str();
}

std::string escapeJSONNumber(double value) {
    if (std::isnan(value) || std::isinf(value)) {
        return "null";
    }
    return std::to_string(value);
}

//Log queried expression and result to SupabaseDB.
void LogSB::logToSupabase(const std::string& expression, double final_result){
    const char* env_url = std::getenv("SUPABASE_URL");
    const char* env_key = std::getenv("SUPABASE_ANON_KEY");

    //Guard against missing variables
    if (env_url == nullptr || env_key == nullptr) {
        std::cerr << "Error: Supabase environment variables are missing!" << std::endl;
        return;
    }

    std::string supabase_url(env_url);
    std::string anon_key(env_key);

    json json_payload = 
    {{"expression",escapeJSONString(expression)},
        {"result", escapeJSONNumber(final_result)}};

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return;
    }

    cpr::Response r = cpr::Post(
        cpr::Url{supabase_url},
        cpr::Header{
            {"apikey", anon_key},                               
            {"Authorization", "Bearer " + anon_key},           
            {"Content-Type", "application/json"},                   
            {"Prefer", "return=minimal"}                            
        },
        cpr::Body{json_payload.dump()}                                     
    );


    if (r.status_code == 201) { 
        std::cout << "Successfully inserted row!" << std::endl;
    } else {
        std::cerr << "Insert failed. Status code: " << r.status_code << '\n'
                  << "Supabase Error: " << r.text << std::endl;
    }
}

