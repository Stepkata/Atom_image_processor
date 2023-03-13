//
// Created by keste on 19.07.2022.
//

#ifndef IMAGEPROCESSOR_IMAGE_H
#define IMAGEPROCESSOR_IMAGE_H

#include <cstdint>
#include <cstdio>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class MyException: public std::exception {
private:
    std::string message_;
public:
    explicit MyException(std::string  message);
    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};


enum ImageType {
    PNG,
    JPG,
    BMP,
    TGA
};

class Image {
public:
    /**
     * @brief default constructor
     */
    Image() = default;

    /**
     * @brief creates Image object by opening existing image
     * @param filename
     */
    Image(const char* filename);

    /**
     * @brief creates empty Image object of given width, height and number of channels
     * @param w - width of the object
     * @param h - height of the object
     * @param c - number of channels of the object
     */
    Image(int w, int h, int c):width(w), height(h), channels (c){
        set_size();
        this->data = new uint8_t [size];
    }

    /**
     * @brief copy constructor
     * @param other
     */
    Image(const Image& other);

    /**
     * @brief destructor;
     */
    ~Image();

    /**
     * @brief open given file for processing. Returns true if successful
     * @param filename
     * @return
     */
    bool read(const char* filename);

    /**
     * @brief write to file. Returns true if successful
     * @param filename
     * @return
     */
    bool write(const char* filename);

    /**
     * @brief write to _filename. Returns true if successful
     * @return
     */
    bool write();

    /**
     * set the size param of the object
     */
    void set_size(){
        this->size = width*height*channels;
    }

    /**
     * set the value of exactly one element in the data
     * @param x
     * @param c
     */
    void set_data(int x, int c){
        data[x] = c;
    }

    /**
     * set width param
     * @param w
     */
    void set_width(int w){
        this->width = w;
    }

    /**
     * set height param
     * @param h
     */
    void set_height(int h){
        this->height = h;
    }

    /**
     * set channels param
     * @param c
     */
    void set_channels(int c){
        this->channels = c;
    }

    /**
     * get object's width
     * @return
     */
    [[nodiscard]] int get_width() const{
        return this->width;
    }

    /**
     * get object's height
     * @return
     */
    [[nodiscard]] int get_height() const{
        return this->height;
    }

    /**
     * get object's number of channels
     * @return
     */
    [[nodiscard]] int get_channels() const{
        return this->channels;
    }

    /**
     * get the value of the object's alpha channel if exists, else return 1
     * @return
     */
    [[nodiscard]] float get_alpha() const{
        if (this->channels >= 4 && size >= 4)
            return (float)(data[4]/.255);
        else return 1;
    }

    /**
     * @brief get the size of the object
     * @return
     */
    [[nodiscard]] size_t get_size() const{
        return this->size;
    }

    /**
     * @brief get pointer to data of the object
     * @return
     */
    [[nodiscard]] uint8_t* get_data() const{
        return this->data;
    }

    /**
     * @brief get the object's filename
     * @return
     */
    [[nodiscard]] const char* get_filename() const{
        return this->_filename;
    }

    /**
     * @brief = operator overload
     * @param other
     * @return
     */
    Image& operator=(Image const& other){
        if (this != &other)
            copy(other);
        return *this;
    }
protected:
    /**
     * @brief private function for writing to file. Returns true if succesful
     * @param filename
     * @return
     */
    bool _write(const char* filename);
    /**
     * @brief copy data from other object
     * @param other
     */
    void copy (const Image& other);
    /**
     * @brief free the object data
     */
    void free();
    /**
     * @brief get the filetype of given file
     * @param filename
     * @return
     */
    ImageType get_file_type(const char* filename);
protected:
    uint8_t* data = nullptr;
    size_t size =0;
    int width;
    int height;
    int channels;
    int force_chan = 0;
    const char* _filename;

};


#endif //IMAGEPROCESSOR_IMAGE_H
