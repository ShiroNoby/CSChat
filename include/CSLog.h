#ifndef _CS_LOG_
#define _CS_LOG_

#include <string>
#include <vector>
#include <fstream>

//|tier|type_log|file|line|massage|param-opt|

enum Tier {
    tier0 = 0,
    tier1,
    tier2,
    tier3
};

const char* tierToStr( Tier tier ){
    switch( tier ){
        case tier0:
            return "";
        case tier1:
            return "\t";
        case tier2:
            return "\t\t";
        case tier3:
            return "\t\t\t";
        default: return "";
    }
}

///////**********************************************************************\\\\\\\\CSLog

class CSLog{
    
    size_t size;
    size_t max_size;
    std::vector< std::string > log;
    std::string filePath;
    
public:
    
    CSLog(const char*);
    ~CSLog();
    
    void clearfile();
    void fileWrite();
    
    void write( const std::string& );   

    void SystemLog(Tier tier, const char* file, int line, const char* msg);
    void UserLog(Tier tier, const char* file, int line, const char* msg);
};

CSLog::CSLog(const char* path) : size(0), max_size(100), log(max_size), filePath(path) {
    
    std::fstream file(filePath);
     
    if (!file.is_open())
    {
        file.clear();
        file.open(filePath, file.out ); // create file
        file.close();
        file.open(filePath);
    }
    
    file << "start app\n";
    
    file.close();
}

CSLog::~CSLog(){
    
    write("stop app");
    fileWrite();
}

void CSLog::clearfile() {

    std::fstream file(filePath, std::fstream::out);

}

void CSLog::fileWrite(){
    
    std::fstream file(filePath, std::fstream::app );
    
    if (!file.is_open())
    {
        file.clear();
        file.open(filePath, file.out ); // create file
        file.close();
        file.open(filePath, file.app );
    }
    
    for( size_t index = 0; index <= size; ++index ){
        file << log[index] << '\n';
    }  
    
    size = 0;
}

void CSLog::write( const std::string& str ){    
    
    if( !str.size() ){ return; }        
        
    if( size == max_size - 1 )
        fileWrite();
    
    log[size] = str;
    size += 1;
}

///////**********************************************************************\\\\\\\\SystemLog

void CSLog::SystemLog(Tier tier, const char* file, int line, const char* msg ){
    
    std::string str( tierToStr( tier ) );
    str += "System_log"; str += ' ';
    str += file; str += ' ';
    str += std::to_string(line); str += ' ';
    str += msg;  str += ' ';
    
    write( str );
}

///////**********************************************************************\\\\\\\\UserLog

//|tier|file|line|massage|
void CSLog::UserLog(Tier tier, const char* file, int line, const char* msg ){
    
    std::string str( tierToStr( tier ) );
    str += "User_log"; str += ' ';
    str += file; str += ' ';
    str += std::to_string(line); str += ' ';
    str += msg;  str += ' ';
    
    write( str );
}
#endif //_CS_LOG_