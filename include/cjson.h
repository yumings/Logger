#pragma once
#include <rapidjson/document.h>
#include <iostream>
#include <string>
#include <fstream>

#if _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <iconv.h>
#endif

std::string UTF8ToGB(const char* str);
std::string GBToUTF8(const char* str);
int GetDocumentFromJsonFile(const std::string &json_file, rapidjson::Document &doc);