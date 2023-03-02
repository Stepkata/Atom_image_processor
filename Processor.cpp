#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "Processor.h"
#include "stb_image_resize.h"
#include "Resdiv.h"

#include <iostream>

std::atomic<unsigned> Processor::num_cut = 0;

//@TODO: sprawdzić czy wszystkie funkcje zwracają to co powinny funkcjonalnie zwracać
//@TODO: exception handling

/**
 * @brief interface function implementing multithreading for @_grayscale_avg() function
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
 */
Processor& Processor::_grayscale_lum(int start, int end){
    for(int i =start; i<end; i+=channels){
        auto gray = (int)(0.2126*data[i]+0.7152*data[i+1]+0.0722*data[i+2]);
        memset(data+i, gray, 3); //Ustawia pierwsze num bajtów bloku wskazywanego przez ptr na value (traktowane jako unsigned char).
    }

    return *this;

}

/**
 * @brief applies a color mask over the image - implements multithreading
 */
void Processor::color_mask(float r, float g, float b){
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else if (r > 1 || g > 1 || b > 1 || r<0 || g<0 || b <0){
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
 */
Processor& Processor::_color_mask(int start, int end, const float change[]) {
    for (int i=start; i<end; i+=channels){
        data[i] = (uint8_t)((float)data[i]*change[0]);
        data[i+1] = (uint8_t)((float)data[i+1]*change[1]);
        data[i+2] = (uint8_t)((float)data[i+2]*change[2]);
    }

    return *this;
}


// @TODO: usunąć zduplikowany kod

/**
 * @brief implements multithreading for @_flip_x()
 */
void Processor::flip_x() {
    Resdiv re(this->height, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_flip_x, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief function that flips parts of the image vertically
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
 * @brief implements multithreading for @_flip_y()
 */
void Processor::flip_y() {
    Resdiv re(width, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_flip_y, this, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief function that flips parts of the image horisontally
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
 * @brief adds a filter altering colors and adding a variation of chromatic aberration
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
 * @brief  applies multithreading to @_bca() that adds a variation of chromatic aberration
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
 * @brief adds a variation of chromatic aberration
 */
Processor& Processor::_pca(int start, int end){
    for (int y=start; y<end; ++y){
        for(int x=0; x< this->width; x++){
            std::swap(this->data[(x +y*this->width)*channels+2], this->data[(x +y*this->width)*channels+20]);
        }
    }
    return *this;
}

/**
 * @brief implements multithreading to @_crop() - crops image to selected part
 */
void Processor::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch) {
    Processor nowy(cw, ch, this->channels);

    Resdiv re(ch, THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_crop, this, std::ref(nowy), cx, cy, cw, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r : 0)
        );

    }
    for (auto &t: threads) {
        t.join();
    }
    this->copy(nowy);

}

/**
 * @brief copies part of the "selection" to temporary object
 */
void Processor::_crop(Processor& other, uint16_t cx, uint16_t cy, uint16_t cw, int start, int end){
    for (int y=start; y<end; ++y){
        if(y+cy >= height)
            break;
        for(uint16_t x=0; x<cw; ++x){
            if (x+cx >= width)
                break;
            uint8_t *cropped_data = &other.get_data()[(x+y*cw)*channels];
            memcpy(cropped_data, &data[((x + cx) + (y+cy)*this->width)*channels], channels);
        }
    }
}
/**
 * @brief implements multithreading for @_chromatic_aberration function
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
 */
Processor& Processor::_chromatic_aberration(int start, int end, int n){
    for (int i=start; i<end; i+=channels) {
        if (i + n * channels < size)
            data[i] = data[i + n * channels];
    }
    return *this;
}

/**
 * @brief implements multithreading for @_distortion() - filter that creates corrupted file effect
 */
void Processor::distortion_filter(float r, float g, float b) {
    if (channels <3){
        printf("Too little channels, the image is grayscale and cannot be modified with a color mask!");
    }
    else if ( r<0 || g<0 || b <0){
        printf("Invalid input");
    }
    else
    {
        Resdiv re(num_channels(), THREADS);
        float change[] = {r, g, b};

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_distortion, this, i * channels * re.c,
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
 * @brief filter that creates corrupted file effect on part of an image
 */
Processor& Processor::_distortion(int start, int end, const float change[]) {
    for (int i=start; i<end; i+=channels){
            float f = data[i], s = data[i+1], t = data[i+2];
            data[i] = data[i]==255?data[i]: (uint8_t )(f*change[0]);
            data[i+1] = data[i+1]==255? data[i+1]: (uint8_t )(s*change[1]);
            data[i+2] = data[i+2]==255? data[i+2]: (uint8_t )(t*change[2]);
        }
    return *this;
}

/**
 * @brief implements multithreading for @_overllay - overlays two images on top of each other.
 * The resulting image has the same alpha value as @this object
 */
void Processor::overlay(Processor &image, int x, int y) {
    Resdiv re(image.get_height(), THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_overlay, this, std::ref(image), x, y, i * re.c,
                (i + 1) * re.c + (i == THREADS - 1 ? re.r : 0)
                );

    }
    for (auto &t: threads) {
            t.join();
    }

}

/**
 * overlays two images on top of each other.
 * The resulting image has the same alpha value as @this object
 */
Processor &Processor::_overlay(Processor& image, int x, int y, int start, int end) { // @TODO; multithreading
    uint8_t *source;
    uint8_t *destination;
    for(int sy = start; sy<end; ++sy){
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

            float src_alpha = image.get_channels() < 4 ? 1: (float)source[3]/255.f;
            float dest_alpha = this->channels < 4 ? 1: (float)destination[3]/255.f;

            if(src_alpha ==1){ //source with alpha = 1 or grayscale
                if(image.get_channels() >= this->channels)
                    memcpy(destination, source, channels);
                else
                {
                    memcpy(destination, source, image.get_channels());
                }
            }
            else{
                source[0] = clamp(src_alpha * (float)source[0] + (1 - src_alpha) * (float)destination[0]);
                source[1] = clamp(src_alpha * (float)source[1] + (1 - src_alpha) * (float)destination[1]);
                source[2] = clamp(src_alpha * (float)source[2] + (1 - src_alpha) * (float)destination[2]);
                source[3] = clamp(dest_alpha*255.f); //@TODO: rozważyć Byte bound
                memcpy(destination, source, channels);
            }
        }
    }
    return *this;
}

/**
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
        size_t new_size = this->size + other.get_size();

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

/**
 * @brief function that creates a fusion out of a vector of images using addition of Processor objects
 */
Processor &Processor::fuse(const std::vector<const char*>& filenames) {
    for (auto name : filenames){
        Processor other(name);
        *this+=other;
    }
    return *this;
}

/**
 * @brief function that rotates the image 90 degrees right using matrix manipulation and copy operation
 */
Processor &Processor::rotate_right() {
    auto *new_data = new uint8_t [this->size];
    int new_height = this->width;
    int new_width = this->height;

    int  pom = 0, index = 0;

    for(int i=0; i<this->width; i++){
        pom = i*this->channels;
        for(int j=0; j<this->height; j++, pom += this->width*this->channels)
            for(int n=0; n<this->channels; n++, index++){
                new_data[index] = data[pom + n];
            }
    }

    free();
    this->data = new_data;
    new_data = nullptr;
    this->height = new_height;
    this->width = new_width;
    this->flip_x();
    return *this;
}

/**
 * @brief function that resizes the image using stbir library function
 */
void Processor::resize(int new_w, int new_h) {
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

/**
 * clamps the value to fit into [0, 255]
 */
uint8_t Processor::clamp(float v) {
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return (uint8_t)v;
}

/**
 * @brief calculates convolution matrix based on fHue value and implements multithreading for @_change_hue() that
 * handles hue shift
 */
void Processor::change_hue(float fHue) {
    float cosA = cos(fHue*3.14159265f/180); //convert degrees to radians
    float sinA = sin(fHue*3.14159265f/180); //convert degrees to radians
    //calculate the rotation matrix, only depends on Hue
    float matrix[3][3] = {{cosA + (1.0f - cosA) / 3.0f, 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA},
                          {1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f*(1.0f - cosA), 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA},
                          {1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f * (1.0f - cosA)}};
    //Use the rotation matrix to convert the RGB directly

    Resdiv r(num_channels(), THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_change_hue, this, i * channels * r.c,
                (i + 1) * channels * r.c + (i == THREADS - 1 ? r.r * channels : 0),
                matrix
        );

    }
    for (auto &t: threads) {
        t.join();
    }

}

/**
 * @brief uses the convolution matrix to calculate hue shift for the rgb values
 */
Processor& Processor::_change_hue(int start, int end, float matrix[3][3]) {
    for (int i = start; i < end; i += channels) {
        data[i] = clamp((float)data[i] * matrix[0][0] + (float)data[i + 1] * matrix[0][1] + (float)data[i + 2] * matrix[0][2]);
        data[i + 1] = clamp((float)data[i] * matrix[1][0] + (float)data[i + 1] * matrix[1][1] + (float)data[i + 2] * matrix[1][2]);
        data[i + 2] = clamp((float)data[i] * matrix[2][0] + (float)data[i + 1] * matrix[2][1] + (float)data[i + 2] * matrix[2][2]);
    }
    return *this;
}

/**
 * @brief implements multithreading for @_overlayText() - inserts text into image
 */
void Processor::overlayText(const char *txt, const Font &font, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
   size_t len = strlen(txt);
   int color[4] = {r, g, b, a};
   Resdiv re(len, THREADS);

   std::vector<std::thread> threads;
   for (int i = 0; i < THREADS; i++) {
       threads.emplace_back(
               &Processor::_overlayText, this, txt, std::ref(font), color, x, y, i * re.c,
               (i + 1) * re.c + (i == THREADS - 1 ? re.r : 0)
       );

   }
   for (auto &t: threads) {
       t.join();
   }

}

/**
 * @brief inserts a part of text into image
 */
Processor& Processor::_overlayText(const char* txt, const Font& font, int *color, int x, int y, int start, int end){
   uint8_t* dstPx;
   uint8_t srcPx;
   int32_t dx, dy;
   SFT_Char c{};
   for(int j = start;j< end;++j) {
       if(sft_char(&font.sft, txt[j], &c) != 0) {
           throw MyException("Font is missing this character!");
       }
       x+=j*(int)c.advance;
       for(int sy = 0;sy < c.height;++sy) {
           dy = sy + y + c.y;
           if(dy < 0) {
               continue;
           }
           else if(dy >= height) {
               break;
           }
           for(int sx = 0;sx < c.width;++sx) {
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
                   float srcAlpha = (float)color[3]/100;
                   float dstAlpha = channels < 4 ? 1 : (float)dstPx[3] / 255.f;

                   if(srcAlpha > .99 && dstAlpha > .99) {
                       memcpy(dstPx, color, channels);
                   }
                   else {
                       float outAlpha = (srcAlpha + dstAlpha * (1 - srcAlpha))<1?(srcAlpha + dstAlpha * (1 - srcAlpha)):1;
                       if(outAlpha < .01) {
                           memset(dstPx, 0, channels);
                       }
                       else {
                           for(int chnl = 0;chnl < channels;++chnl) {
                               dstPx[chnl] = clamp(((float)color[chnl]/255.f * srcAlpha + (float)dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
                           }
                           if(channels > 3) {dstPx[3] = clamp(outAlpha * 255.f);}
                       }
                   }
               }
           }
       }

       delete c.image;
   }

   return *this;
}

/**
* @brief implements multithreading for @_change_saturation() function, changes saturation of colors in the image
*/
void Processor::change_saturation(float change) {
    change = change > 1 ? 1 : change;
    change = change < 0 ? 0 : change; //make sure change is a legal amount

    Resdiv r(num_channels(), THREADS);

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(
                &Processor::_change_saturation, this, i * channels * r.c,
                (i + 1) * channels * r.c + (i == THREADS - 1 ? r.r * channels : 0),
                change
        );

    }
    for (auto &t: threads) {
        t.join();
    }
}

/**
 * @brief changes saturation of a part of the image using convolution matrix
 */
Processor &Processor::_change_saturation(int start, int end, float change) {
    for(int i=start; i<end; i+=channels){
        double  P=sqrt(
                data[i]*data[i]*Pr+
                data[i+1]*data[i+1]*Pg+
                data[i+2]*data[i+2]*Pb ) ;
        data[i] = (uint8_t)(P+(data[i]-P)*change);
        data[i+1]= (uint8_t)(P+(data[i+1]-P)*change);
        data[i+2] = (uint8_t)(P+(data[i+2]-P)*change);
    }

    return *this;
}

/**
 * @brief implementing multithreading, cuts an image into separate images
 */
void Processor::cut(bool vertical, bool to_parts, int n) {
    std::vector<std::thread> threads;

    if(vertical){
        int x = to_parts? int(this->height/n): n;
        int m = to_parts? n : int(this->height/n)+1;
        Resdiv r(m, THREADS);

        for (int i = 0; i <THREADS; i++) {
            threads.emplace_back(
                    &Processor::_cut,
                    this,
                    x,
                    i*r.c,
                    (i + 1)* r.c + (i == THREADS - 1 ? r.r : 0),
                    m
            );

        }
        for (auto &t: threads) {
            t.join();
        }

    }
    else{
        int x = to_parts? int(this->width/n): n;
        int m = to_parts? n : int(this->width/n)+1;
        Resdiv r(m, THREADS);
        for (int i = 0; i < THREADS; i++) {
            threads.emplace_back(
                    &Processor::_cut_h,
                    this,
                    x,
                    i * r.c,
                    (i + 1) * r.c + (i == THREADS - 1 ? r.r : 0),
                    m
            );

        }
        for (auto &t: threads) {
            t.join();
        }
    }
}

/**
 * @brief creates new images from a part of the image vertically
 */
void Processor::_cut(int x, int start, int end, int stop) {

    for (int i=start; i<end; i++){
        //skopiuj część obrazka do nowej tablicy
        int new_height = (i==stop-1) ? this->height-i*x : x;
        Image nowy(this->width, new_height, this->channels);
        for (int z = i * x * this->width * this->channels, j = 0;
             z < (i*x+ new_height) * this->width * this->channels; z++, j++)
            nowy.set_data(j, this->data[z]);
        std::string nazwa = R"(C:\Users\keste\CLionProjects\ImageProcessor\images\cut_)" + std::to_string(i) + ".png";
        num_cut++;
        nowy.write(nazwa.c_str());

    }

}

/**
 * @brief creates new images from a part of the image horizontally
 */
void Processor::_cut_h(int x, int start, int end, int stop) {
    for (int i=start; i<end; i++){
        //skopiuj część obrazka do nowej tablicy
        int new_width = (i==stop-1) ? this->width-i*x : x;
        Image nowy(new_width, this->height, this->channels);

        int j=0;
        int pomi = i*x*channels;
        for (int k=0; k<this->height; k++){
            for(int z=pomi; z<new_width*channels+pomi; z++, j++)
                nowy.set_data(j, this->data[z]);
            pomi+=this->width*this->channels;
        }

        std::string nazwa = R"(C:\Users\keste\CLionProjects\ImageProcessor\images\cut_hor_)" + std::to_string(i) + ".png";
        num_cut++;
        nowy.write(nazwa.c_str());
    }
}


