#include <cstdio>
#include <string>
#include <windows.h>
#include "utf8.h"

bool LoadFile(const std::string &path, std::string *contents_ptr){
    FILE *file;
    file = fopen(path.c_str(), "rb");

    if(!file){
        printf("Error! Could not load file: %s\n", path.c_str());
        return false;
    }

    // Determine the size of the file    
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the entire file
    std::string& contents = *contents_ptr;
    contents.resize(len);
    int c = fread(&contents[0], len, 1, file);
    
    fclose(file);
    return true;
}

int main(int argc, char* argv[]){
    std::string str_ascii;
    if(!LoadFile("alice.txt",&str_ascii)){
        getchar();
        return 1;
    } 
    printf("ASCII file loaded: %d bytes\n",str_ascii.length()*sizeof(str_ascii[0]));
    std::string str_utf8 = str_ascii;
    printf("UTF8 file loaded: %d bytes\n",str_utf8.length()*sizeof(str_utf8[0]));

    std::basic_string<int> str_utf32;
    int str_len = str_ascii.length();
    str_utf32.resize(str_len);
    for(unsigned i=0; i<str_len; ++i){
        str_utf32[i] = str_ascii[i];
    }
    printf("UTF-32 file loaded: %d bytes\n",str_utf32.length()*sizeof(str_utf32[0]));
    
    LARGE_INTEGER time_start, time_end;
    QueryPerformanceCounter(&time_start);
    for(int i=0; i<str_len; ++i){
        if(str_ascii[i] == '.'){
            str_ascii[i] = ',';
        }
    }
    QueryPerformanceCounter(&time_end);
    printf("Find/replace from '.' to ',' took %lld microseconds using ASCII\n", time_end.QuadPart - time_start.QuadPart);
    
    QueryPerformanceCounter(&time_start);
    for(int i=0; i<str_len; ++i){
        if(str_utf32[i] == '.'){
            str_utf32[i] = ',';
        }
    }
    QueryPerformanceCounter(&time_end);
    printf("Find/replace from '.' to ',' took %lld microseconds using UTF-32\n", time_end.QuadPart - time_start.QuadPart);

    QueryPerformanceCounter(&time_start);
    std::string processed_str_utf8;
    std::string::iterator iter = str_utf8.begin();
    int code_point;
    while(iter != str_utf8.end()){
        code_point = utf8::next(iter, str_utf8.end());
        if(code_point == '.'){
            utf8::append(',', std::back_inserter(processed_str_utf8));
        } else {
            utf8::append(code_point, std::back_inserter(processed_str_utf8));
        }
    }
    QueryPerformanceCounter(&time_end);
    printf("Find/replace from '.' to ',' took %lld microseconds using UTF-8\n", time_end.QuadPart - time_start.QuadPart);
    getchar();
    return 0;
}