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
    /**
     * @brief default constructor
     */
    Processor() = default;

    /**
     * @brief constructor creating object from file (inherited)
     * @param filename - the name of the file that will be opened
     */
    explicit Processor(const char* filename):Image(filename){}

    /**
     * @brief constructor that creates empty image (inherited)
     * @param w - width of the image
     * @param h - height of the image
     * @param c - number of channels
     */
    Processor(int w, int h, int c):Image(w, h, c){}

    /**
     * @brief default destructor
     */
    ~Processor()= default;

    /**
     * @brief implementsing multithreading for @_grayscale_avg() function
     * @param r - the size of one "part" of channels per a thread, including the rest of division
     * @return void
     */
    void grayscale_avg();

    /**
     * @brief implements multithreading for @_grayscale_lum() function
     * @param r - the size of one "part" of channels per a thread, including the rest of division
     * @return void
     */
    void grayscale_lum();

    /**
     * @brief applies a color mask over the image - implements multithreading
     * @param r - value of change in the red color, between 0 and 1
     * @param g - value of change in the green color, between 0 and 1
     * @param b - value of change in the blue color, between 0 and 1
     */
    void color_mask(float r, float g, float b);

    /**
     * @brief implements multithreading for @_flip_x()
     */
    void flip_x();

    /**
     * @brief implements multithreading for @_flip_y()
     */
    void flip_y();

    /**
    * @brief funtion that aplies multithreading to @_neon_ca() that adds a filter altering
    * colors and adding a variation of chromatic aberration
    * @note it looks cool
    */
    void neon_chromatic_aberration();

    /**
    * @brief  applies multithreading to @_bca() that adds a variation of chromatic aberration
    * @note it looks cool
    */
    void purple_chromatic_aberration();

    /**
     * @brief implements multithreading to @_crop() - crops image to selected part
     * @param cx - x position of the left up corner of selection
     * @param cy - y position of the left up corner of selection
     * @param cw - width of selection
     * @param ch - height of selection
     */
    void crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);

    /**
     * @brief implements multithreading for @_chromatic_aberration function
     * @param n the offset of chromatic aberration
     */
    void chromatic_aberration(int n);

    /**
     * @brief implements multithreading for @_distortion() - filter that creates corrupted file effect
     * @param r - change in r value
     * @param g - change in r value
     * @param b - change in r value
     */
    void distortion_filter(float r, float g, float b);

    /**
     * @brief implements multithreading for @_overllay - overlays two images on top of each other.
     * The resulting image has the same alpha value as @this object
     * @param image - the image that will be overlaid on top
     * @param x - the x position of the top left corner of the image
     * @param y - the y position of the top left corner of the image
     */
    void overlay(Processor& image, int x, int y);

    /**
     * @brief fuses multiple images together (adjusts height and width to the smallest ones)
     * @TODO: multithreading
     * @param filenames - list of images to fuse together
     * @return this
     */
    Processor& fuse(const std::vector<const char*>& filenames);

    /**
     * @brief function that rotates the image 90 degrees right using matrix manipulation and copy operation
     * @TODO multithreading + lepszy sposób
     * @return this
     */
    Processor& rotate_right();

    /**
     * @brief function that resizes the image using stbir library function
     * @note alpha channel might cause issues
     * @TODO: zrobić wersję z alphą
     * @param new_w
     * @param new_h
     */
    void resize(int new_w, int new_h);

    /**
     * @brief calculates convolution matrix based on fHue value and implements multithreading for @_change_hue() that
     * handles hue shift
     * @param fHue - value of the hue shift, any value possible - treated like trigonometric degrees
     */
    void change_hue(float fHue);

    /**
     * @brief implements multithreading for @_overlayText() - inserts text into image
     * @param txt - the text that will be pasted
     * @param font - chosen font of the text
     * @param x - x position of the top right corner of the text
     * @param y - y position of the top right corner of the text
     * @param r - r value of the color of the text
     * @param g - g value of the color of the text
     * @param b - b value of the color of the text
     * @param a - alpha value of the text
     */
    void overlayText(const char* txt, const Font& font, int x, int y, uint8_t r = 0, uint8_t g = 0,
                           uint8_t b = 0, uint8_t a = 100);

    /**
     * @brief implements multithreading for @_change_saturation() function, changes saturation of colors in the image
     * @param change - the amount of shift in the saturation, must be between 0 and 1
     */
    void change_saturation(float change);

    /**
     * @brief implementing multithreading, cuts an image into separate images:
     * 1. vertically, to a number of specified parts (vertical = true, to_parts = true, n= number of parts)
     * 2. vertically, to specified size of a part (vertical = true, to_parts = false, n=height of a new part)
     * 3. horizontally, to a number of specified parts (vertical = false, to_parts = true, n= number of parts)
     * 4. horizontally, to specified size of a part (vertical = false, to_parts = false, n=weight of a new part)
     * saves the new images automatically
     * @param vertical - bool, is the image supposed to be cut vertially
     * @param to_parts - bool, is the iage supposed to be cut into a number or sized parts
     * @param n - number or size of parts
     */
    void cut(bool vertical, bool to_parts, int n);

    /**
     * @brief = operator overload
     * @param other
     * @return
     */
    Processor& operator=(Processor *other){
        if (this != other)
            copy(*other);
        return *this;
    }

    /**
     * @brief += operator overload
     * @param other - the image that will be added to @this
     * @return this
     */
    Processor& operator+=(Processor& other);

private:
    /**
     * @brief function that changes a portion of image into grayscale using rgb average
     * @param start place in the data where the function starts
     * @param end place in the data where the function ends
     * @return this
     */
    Processor& _grayscale_avg(int start, int end);

    /**
     * @brief function that changes a portion of image into grayscale using weighted rgb average - produces higher quality
     * images than @grayscale_avg()
     * @param start place in the data where the function starts
     * @param end place in the data where the function ends
     * @return this
     */
    Processor& _grayscale_lum(int start, int end);

    /**
     * @brief applies changes to colors of a part of the image creating a color mask
     * @param start - the point in data where the function starts
     * @param end  - the point in data where the function ends
     * @param change - array containing values of changes in the rgb
     * @return this
     */
    Processor& _color_mask(int start, int end, const float change[]);

    /**
    * @brief function that flips parts of the image vertically
    * @param start - the point in data where the function starts
    * @param end  - the point in data where the function ends
    * @return
    */
    Processor& _flip_x(int start, int end);

    /**
     * @brief function that flips parts of the image horisontally
     * @param start - the point in data where the function starts
     * @param end  - the point in data where the function ends
     * @return this
     */
    Processor& _flip_y(int start, int end);

    /**
     * @brief adds a filter altering colors and adding a variation of chromatic aberration
     * @param start - the point in data where the function starts
     * @param end - the point in data where the function ends
     * @return this
     */
    Processor& _neon_ca(int start, int end);

    /**
     * @brief adds a variation of chromatic aberration
     * @param start - the point in data where the function starts
     * @param end - the point in data where the function ends
     * @return this
     */
    Processor& _pca(int start, int end);

    /**
     * @brief copies part of the "selection" to temporary object
     * @param other - image that holds selected data (later copied to @this)
     * @param cx - x position of the left up corner of selection
     * @param cy - y position of the left up corner of selection
     * @param cw - width of selection
     * @param start - the point in data where the function starts
     * @param end - the point in data where the function ends
     */
    void _crop(Processor& other, uint16_t  cx, uint16_t cy, uint16_t cw, int start, int end);

    /**
     * @braief function applying chromatic aberration in part of the image
     * @param start - the point in data where the function starts
     * @param end  - the point in data where the function ends
     * @param n - offset
     * @return this
     */
    Processor& _chromatic_aberration(int start, int end, int n);

    /**
     * @brief filter that creates corrupted file effect on part of an image
     * @param start - the point in data where the function starts
     * @param end  - the point in data where the function ends
     * @param change - array containing values of changes in the rgb
     * @return this
     */
    Processor& _distortion(int start, int end, const float change[]);

    /**
     * overlays two images on top of each other.
     * The resulting image has the same alpha value as @this object
     * @param image - the image that will be overlaid on top
     * @param x - the x position of the top left corner of the image
     * @param y - the y position of the top left corner of the image
     * @param start - the point in data where the function starts
     * @param end  - the point in data where the function ends
     * @return this
     */
    Processor& _overlay(Processor& image, int x, int y, int start, int end);

    /**
     * clamps the value to fit into [0, 255]
     * @param v - value to be clamped
     * @return
     */
    static uint8_t clamp(float v);

    /**
     * @brief uses the convolution matrix to calculate hue shift for the rgb values
     * @param start place in the data where the function starts
     * @param end place in the data where the function ends
     * @param matrix matrix used to calculate shift
     * @return this
     */
    Processor& _change_hue(int start, int end, float matrix[3][3]);

    /**
     * @brief inserts a part of text into image
     * @param txt - the full text
     * @param font - the font of the text
     * @param color - color of the text (includes alpha)
     * @param x - the x position of the top left corner of the text
     * @param y - the y position of the top left corner of the text
     * @param  start place in the data where the function starts
     * @param end place in the data where the function ends
     * @return this
     */
    Processor& _overlayText(const char* txt, const Font& font, int* color, int x, int y, int start, int end);

    /**
     * @brief changes saturation of a part of the image using convolution matrix
     * @param start place in the data where the function starts
     * @param end place in the data where the function ends
     * @param change the amount of shift in the saturation
     * @return
     */
    Processor& _change_saturation(int start, int end, float change);

    /**
     * @brief creates new images from a part of the image vertically
     * @param x - height of the new image
     *  @param start - place in the data where the function starts
     * @param end - place in the data where the function ends
     * @param stop - control variable handling uneven division of the original height
     */
    void _cut(int x, int start, int end, int stop);

    /**
     * @brief creates new images from a part of the image horizontally
     * @param x - width of the new image
     *  @param start - place in the data where the function starts
     * @param end - place in the data where the function ends
     * @param stop - control variable handling uneven division of the original width
     */
    void _cut_h(int x, int start, int end, int stop);

    /**
     * returns the number of "channel packages" that are in the image data
     * @return
     */
    size_t num_channels(){
        return (size_t)size/channels;
    }

    static std::atomic<unsigned> num_cut;
};


#endif //IMAGEPROCESSOR_PROCESSOR_H
