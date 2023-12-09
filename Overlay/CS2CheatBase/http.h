#pragma once
#include <Windows.h>
#include <WinInet.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>


#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Crypt32.lib")

using namespace std;

std::string HttpRequest(const char* url, const char* accessToken) {
    std::string responseData;

    HINTERNET hInternet = InternetOpenA("WinINet Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpen failed: " << GetLastError() << std::endl;
        return responseData;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        std::cerr << "InternetOpenUrl failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hInternet);
        return responseData;
    }

    // Set request headers (including Authorization header)
    std::string headers = "Authorization: Bearer ";
    headers += accessToken;
    if (!HttpSendRequestA(hConnect, headers.c_str(), headers.length(), NULL, 0)) {
        std::cerr << "HttpSendRequest failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return responseData;
    }

    // Read response
    constexpr int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead = 0;
    while (InternetReadFile(hConnect, buffer, BUFFER_SIZE - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        responseData += std::string(buffer, buffer + bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return responseData;
}


std::string SendPostRequest(const std::string& url, const std::string& data, const std::string& accessToken) {
    HINTERNET hInternet = InternetOpen("WinINet Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpen failed: " << GetLastError() << std::endl;
        return "";
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        std::cerr << "InternetOpenUrl failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // Set request headers (including Authorization header)
    std::string headers = "Authorization: Bearer " + accessToken + "\r\nContent-Type: application/json";
    if (!HttpSendRequestA(hConnect, headers.c_str(), headers.length(), const_cast<char*>(data.c_str()), data.length())) {
        std::cerr << "HttpSendRequest failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    // Read response
    constexpr int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead = 0;
    std::string responseData;
    while (InternetReadFile(hConnect, buffer, BUFFER_SIZE - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        responseData += buffer;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return responseData;
}

std::string DecodeBase64(const std::string& encodedData) {
    std::string decodedData;
    DWORD decodedLength = 0;

    CryptStringToBinaryA(encodedData.c_str(), encodedData.length(), CRYPT_STRING_BASE64, nullptr, &decodedLength, nullptr, nullptr);

    std::vector<BYTE> buffer(decodedLength);
    if (CryptStringToBinaryA(encodedData.c_str(), encodedData.length(), CRYPT_STRING_BASE64, buffer.data(), &decodedLength, nullptr, nullptr)) {
        decodedData.assign(buffer.begin(), buffer.begin() + decodedLength);
    }

    return decodedData;
}

std::string ResponseParser(const std::string& response) {
    std::size_t found = response.find("\"content\":");
    if (found != std::string::npos) {
        std::string smth2;
        std::string smth = response.substr(found + 11); // Extract the substring after "content":
        found = smth.find('"');
        smth = smth.substr(0, found);
        do {
            found = smth.find("\\n");
            smth2 = smth.substr(0, found);
            smth = smth2 + smth.substr(found + 2);
        } while (smth.find("\\n") != std::string::npos);
        return smth2;
    }
    return ""; // Return empty string if extraction fails
}