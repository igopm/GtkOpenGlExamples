#pragma once

// By Oleksiy Grechnyev
// Fatal Error for MotokoGL

#include <iostream>

#include "./glheader.h"


namespace MotokoGL{

inline void fatal(const char * msg){
    std::cerr << msg << std::endl;
    exit(-1);
}
//=============================================================
inline void fatal2(const char * msg1, const char * msg2){
    std::cerr << msg1 << " : " << msg2 << std::endl;
    exit(-1);
}
}

