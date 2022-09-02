//
// Created by keste on 19.07.2022.
//

#ifndef IMAGEPROCESSOR_PROCESSOR_H
#define IMAGEPROCESSOR_PROCESSOR_H

#include "image.h"
#include <cstring>
#include <vector>

class Processor: public Image {
public:
    Processor() = default;
    Processor(const char* filename):Image(filename){}
    Processor(int w, int h, int c):Image(w, h, c){}
    ~Processor(){}
    Processor& grayscale_avg();
    Processor& grayscale_lum();
    Processor& color_mask(float r, float g, float b);
    Processor& flip_x();
    Processor& flip_y();
    Processor& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
    Processor& neon_chromatic_aberration();
    Processor& purple_chromatic_aberration();
    Processor& chromatic_aberration(int n);
    Processor& distortion_filter(float r, float g, float b);
    Processor& overlay(Processor& image, int x, int y);
    Processor& fuse(std::vector<const char*> filenames);
    Processor& rotate_right(); //@TODO
    void cut(int n);
    void cut_to_form(int w, int h);
    void cut_horisontal(int n);
    void cut_horisontal_to_form(int n);
    bool resize(int new_w, int new_h);

    Processor& operator=(Processor const& other){
        if (this != &other)
            copy(other);
        return *this;
    }

    Processor& operator+=(Processor& other);
private:
    void _cut(bool t, int n);
    void _cut_h(bool t, int n);
};


#endif //IMAGEPROCESSOR_PROCESSOR_H