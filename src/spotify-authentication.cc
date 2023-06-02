#include <iostream>
#include <httplib.h>
#include <curl/curl.h>
#include <sstream>
#include <map>
#include <dotenv.h>
#include <json.h>

std::string client_id;
std::string client_secret;
std::string redirect_uri = "http://192.168.178.158:8080/callback";
std::string authorization_code;

std::string base64_encode(const std::string& input) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string encoded;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (char ch : input) {
        char_array_3[i++] = ch;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                encoded += base64_chars[char_array_4[i]];

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; j++)
            encoded += base64_chars[char_array_4[j]];

        while (i++ < 3)
            encoded += '=';
    }

    return encoded;
}

std::string generateAuthUrl() {
    std::string url = "https://accounts.spotify.com/authorize";
    std::string scope = "user-read-private user-read-email"; // Add additional scopes as needed

    std::string authUrl = url + "?client_id=" + client_id +
                          "&response_type=code" +
                          "&redirect_uri=" + redirect_uri +
                          "&scope=" + scope;

    return authUrl;
}

size_t cb(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

void handleRequest(const httplib::Request& req, httplib::Response& res) {
    std::string url = "https://accounts.spotify.com/api/token";

    // Extract the authorization code from the request query parameters
    std::string authorization_code = req.get_param_value("code");

    // Build the request payload
    std::string payload = "grant_type=authorization_code&code=" + authorization_code +
                          "&redirect_uri=" + redirect_uri;

    // Set up cURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "cURL initialization failed." << std::endl;
        return;
    }

    // Set headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Basic " + base64_encode(client_id + ":" + client_secret)).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    // Set cURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the request
    CURLcode res_code = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res_code != CURLE_OK) {
        std::cerr << "cURL request failed: " << curl_easy_strerror(res_code) << std::endl;
        return;
    }

    // Process the response and obtain the refresh token
    std::map<std::string, std::string> responseData;
    std::stringstream responseStream(response);
    std::string line;
    while (std::getline(responseStream, line, '&')) {
        std::size_t equalPos = line.find('=');
        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);
        responseData[key] = value;
    }

    nlohmann::json json = nlohmann::json::parse(responseData);
    std::string accessToken = json["access_token"];

    std::cout << accessToken << std::endl;

    // Send a response if needed
    // res.set_content("Refresh Token: " + refresh_token, "text/plain");
}



int main() {
    dotenv::init();

    client_id = std::getenv("CLIENT_ID");
    client_secret = std::getenv("CLIENT_SECRET");

    std::string authUrl = generateAuthUrl();
    std::cout << "Please click the following link to authenticate: " << std::endl;
    std::cout << authUrl << std::endl;

    httplib::Server server;

    // Handle the callback route
    server.Get("/callback", [](const httplib::Request& req, httplib::Response& res) {
        handleRequest(req, res);
    });

    // Start the server
    server.listen("0.0.0.0", 8080); // Listen on all available network interfaces

    return 0;
}
