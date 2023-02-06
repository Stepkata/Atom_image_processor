#ifndef IMAGEPROCESSOR_PROCESSOR_H
#define IMAGEPROCESSOR_PROCESSOR_H

#include "image.h"
#include "Font.h"

#include <cstring>
#include <vector>
#include <cmath>
#include <thread>

#define  Pr  .299
#define  Pg  .587
#define  Pb  .114
#define THREADS 4

class Processor: public Image {
public:
    Processor() = default;
    Processor(const char* filename):Image(filename){}
    Processor(int w, int h, int c):Image(w, h, c){}
    ~Processor(){}

    void grayscale_avg();
    void grayscale_lum();
    void color_mask(float r, float g, float b);
    void flip_x();
    void flip_y();
    Processor& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
    void neon_chromatic_aberration();
    void purple_chromatic_aberration();
    void chromatic_aberration(int n);
    Processor& distortion_filter(float r, float g, float b);
    Processor& overlay(Processor& image, int x, int y);
    Processor& fuse(const std::vector<const char*>& filenames);
    Processor& rotate_right();
    Processor& change_hue(float fHue);
    Processor& overlayText(const char* txt, const Font& font, int x, int y, uint8_t r = 0, uint8_t g = 0,
                           uint8_t b = 0, uint8_t a = 100);
    Processor& change_saturation(float change);
    void cut(int n);
    void cut_to_form(int n);
    void cut_horisontal(int n);
    void cut_horisontal_to_form(int n);
    void resize(int new_w, int new_h);


    Processor& operator=(Processor *other){
        if (this != other)
            copy(*other);
        return *this;
    }

    Processor& operator+=(Processor& other);

private:
    Processor& _grayscale_avg(int start, int end);
    Processor& _grayscale_lum(int start, int end);
    Processor& _color_mask(int start, int end, const float change[]);
    Processor& _chromatic_aberration(int start, int end, int n);
    Processor& _flip_x(int start, int end);
    Processor& _flip_y(int start, int end);
    Processor& _neon_ca(int start, int end);
    Processor& _pca(int start, int end);
    void _cut(bool t, int n);
    void _cut_h(bool t, int n);
    uint8_t clamp(float v);
    int num_channels(){
        return size/channels;
    }
    static int num_cut;
};


#endif //IMAGEPROCESSOR_PROCESSOR_H
