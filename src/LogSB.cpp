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
#include <curl/curl.h>

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

    std::string json_payload = 
        "{\"expression\": \"" + escapeJSONString(expression) + "\", " + "\"result\": \"" + escapeJSONNumber(final_result) + "\"}";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("apikey: " + anon_key).c_str());
    headers = curl_slist_append(headers, ("Authorization: Bearer " + anon_key).c_str());
    headers = curl_slist_append(headers, "Prefer: return=minimal");

    //Set the target Supabase URL
    curl_easy_setopt(curl, CURLOPT_URL, supabase_url.c_str());

    //Pass the headers list to the request
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    //Pass the payload string (equivalent to cpr::Body and implicitly sets method to POST)
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());

    //Execute the request synchronously
    CURLcode res = curl_easy_perform(curl);

    //Check for network/protocol layer errors
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        //Retrieve the HTTP response status code (e.g., 201 for success)
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        
        if (response_code == 201) {
            std::cout << "Successfully posted payload to Supabase!" << std::endl;
        } else {
            std::cerr << "Supabase API returned error status: " << response_code << std::endl;
        }
    }

    //Free the allocated linked list memory and clean up the session handle
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

