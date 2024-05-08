#ifndef _CS_GENERALS_
#define _CS_GENERALS_

#include <iostream>

/////////***********************************\\\\\\\\\\Generals

void logExit(const char* massage) {

    std::cout << massage; //log
    std::terminate();

}

inline int CStrlen(const char* data) {
    int sum = 0;
    int i = 0;
    while (data[i]) { ++sum; ++i; }

    return sum;
}

int CStr(const char* from, char* to, int n) {

    if (!from)   return -1;
    if (!to)     return -2;
    if (!n)      return -3;

    for (int index = 0; index < n; ++index) {
        to[index] = from[index];
    }

    return 0;
}

int toCSTR(const char* from, char* to, int n) {

    CStr(from, to, n - 1);

    to[n - 1] = '\0';

    return 0;
}

//////////////////////////////////////////
// функция сравнения левой строки с правой
// LStr > RStr     return -1
// LStr < RStr     return 1
// LStr == RStr    return 0
//
// /param LStr - left string
//        RStr - right string
//          
// /return -1 0 1
//////////////////////////////////////////
int CStrCmp(const char* LStr, const char* RStr)
{
    if (LStr == RStr) return 0;

    size_t Llength = CStrlen(LStr);
    size_t Rlength = CStrlen(RStr);
    size_t low = (Llength <= Rlength) ? Llength : Rlength;

    for (size_t index = 0; index < low; ++index)
    {
        if (LStr[index] > RStr[index]) return -1;      // LStr > RStr
        if (LStr[index] < RStr[index]) return 1;       // LStr < RStr
    }

    if (Llength > Rlength) return -1;                  // LStr > RStr
    if (Llength < Rlength) return 1;                   // LStr < RStr

    return 0;                                           // LStr == RStr
}


#endif //_CS_GENERALS_