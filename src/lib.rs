use reqwest::{Error, Response};
use serde::{Deserialize, Serialize};
use serde_json::from_str;
use std::time::Duration;
use dotenvy::dotenv;
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use tokio::process::Command;
use std::process::Stdio;
use std::io::{Write};
use tokio::io::{self, AsyncBufReadExt, AsyncWriteExt, BufReader};

//incoming payloads with deserialize
#[derive(Deserialize, Debug)]
struct GeminiResponse {
    candidates: Vec<InCandidates>,
}

#[derive(Deserialize, Debug)]
struct InCandidates {
    content: InContents,
}


#[derive(Deserialize, Debug)]
struct InContents {
    parts: Vec<InParts>,
    role: String,
}

#[derive(Deserialize, Debug)]
struct InParts {
    text: String,
}

//outgoing payloads with serialize
#[derive(Serialize)]
struct OutCandidates {
    contents: Vec<OutContents>,
    tools: RootTools,
}

#[derive(Serialize)]
struct OutContents {
    parts: Vec<OutParts>,
}

#[derive(Serialize)]
struct OutParts {
    text: String,
}

#[derive(Debug, Serialize, Deserialize)]
struct RootTools {
    tools: Vec<OutTools>,
}

#[derive(Debug, Serialize, Deserialize)]
struct OutTools {
    functionDeclaractions: Vec<OutDeclarations>,
}

#[derive(Debug, Serialize, Deserialize)]
struct OutDeclarations {
    name: String,
    description: String,
    parameters: OutParams,
}

#[derive(Debug, Serialize, Deserialize)]
struct OutParams {
    r#type: String,
    properties: OutProps
}

#[derive(Debug, Serialize, Deserialize)]
struct OutProps {
    location: OutLocation,
}

#[derive(Debug, Serialize, Deserialize)]
struct OutLocation {
    r#type: String,
}

// unsafe operation so tag. ensure async process runs before returning
#[unsafe(no_mangle)]
pub extern "C" fn return_to_main(raw_input: *const c_char) -> *mut c_char {

    //create a runtime so we can run async code fully before returning.
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();

    let child_path = env::var("PATH_TO_SERVER_CARGO_TOML").expect("failed");
    //instantiate child MCP server

    rt.block_on(async {
        //creating running process from binaries (allows inouts and to grab outputs)
        let mut child  = Command::new("cargo")
        .arg("run")
        // Point exactly to the Cargo.toml file of the other project
        .arg("--manifest-path")
        .arg(child_path)
        .stdout(Stdio::piped())
        .stdin(Stdio::piped())
        .spawn()
        .expect("failed to spawn child process");


        //grab input stream to send data.
        let mut sin = child.stdin.take().expect("Failed to open stdin");


        //output stream
        let sout = child.stdout.take().expect("failed to open stdout");


        //BufReader for safety
        let mut out_reader = BufReader::new(sout);
        //let mut in_reader = BufReader::new(io::stdin());


        //CALL MCP SERVER FOR CONTEXT TO PROVIDE TO LLM HERE
        let mut mcp_context = String::new();


        //reading initial output of MCP server, should be tools.
        out_reader.read_line(&mut mcp_context).await.expect("failed to read data from mcp server");

        println!("{}",mcp_context);
        
        //prepping input to api_call
        let mut to_input = raw_input.clone();


        //loop over LLM->MCP->LLM until non-tool response.
        loop {
            // calling api with extra params
            let result = api_call(to_input, mcp_context.clone()).await;
            let string_res = match result {
                Ok(s) => s,
                Err(e) => e.to_string(),
            };

            //if not making a tool request, return the response/error
            if string_res.get(0..4) != Some("tool") {
                let c_string = CString::new(string_res).unwrap();
                return c_string.into_raw();
            }

            //ship off tool request to mcp for it to handle and gather resulting data
            sin.write_all(string_res.as_bytes());

            //mutable string to handle input
            let mut buf_input = String::new(); 

            //grab output from MCP server
            out_reader.read_line(&mut buf_input).await.expect("failed to read data from mcp server");
            //converting string to *const i8 for api call.
            to_input = CString::new(buf_input).expect("String contained null bytes").as_ptr();


        }
    })
}


//async func for making api call
async fn api_call(raw_input: *const c_char, mcp_context:String) -> Result<String,Box<dyn std::error::Error>> {
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

    //unpacking mcp string response into RootTools struct.
    let parsed_data: RootTools = serde_json::from_str(&mcp_context)?;

    //create payload to send
    let payload = OutCandidates {
        contents: vec![ OutContents {
            parts: vec![ OutParts {
                text: String::from(data_to_send)}
                ]
            } ],
        tools: parsed_data
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