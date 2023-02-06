#define STB_IMAGE_RESIZE_IMPLEMENTATION


#include "Processor.h"
#include "stb_image_resize.h"
#include "Resdiv.h"

#include <iostream>

#define BYTE_BOUND(value) value < 0? 0: (value<255? 255:value)

//@TODO: zamienić niektóre lambdy na funkcje, naprawić multithreading

int Processor::num_cut = 0;

/**
 * @brief interface function implementing multithreading for @_grayscale_avg() function
 * @param r - the size of one "part" of channels per a thread, including the rest of division
 * @return void
 */
void Processor::grayscale_avg() {
    if (channels <3){
    printf("Too little channels, the image is already grayscale!");
    }
    else
    {
        Resdiv r(num_channels(), THREADS);

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_grayscale_avg, this, i * channels * r.c,
                    (i + 1) * channels * r.c + (i == THREADS - 1 ? r.r * channels : 0)
            );

        }
        for (auto &t: threads) {
            t.join();
        }
    }
}


/**
 * @brief function that changes a portion of image into grayscale using rgb average
 * @param start place in the data where the function starts
 * @param end place in the data where the function ends
 * @return this
 */
Processor& Processor::_grayscale_avg(int start, int end){

    for(int i =start; i<end; i+=channels){
        int gray = (data[i]+data[i+1]+data[i+2])/3;
        memset(data+i, gray, 3); //Ustawia pierwsze num bajtów bloku wskazywanego przez ptr na value (traktowane jako unsigned char).
    }

    return *this;
}

/**
 * @brief interface function implementing multithreading for @_grayscale_lum() function
 * @param r - the size of one "part" of channels per a thread, including the rest of division
 * @return void
 */
void Processor::grayscale_lum() {
    if (channels <3){
        printf("Too little channels, the image is already grayscale!");
    }
    else
    {
        Resdiv r(num_channels(), THREADS);

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_grayscale_lum, this, i * channels * r.c,
                    (i + 1) * channels * r.c + (i == THREADS - 1 ? r.r * channels : 0)
            );

        }
        for (auto &t: threads) {
            t.join();
        }
    }
}


/**
 * @brief function that changes a portion of image into grayscale using weighted rgb average - produces higher quality
 * images than @grayscale_avg()
 * @param start place in the data where the function starts
 * @param end place in the data where the function ends
 * @return this
 */
Processor& Processor::_grayscale_lum(int start, int end){
    for(int i =start; i<end; i+=channels){
        int gray = 0.2126*data[i]+0.7152*data[i+1]+0.0722*data[i+2];
        memset(data+i, gray, 3); //Ustawia pierwsze num bajtów bloku wskazywanego przez ptr na value (traktowane jako unsigned char).
    }

    return *this;

}


/**
 * @brief applies a color mask over the image - interface function implementing multithreading
 * @param r - value of change in the red color, between 0 and 1
 * @param g - value of change in the green color, between 0 and 1
 * @param b - value of change in the blue color, between 0 and 1
 */
void Processor::color_mask(float r, float g, float b){
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else if (r > 1 || g > 1 || b > 1){
        printf("Invalid input");
    }
    else
    {
        Resdiv re(num_channels(), THREADS);
        float change[] = {r, g, b};

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_color_mask, this, i * channels * re.c,
                    (i + 1) * channels * re.c + (i == THREADS - 1 ? re.r * channels : 0),
                    change
            );

        }
        for (auto &t: threads) {
            t.join();
        }
    }

}


/**
 * @brief applies changes to colors of a part of the image creating a color mask
 * @param start - the point in data where the function starts
 * @param end  - the point in data where the function ends
 * @param change - array containing values of changes in the rgb
 * @return this
 */
Processor& Processor::_color_mask(int start, int end, const float change[]) {
    for (int i=start; i<end; i+=channels){
        data[i] *= change[0];
        data[i+1] *= change[1];
        data[i+2] *= change[2];
    }

    return *this;
}

//@TODO
void Processor::cut(int n) {
    _cut(true, n);
}

//@TODO: usunąć zduplikowany kod

/**
 * @brief interface function that implements multithreading for @_flip_x()
 */
void Processor::flip_x() {
    Resdiv re(this->height, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_flip_x, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r * channels : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief function that flips parts of the image vertically
 * @param start - the point in data where the function starts
 * @param end  - the point in data where the function ends
 * @return
 */
Processor& Processor::_flip_x(int start, int end) {
    uint8_t pom[this->channels];
    uint8_t *ptr1, *ptr2;

    for (int y=start; y<end; ++y){
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

/**
 * @brief interface function that implements multithreading for @_flip_y()
 */
void Processor::flip_y() {
    Resdiv re(width, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_flip_y, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r * channels : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief function that flips parts of the image horisontally
 * @param start - the point in data where the function starts
 * @param end  - the point in data where the function ends
 * @return this
 */
Processor& Processor::_flip_y(int start, int end) { //odbicie względem osi y
    uint8_t *ptr1, *ptr2;
    uint8_t pom[this->channels];

    for (int x=start; x<end; ++x){
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



/**
 * @brief funtion that aplies multithreading to @_neon_ca() that adds a filter altering
 * colors and adding a variation of chromatic aberration
 * @note it looks cool
 */
void Processor::neon_chromatic_aberration() { //dodaje chromatic abberration ale też zmienia kolory, w każdym razie wygląda fajnie
    Resdiv re(this->height, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_neon_ca, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r * channels : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief funtion that adds a filter altering colors and adding a variation of chromatic aberration
 * @param start - the point in data where the function starts
 * @param end - the point in data where the function ends
 * @return this
 */
Processor& Processor::_neon_ca(int start, int end){
    for (int y=start; y<end; ++y){
        for(int x=0; x< this->width; x++){
            std::swap(this->data[(x +y*this->width)*channels+5], this->data[(x +y*this->width)*channels+30]);
        }
    }
    return *this;
}

/**
 * @brief funtion that aplies multithreading to @_bca() that adds a variation of chromatic aberration
 * @note it looks cool
 */
void Processor::purple_chromatic_aberration() {
    Resdiv re(this->height, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_pca, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r * channels : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief funtion that adds a variation of chromatic aberration
 * @param start - the point in data where the function starts
 * @param end - the point in data where the function ends
 * @return this
 */
Processor& Processor::_pca(int start, int end){
    for (int y=start; y<end; ++y){
        for(int x=0; x< this->width; x++){
            std::swap(this->data[(x +y*this->width)*channels+2], this->data[(x +y*this->width)*channels+20]);
        }
    }
    return *this;
}

/*
 * function that crops the image by performing copy operation
 */
Processor &Processor::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch){
    auto *cropped_data = new uint8_t [cw*ch*this->channels];
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

/**
 * @brief interface function implementing multithreading for @_chromatic_aberration function
 * @param n the offset of chromatic aberration
 */
void Processor::chromatic_aberration(int n) {
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a chromatic aberration filter!");
    }
    else
    {
        Resdiv re(num_channels(), THREADS);

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_chromatic_aberration, this, i * channels * re.c,
                    (i + 1) * channels * re.c + (i == THREADS - 1 ? re.r * channels : 0),
                    n
            );

        }
        for (auto &t: threads) {
            t.join();
        }
    }
}

/**
 * @braief function applying chromatic aberration in part of the image
 * @param start - the point in data where the function starts
 * @param end  - the point in data where the function ends
 * @param n - offset
 * @return this
 */
Processor& Processor::_chromatic_aberration(int start, int end, int n){
    for (int i=start; i<end; i+=channels) {
        if (i + n * channels < size)
            data[i] = data[i + n * channels];
    }
    return *this;
}

/*
 * @brief function that uses corruption-like filter
 */
Processor &Processor::distortion_filter(float r, float g, float b) { //@TODO: naprawić multithreading
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else{
        auto functor0 = [this](float change){
            for (int i=0; i<size; i+=channels){
                uint8_t f = data[i], s = data[i+1], t = data[i+2];
                data[i] = data[i]==255?data[i]: f*change;

            }
        };
        auto functor1 = [this](float change){
            for (int i=0; i<size; i+=channels){
                uint8_t f = data[i], s = data[i+1], t = data[i+2];
                data[i+1] = data[i+1]==255? data[i+1]: s*change;
            }
        };
        auto functor2 = [this](float change){
            for (int i=0; i<size; i+=channels){
                uint8_t f = data[i], s = data[i+1], t = data[i+2];
                data[i+2] = data[i+2]==255? data[i+2]: t*change;
            }
        };

        std::thread t0(functor0, r);
        std::thread t1(functor1, g);
        std::thread t2(functor2, b);

        t0.join();
        t1.join();
        t2.join();

        /*for (int i=0; i<size; i+=channels){
            uint8_t f = data[i], s = data[i+1], t = data[i+2];
            data[i] = data[i]==255?data[i]: f*r;
            data[i+1] = data[i+1]==255? data[i+1]: s*g;
            data[i+2] = data[i+2]==255? data[i+2]: t*b;
        }*/
    }
    return *this;
}

void Processor::cut_to_form(int n) {
    _cut(false, n);
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
        std::string nazwa = "C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\cut_" + std::to_string(num_cut) + ".png";
        num_cut++;
        nowy.write(nazwa.c_str());
        pom += new_height;

    }

}

/*
 * @brief funtion that overlays one image over the other
 */
Processor &Processor::overlay(Processor& image, int x, int y) { // @TODO; multithreading
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


/*
 * @brief += operator overload
 */
Processor& Processor::operator+=(Processor& other){ //@TODO: multithreading
    if (this!= &other){
        if (this->channels != other.get_channels())
            throw MyException("Incopatibile channels!");

        if(this->width < other.get_width()) //muszą być tej samej szerokości
            other.crop(0, 0, this->width, other.get_height());
        else if(this->width > other.get_width())
            this->crop(0, 0, other.get_width(), this->height);

        int new_height = this->height+other.get_height();
        int new_size = this->size + other.get_size();

        auto *new_data = new uint8_t [new_size];
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

/*
 * @brief function that creates a fusion out of a vector of images using addition of Processor objects
 */
Processor &Processor::fuse(const std::vector<const char*>& filenames) {
    for (auto name : filenames){
        Processor other(name);
        *this+=other;
    }
    return *this;
}

/*
 * @brief function that rotates the image 90 degrees right using matrix manipulation and copy operation
 */
Processor &Processor::rotate_right() { //@TODO: multithreading
    auto *new_data = new uint8_t [this->size];
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


        std::string nazwa = R"(C:\Users\keste\CLionProjects\ImageProcessor\images\cut_hor_)" + std::to_string(num_cut) + ".png";
        num_cut++;
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

/**
 * @brief function that resizes the image using stbir library function
 * @note alpha channel might cause issues
 * @param new_w
 * @param new_h
 */
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

uint8_t Processor::clamp(float v) {
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return (uint8_t)v;
}

Processor& Processor::change_hue(float fHue) {
    float cosA = cos(fHue*3.14159265f/180); //convert degrees to radians
    float sinA = sin(fHue*3.14159265f/180); //convert degrees to radians
    //calculate the rotation matrix, only depends on Hue
    float matrix[3][3] = {{cosA + (1.0f - cosA) / 3.0f, 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA},
                          {1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f*(1.0f - cosA), 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA},
                          {1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f * (1.0f - cosA)}};
    //Use the rotation matrix to convert the RGB directly

    auto functor0 = [this](float matrix[3][3]){
        for(int i=0; i<this->size; i+=channels){
            data[i] = clamp(data[i]*matrix[0][0] + data[i+1]*matrix[0][1] + data[i+2]*matrix[0][2]);
        }
    };
    auto functor1 = [this](float matrix[3][3]){
        for(int i=0; i<this->size; i+=channels){
            data[i+1]= clamp(data[i]*matrix[1][0] + data[i+1]*matrix[1][1] + data[i+2]*matrix[1][2]);
        }
    };
    auto functor2 = [this](float matrix[3][3]){
        for(int i=0; i<this->size; i+=channels){
            data[i+2] = clamp(data[i]*matrix[2][0] + data[i+1]*matrix[2][1] + data[i+2]*matrix[2][2]);
        }
    };

    std::thread t0(functor0, matrix);
    std::thread t1(functor1, matrix);
    std::thread t2(functor2, matrix);

    t0.join();
    t1.join();
    t2.join();


    /*for(int i=0; i<this->size; i+=channels){
        data[i] = clamp(data[i]*matrix[0][0] + data[i+1]*matrix[0][1] + data[i+2]*matrix[0][2]);
        data[i+1]= clamp(data[i]*matrix[1][0] + data[i+1]*matrix[1][1] + data[i+2]*matrix[1][2]);
        data[i+2] = clamp(data[i]*matrix[2][0] + data[i+1]*matrix[2][1] + data[i+2]*matrix[2][2]);
    }*/

    return *this;
}

Processor& Processor::overlayText(const char *txt, const Font &font, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    size_t len = strlen(txt);
    SFT_Char c{};
    int32_t dx, dy;
    uint8_t* dstPx;
    uint8_t srcPx;
    uint8_t color[4] = {r, g, b, a};

    for(size_t i = 0;i < len;++i) {
        if(sft_char(&font.sft, txt[i], &c) != 0) {
            throw MyException("Font is missing this character!");
        }

        for(uint16_t sy = 0;sy < c.height;++sy) {
            dy = sy + y + c.y;
            if(dy < 0) {
                continue;
            }
            else if(dy >= height) {
                break;
            }
            for(uint16_t sx = 0;sx < c.width;++sx) {
                dx = sx + x + c.x;
                if(dx < 0) {
                    continue;
                }
                else if(dx >= width) {
                    break;
                }
                dstPx = &data[(dx + dy * width) * channels];
                srcPx = c.image[sx + sy * c.width];

                if(srcPx != 0) {
                    float srcAlpha = (srcPx / 255.f) * (a / 255.f);
                    float dstAlpha = channels < 4 ? 1 : dstPx[3] / 255.f;
                    if(srcAlpha > .99 && dstAlpha > .99) {
                        memcpy(dstPx, color, channels);
                    }
                    else {
                        float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
                        if(outAlpha < .01) {
                            memset(dstPx, 0, channels);
                        }
                        else {
                            for(int chnl = 0;chnl < channels;++chnl) {
                                dstPx[chnl] = (uint8_t)BYTE_BOUND((color[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
                            }
                            if(channels > 3) {dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f);}
                        }
                    }
                }
            }
        }

        x += c.advance;
        delete c.image;
    }
    return *this;
}

Processor &Processor::change_saturation(float change) {
    change = change>1 ? 1 : change;
    change = change<0 ? 0:change; //make sure change is a legal amount


    for(int i=0; i<this->size; i+=channels){
        float  P=sqrt(
                data[i]*data[i]*Pr+
                data[i+1]*data[i+1]*Pg+
                data[i+2]*data[i+2]*Pb ) ;
        data[i] = P+(data[i]-P)*change;
        data[i+1]= P+(data[i+1]-P)*change;
        data[i+2] = P+(data[i+2]-P)*change;
    }

    return *this;
}

