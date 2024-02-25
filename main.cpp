#include <iostream>
#include <map>
#include <ctime>
#include <sstream>
#include <string>
#include <cstring>
using namespace std;
// Structure to represent a key
struct Key {
    string kid;
    string public_key;
    time_t expiry_time;
};

// Dummy key data
map<string, Key> keyMap = {
    {"key1", {"key1", "public_key1", time(nullptr) + 3600}}, // expires in 1 hour
    {"key2", {"key2", "public_key2", time(nullptr) + 7200}}  // expires in 2 hours
};

// Function to generate JWKS response
string generateJWKS() {
    ostringstream oss;
    oss << "{\"keys\":[";
    for (const auto& pair : keyMap) {
        const Key& key = pair.second;
        oss << "{\"kty\":\"RSA\",\"kid\":\"" << key.kid << "\",\"use\":\"sig\",\"n\":\"" << key.public_key << "\"},";
    }
    oss << "]}";
    return oss.str();
}
// Function to handle authentication
string authenticate(const string& kid) {
    auto it = keyMap.find(kid);
    if (it != keyMap.end()) {
        if (it->second.expiry_time > time(nullptr)) {
            return "Authentication successful";
        } else {
            return "Key expired";
        }
    }
    return "Invalid kid";
}

int main() {
    // Start listening on port 8080
    cout << "Server started on port 8080" << endl;

    // Simulate incoming requests
    while (true) {
        string request;
        getline(std::cin, request);

        // Check if the request is for JWKS endpoint
        if (request.find("GET /jwks") != string::npos) {
            cout << "HTTP/1.1 200 OK" << endl;
            cout << "Content-Type: application/json" << endl << endl;
            cout << generateJWKS() << endl;
        }
 else {
            cout << "HTTP/1.1 404 Not Found" << endl;
        }
    }

    return 0;
}

