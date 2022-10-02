#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"


MyException::MyException(const std::string& message) : message_(message) {

}


Image::Image(const char* filename){
    if(this->read(filename)){
        printf("Read the %s file succesfully!\n", filename);
        set_size();
        this->filename = filename;
    }
    else
        throw MyException("Reading failure! \n");
}

Image::Image(const Image& other){
    this->copy(other);
}

Image::~Image(){
    this->free();
}

bool Image::read(const char* filename){
    this->data = stbi_load(filename, &width, &height, &channels, 0); //force chan - można wymusić ilość użytych kanałów, 0-bez wymuszania
    return (data != nullptr);
}

bool Image::write(){
    return _write(this->filename);
}

bool Image::write(const char *filename) {
    return _write(filename);
}

bool Image::_write(const char* filename){
    ImageType type = get_file_type(filename);
    int success;
    switch(type){
        case PNG:
            success = stbi_write_png(filename, width, height, channels, data, width*channels);
            break;
        case JPG:
            success = stbi_write_jpg(filename, width, height, channels, data, 100);
            break;
        case BMP:
            success = stbi_write_bmp(filename, width, height, channels, data);
            break;
        case TGA:
            success = stbi_write_tga(filename, width, height, channels, data);
            break;
    }
    return bool(success);
}

void Image::copy(const Image &other){
    free();
    this->width = other.get_width();
    this->height = other.get_height();
    this->channels = other.get_channels();
    this->filename = other.get_filename();
    set_size();
    this->data = new uint8_t [size];
    memcpy(this->data, other.data, this->size);

}

void Image::free(){
    if (data)
        stbi_image_free(data); //funkcja free z bibloteki
}

ImageType Image::get_file_type(const char* filename){ //@TODO: Norton coś marudził?

    fs::path file_path = filename;

    if (file_path.extension() == ".png")
        return PNG;
    else if (file_path.extension() ==".jpg")
        return JPG;
    else if (file_path.extension() == ".bmp")
        return BMP;
    else if (file_path.extension() == ".tga")
        return TGA;
    else
        throw MyException("Nieznany format!");

}


