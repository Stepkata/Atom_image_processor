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
    const char* what() const noexcept override {
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
    Image() = default;
    Image(const char* filename);
    Image(int w, int h, int c):width(w), height(h), channels (c){
        set_size();
        this->data = new uint8_t [size];
    }
    Image(const Image& other);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);
    bool write();
    bool _write(const char* filename);

    void set_size(){
        this->size = width*height*channels;
    }

    void set_data(int x, int c){
        data[x] = c;
    }


    void set_width(int w){
        this->width = w;
    }

    void set_height(int h){
        this->height = h;
    }
    void set_channels(int c){
        this->channels = c;
    }

    int get_width() const{
        return this->width;
    }

    int get_height() const{
        return this->height;
    }
    int get_channels() const{
        return this->channels;
    }
    size_t get_size() const{
        return this->size;
    }
    uint8_t* get_data() const{
        return this->data;
    }

    const char* get_filename() const{
        return this->filename;
    }

    Image& operator=(Image const& other){
        if (this != &other)
            copy(other);
        return *this;
    }
protected:
    void copy (const Image& other);
    void free();
    ImageType get_file_type(const char* filename);
protected:
    uint8_t* data = nullptr;
    size_t size =0;
    int width;
    int height;
    int channels;
    int force_chan = 0;
    const char* filename;

};


#endif //IMAGEPROCESSOR_IMAGE_H
