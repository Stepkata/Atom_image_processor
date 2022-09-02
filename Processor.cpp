#define STB_IMAGE_RESIZE_IMPLEMENTATION


#include "Processor.h"
#include "stb_image_resize.h"

#include <iostream>

#define BYTE_BOUND(value) value < 0? 0: (value<255? 255:value)

Processor& Processor::grayscale_avg(){
    if (channels <3){
        printf("Too little channels, the image is already grayscale!");
    }
    else{
        for(int i =0; i<this->size; i+=channels){
            int gray = (data[i]+data[i+1]+data[i+2])/3;
            memset(data+i, gray, 3); //Ustawia pierwsze num bajtów bloku wskazywanego przez ptr na value (traktowane jako unsigned char).
        }
    }
    return *this;
}

Processor& Processor::grayscale_lum(){ //the better grayscale
    if (channels <3){
        printf("Too little channels, the image is already grayscale!");
    }
    else{
        for(int i =0; i<this->size; i+=channels){
            int gray = 0.2126*data[i]+0.7152*data[i+1]+0.0722*data[i+2];
            memset(data+i, gray, 3); //Ustawia pierwsze num bajtów bloku wskazywanego przez ptr na value (traktowane jako unsigned char).
        }
    }
    return *this;

}

Processor& Processor::color_mask(float r, float g, float b){
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else{
        for (int i=0; i<size; i+=channels){
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }
    return *this;
}

void Processor::cut(int n) {
    _cut(true, n);
}

Processor& Processor::flip_x() {
    uint8_t pom[this->channels];
    uint8_t *ptr1, *ptr2;
    for (int y=0; y<this->height; ++y){
        for(int x=0; x< this->width/2; x++){
            ptr1 = &this->data[(x +y*this->width)*channels];
            ptr2 = &this->data[((this->width-1-x)+y*this->width)*channels];
            memcpy(pom, ptr1, this->channels);
            memcpy(ptr1, ptr2, this->channels);
            memcpy(ptr2, pom, this->channels);
        }
    }

    return *this;
}

Processor& Processor::flip_y() {
    uint8_t pom[this->channels];
    uint8_t *ptr1, *ptr2;
    for (int x=0; x<this->width; ++x){
        for(int y=0; y< this->height/2; y++){
            ptr1 = &this->data[(x +y*this->width)*channels];
            ptr2 = &this->data[(x+(this->height-1-y)*this->width)*channels];
            memcpy(pom, ptr1, this->channels);
            memcpy(ptr1, ptr2, this->channels);
            memcpy(ptr2, pom, this->channels);
        }
    }
    return *this;
}

Processor &Processor::neon_chromatic_aberration() {
    for (int y=0; y<this->height; ++y){
        for(int x=0; x< this->width; x++){
            std::swap(this->data[(x +y*this->width)*channels+5], this->data[(x +y*this->width)*channels+30]);
        }
    }

    return *this;
}

Processor &Processor::purple_chromatic_aberration() {
    for (int y=0; y<this->height; ++y){
        for(int x=0; x< this->width; x++){
            std::swap(this->data[(x +y*this->width)*channels+2], this->data[(x +y*this->width)*channels+20]);
        }
    }

    return *this;
}

Processor &Processor::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch){
    uint8_t *cropped_data = new uint8_t [cw*ch*this->channels];
    memset(cropped_data, 0, cw*ch*this->channels);
    for (uint16_t y=0; y< ch; ++y){
        if(y+cy >= height)
            break;
        for(uint16_t x=0; x<cw; ++x){
            if (x+cx >= width)
                break;
            memcpy(&cropped_data[(x+y*cw)*channels], &data[((x + cx) + (y+cy)*this->width)*channels], channels);
        }
    }

    this->width = cw;
    this->height = ch;
    set_size();
    this->free();
    this->data = cropped_data;
    cropped_data = nullptr;

    return *this;
}

Processor &Processor::chromatic_aberration(int n) {
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a chromatic aberration filter!");
    }
    else{
        for (int i=0; i<size; i+=channels){
            if (i+n*channels < size)
                data[i] = data[i+n*channels];
        }
    }
    return *this;
}

Processor &Processor::distortion_filter(float r, float g, float b) {
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else{
        for (int i=0; i<size; i+=channels){
            uint8_t f = data[i], s = data[i+1], t = data[i+2];
            data[i] = data[i]==255?data[i]: f*r;
            data[i+1] = data[i+1]==255? data[i+1]: s*g;
            data[i+2] = data[i+2]==255? data[i+2]: t*b;
        }
    }
    return *this;
}

void Processor::cut_to_form(int w, int h) {
    if (w < this->width)
        crop(0, 0, w, this->height);
    _cut(false, h);
}

void Processor::_cut(bool t, int n) {
    int x = t? int(this->height/n): n; //n jest ilością części czy rozmiarem? t=true - n jest ilością częsci
    int m = t? n : int(this->height/n)+1;
    int pom = 0;
    for (int i=0; i<m; i++){
        //skopiuj część obrazka do nowej tablicy
        int new_height = (i==m-1) ? this->height-pom : x;
        Image nowy(this->width, new_height, this->channels);
        for (int i=pom*this->width* this->channels, j=0; i<(pom+new_height)*this->width* this->channels; i++, j++)
            nowy.set_data(j, this->data[i]);
        std::string nazwa = "C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\cut2_" + std::to_string(i) + ".png";
        nowy.write(nazwa.c_str());
        pom += new_height;

    }

}

Processor &Processor::overlay(Processor& image, int x, int y) {
    uint8_t *source;
    uint8_t *destination;
    for(int sy = 0; sy<image.get_height(); ++sy){
        if (sy+y <0)
            continue;
        else if (sy + y >= height)
            break;
        for (int sx = 0; sx < image.get_width(); ++sx){
            if (sx+x <0)
                continue;
            else if (sx + x >= width)
                break;
            source = &image.get_data()[(sx + sy*image.get_width())*image.get_channels()];
            destination = &data[(sx + x + (sy+y)*width)*channels];

            float src_alpha = image.get_channels() < 4 ? 1: source[3]/255.f;
            float dest_alpha = this->channels < 4 ? 1: destination[3]/255.f;


            if(src_alpha ==1 && dest_alpha ==1){
                if(image.get_channels() >= this->channels)
                    memcpy(destination, source, channels);
                else //jeżeli ten drugi jest grayscale @TODO: poprawić bo nie zawsze działa
                    memset(destination, source[0], channels);
            }

            else{
                float out_alpha = src_alpha + dest_alpha*(1-src_alpha);
                if(out_alpha < .01)
                    memset(destination, 0, channels);
                else{
                    for(int chnl=0; chnl<channels;chnl++)
                        destination[chnl] = (uint8_t )BYTE_BOUND((source[chnl]/255.f *src_alpha + destination[chnl]/255.f*dest_alpha*(1-src_alpha))/out_alpha*255.f);
                    if(channels>3)
                        destination[3] = (uint8_t )BYTE_BOUND(out_alpha*255.f);
                }
            }
        }
    }
    return *this;
}

Processor& Processor::operator+=(Processor& other){
    if (this!= &other){
        if (this->channels != other.get_channels())
            throw MyException("Incopatibile channels!");

        if(this->width < other.get_width()) //muszą być tej samej szerokości
            other.crop(0, 0, this->width, other.get_height());
        else if(this->width > other.get_width())
            this->crop(0, 0, other.get_width(), this->height);

        int new_height = this->height+other.get_height();
        int new_size = this->size + other.get_size();

        uint8_t *new_data = new uint8_t [new_size];
        int i =0;
        for(; i<this->size; i++){
            memcpy(&new_data[i], &data[i], channels); //@TODO: zrobić to lepiej
        }
        for(int j=0; j<other.get_size(); j++, i++){
            memcpy(&new_data[i], &other.get_data()[j], channels);
        }
        free();
        this->data = new_data;
        new_data = nullptr;
        this->height = new_height;
        this->size = new_size;
    }
    return *this;
}

Processor &Processor::fuse(std::vector<const char*> filenames) {
    for (auto name : filenames){
        Processor other(name);
        *this+=other;
    }
    return *this;
}

Processor &Processor::rotate_right() {
    uint8_t *new_data = new uint8_t [this->size];
    int new_height = this->width;
    int new_width = this->height;

    int h=this->height, w=this->width, ch = this->channels, pom = 0, index = 0, pom2=0;

    for(int i=0; i<w; i++, pom2+=ch){
        pom = pom2;
        for(int j=0; j<h; j++, pom += w*ch)
            for(int n=0; n<ch; n++, index++){
                new_data[index] = data[pom + n];
            }
    }
    free();
    this->data = new_data;
    new_data = nullptr;
    this->height = new_height;
    this->width = new_width;
    flip_x();
    return *this;
}

void Processor::_cut_h(bool t, int n) {
    int x = t? int(this->width/n): n; //n jest ilością części czy rozmiarem? t=true - n jest ilością częsci
    int m = t? n : int(this->width/n)+1;
    int pom = 0;
    for (int i=0; i<m; i++){
        //skopiuj część obrazka do nowej tablicy
        int new_width = (i==m-1) ? this->width-pom : x;
        Image nowy(new_width, this->height, this->channels);

        int j=0;
        int pomi = pom*channels;
        for (int k=0; k<this->height; k++){
            for(int i=pomi; i<new_width*channels+pomi; i++, j++)
                nowy.set_data(j, this->data[i]);
            pomi+=this->width*this->channels;
        }


        std::string nazwa = "C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\cut_hor_" + std::to_string(i) + ".png";
        nowy.write(nazwa.c_str());
        pom += new_width;

    }
}

void Processor::cut_horisontal(int n) {
    _cut_h(1, n);
}

void Processor::cut_horisontal_to_form(int n) {
    _cut_h(0, n);
}

void Processor::resize(int new_w, int new_h) { //@TODO: zrobić wersję z alphą
    auto *new_data = new uint8_t [new_w*new_h*this->channels];
    int success;
    success = stbir_resize_uint8((const uint8_t*)this->data, this->width, this->height, 0,
                       new_data, new_w, new_h, 0, this->channels);
    if(!success)
        throw MyException("Resize failure!");
    delete [] data;
    this->data = new_data;
    this->width = new_w;
    this->height = new_h;
}

