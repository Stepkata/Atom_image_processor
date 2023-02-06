#ifndef IMAGEPROCESSOR_RESDIV_H
#define IMAGEPROCESSOR_RESDIV_H

/**
 * @brief Class that creates a number that consists of the results of division with rest
 * @param c the integer part of division
 * @param r the rest of division
 */
class Resdiv {
public:
    int c;
    int r;
    Resdiv(int a, int b){
        this->b = b;
        c = (int)a/b;
        r = a-c*b;
    }
private:
    int b;
};


#endif //IMAGEPROCESSOR_RESDIV_H
