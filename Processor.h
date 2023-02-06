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
    explicit Processor(const char* filename):Image(filename){}
    Processor(int w, int h, int c):Image(w, h, c){}
    ~Processor()= default;

    void grayscale_avg();
    void grayscale_lum();
    void color_mask(float r, float g, float b);
    void flip_x();
    void flip_y();
    Processor& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
    void neon_chromatic_aberration();
    void purple_chromatic_aberration();
    void chromatic_aberration(int n);
    void distortion_filter(float r, float g, float b);
    Processor& overlay(Processor& image, int x, int y);
    Processor& fuse(const std::vector<const char*>& filenames);
    Processor& rotate_right();
    void change_hue(float fHue);
    Processor& overlayText(const char* txt, const Font& font, int x, int y, uint8_t r = 0, uint8_t g = 0,
                           uint8_t b = 0, uint8_t a = 100);
    void cut(bool vertical, bool to_parts, int n);
    void resize(int new_w, int new_h);
    void change_saturation(float change);


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
    Processor& _change_saturation(int start, int end, float change);
    Processor& _change_hue(int start, int end, float matrix[3][3]);
    Processor& _distortion(int start, int end, const float change[]);
    void _cut(int x, int m);
    void _cut_h(int x, int m);
    static uint8_t clamp(float v);
    int num_channels(){
        return (int)size/channels; //@TODO: może zabraknąć inta
    }
    static int num_cut;
};


#endif //IMAGEPROCESSOR_PROCESSOR_H
