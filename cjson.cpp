#include "cjson.h"


// get doc object from json file
int ResolveJson(const std::string &json_string, rapidjson::Document &doc)
{
    doc.Parse(json_string.c_str());
    if (doc.HasParseError())
    {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        switch (code)
        {
        case rapidjson::kParseErrorDocumentEmpty:
            std::cout << "ParseErrorCode = " << code << ", The document is empty." << std::endl;
            break;
        case rapidjson::kParseErrorDocumentRootNotSingular:
            std::cout << "ParseErrorCode = " << code << ", The document root must not follow by other values." << std::endl;
            break;
        case rapidjson::kParseErrorValueInvalid:
            std::cout << "ParseErrorCode = " << code << ", Invalid value." << std::endl;
            break;
        case rapidjson::kParseErrorObjectMissName:
            std::cout << "ParseErrorCode = " << code << ", Missing a name for object member." << std::endl;
            break;
        case rapidjson::kParseErrorObjectMissColon:
            std::cout << "ParseErrorCode = " << code << ", Missing a colon after a name of object member." << std::endl;
            break;
        case rapidjson::kParseErrorObjectMissCommaOrCurlyBracket:
            std::cout << "ParseErrorCode = " << code << ", Missing a comma or '}' after an object member." << std::endl;
            break;
        case rapidjson::kParseErrorArrayMissCommaOrSquareBracket:
            std::cout << "ParseErrorCode = " << code << ", Missing a comma or ']' after an array element." << std::endl;
            break;
        case rapidjson::kParseErrorStringUnicodeEscapeInvalidHex:
            std::cout << "ParseErrorCode = " << code << ", Incorrect hex digit after \\u escape in string." << std::endl;
            break;
        case rapidjson::kParseErrorStringUnicodeSurrogateInvalid:
            std::cout << "ParseErrorCode = " << code << ", The surrogate pair in string is invalid." << std::endl;
            break;
        case rapidjson::kParseErrorStringEscapeInvalid:
            std::cout << "ParseErrorCode = " << code << ", Invalid escape character in string." << std::endl;
            break;
        case rapidjson::kParseErrorStringMissQuotationMark:
            std::cout << "ParseErrorCode = " << code << ", Missing a closing quotation mark in string." << std::endl;
            break;
        case rapidjson::kParseErrorStringInvalidEncoding:
            std::cout << "ParseErrorCode = " << code << ", Invalid encoding in string." << std::endl;
            break;
        case rapidjson::kParseErrorNumberTooBig:
            std::cout << "ParseErrorCode = " << code << ", Number too big to be stored in double." << std::endl;
            break;
        case rapidjson::kParseErrorNumberMissFraction:
            std::cout << "ParseErrorCode = " << code << ", Miss fraction part in number." << std::endl;
            break;
        case rapidjson::kParseErrorNumberMissExponent:
            std::cout << "ParseErrorCode = " << code << ", Miss exponent in number." << std::endl;
            break;
        case rapidjson::kParseErrorTermination:
            std::cout << "ParseErrorCode = " << code << ", Parsing was terminated." << std::endl;
            break;
        case rapidjson::kParseErrorUnspecificSyntaxError:
            std::cout << "ParseErrorCode = " << code << ", Unspecific syntax error." << std::endl;
            break;
        default:
            break;
        }
        return -1;
    }

    return 0;
}


// UTF-8到GB2312的转换
std::string UTF8ToGB(const char* str)
{
    std::string result;

#if _WIN32
    WCHAR *strSrc;
    LPSTR szRes;

    // 获得临时变量的大小
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[len + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, len);

    // 获得临时变量的大小
    len = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[len + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, len, NULL, NULL);

    result = szRes;
    delete[] strSrc;
    delete[] szRes;
#else
    result = std::string(str);
#endif

    return result;
}

// GB2312到UTF-8的转换
std::string GBToUTF8(const char* str)
{
    std::string result;

#if _WIN32
    WCHAR *strSrc;
    CHAR *szRes;

    // 获得临时变量的大小
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[len + 1];
    MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, len);

    // 获得临时变量的大小
    len = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, len, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;
#else
    result = std::string(str);
#endif

    return result;
}


// read the whole json file into a string.
int GetStringFromJsonFile(const std::string &json_file, std::string &json_string)
{
    std::ifstream ifs;
    ifs.open(json_file, std::ifstream::in);
    if (ifs.is_open() == false)
    {
        std::cout << "Open json file failed." << std::endl;
        ifs.close();
        return -1;
    }

    json_string.clear();
    std::string line;

    while (getline(ifs, line)) // TODO: 此步骤会导致包含中文的字符不能正常解析, ifstream读取UTF-8中文需要处理, 已解决
    {
        line = UTF8ToGB(line.c_str());
        json_string.append(line + "\n");
    }

    ifs.close();
    return 0;
}

int GetDocumentFromJsonFile(const std::string &json_file, rapidjson::Document &doc)
{
    std::string buffer;

    if (GetStringFromJsonFile(json_file, buffer) != 0)
        return -1;

    if (ResolveJson(buffer, doc) != 0)
        return -1;

    return 0;
}
