#ifndef IMAGEPROCESSOR_RESDIV_H
#define IMAGEPROCESSOR_RESDIV_H

#include <cstdio>

/**
 * @brief Class that creates a number that consists of the results of division with rest
 * @param c the integer part of division
 * @param r the rest of division
 */
class Resdiv {
public:
    size_t c;
    size_t r;
    Resdiv(size_t a, size_t b){
        c = (int)a/b;
        r = a-c*b;
    }

};


#endif //IMAGEPROCESSOR_RESDIV_H
