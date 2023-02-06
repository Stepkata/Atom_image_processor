//
// Created by keste on 05.02.2023.
//

#ifndef IMAGEPROCESSOR_RESDIV_H
#define IMAGEPROCESSOR_RESDIV_H

/**
 * @brief Class that creates a number that consists of the results of division with rest
 * @param c the integer part of division
 * @parm r the rst of division
 */
class Resdiv {
public:
    int c;
    int r;
    Resdiv(int a, int b){
        c = (int)a/b;
        r = a-c*b;
    }
};


#endif //IMAGEPROCESSOR_RESDIV_H
