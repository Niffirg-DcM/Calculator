use reqwest::{Error, Response};
use serde::{Deserialize, Serialize};
use std::fs::read_to_string;
use std::ptr::null_mut;
use std::time::Duration;
use std::io;
use dotenvy::dotenv;
use std::ffi::{CStr, CString};
use std::os::raw::c_char;

//incoming payloads with deserialize
#[derive(Deserialize, Debug)]
struct inCandidates {
    content: inContents,
}

#[derive(Deserialize, Debug)]
struct inContents {
    parts: Vec<inParts>,
    role: String,
}

#[derive(Deserialize, Debug)]
struct inParts {
    text: String,
}

#[derive(Deserialize, Debug)]
struct GeminiResponse {
    candidates: Vec<inCandidates>,
}


//outgoing payloads with serialize
#[derive(Serialize)]
struct outCandidates {
    contents: Vec<outContents>,
}

#[derive(Serialize)]
struct outContents {
    parts: Vec<outParts>,
}

#[derive(Serialize)]
struct outParts {
    text: String,
}

//unsafe operation so tag. ensure async process runs before returning
#[unsafe(no_mangle)]
pub extern "C" fn return_to_main(raw_input: *const c_char) -> *mut c_char {
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();

    //Block the calling FFI thread until the async function completes
    rt.block_on(async {

        let result = api_call(raw_input).await;

        let to_be_returned = match result {

            Ok(s) => {
                {
            let c_string = CString::new(s).unwrap();
            
            c_string.into_raw()
        }
            },

            Err(e) => {
                let c_string = CString::new(e.to_string()).unwrap();
            
                c_string.into_raw()
            },
        };

        return to_be_returned;
    
    })

}

//async func for making api call
async fn api_call(raw_input: *const c_char) -> Result<String,Box<dyn std::error::Error>> {
    //ensure .env file exists
    dotenv().expect(".env file not found");


    //convert constant c char pointer into string to send in request.
    let c_str = unsafe {CStr::from_ptr(raw_input)};
    let data_to_send = c_str.to_str()?;


    //ensure api key is there
    let key = env::var("GEMINI_API_KEY")?;



    //instantiate client request
    let client = reqwest::Client::builder()
        .timeout(Duration::from_secs(10))
        .build()?;





    let url = format!("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key={}",key);

    //create payload to send
    let payload = outCandidates {
        contents: vec![ outContents {
            parts: vec![ outParts {
                text: String::from(data_to_send)}
                ]
            } ]
    };

    //send post and get response
    let post_response:Response = client.post(url)
        .header("Content-Type", "application/json")
        .json(&payload)
        .send()
        .await?;


    //if successful post then parse response to send back to main
    if post_response.status().is_success() {
        let parsed_response: GeminiResponse = post_response.json().await?;

        Ok(parsed_response.candidates[0].content.parts[0].text.clone())     
    } else {
        Err("error".into())
    }

    
    
}

//handle memory logic for cpp.
#[unsafe(no_mangle)]
pub extern "C" fn free_rust_string(s: *mut c_char) {
    if s.is_null() { return; }
    
    //take back ownership of memory with from raw to ensure properly freed memory.
    unsafe {
        let _ = CString::from_raw(s);
    }
}

