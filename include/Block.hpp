#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <raylib.h>


// Block representation

struct Block {
    bool init = false;  //initialized?
    int num = 0;        //number (2,4,8,...)
    float x = 0, y = 0, alpha = 1.0f; //screen position and alpha

    bool merged = false;    //merged this turn?
    int merged_num = 0;     //number after merge
    float merged_x = 0, merged_y = 0, merged_alpha = 1.0f; //merged block screen data

    Block() = default;  
};

#endif
