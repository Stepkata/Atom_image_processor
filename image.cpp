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
    }
    else
        printf("Reading failure! %s \n", filename);
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

bool Image::write(const char* filename){ //@TODO: przepisać jako template
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
    set_size();
    this->data = new uint8_t [size];
    memcpy(this->data, other.data, this->size);
}

void Image::free(){
    if (data)
        stbi_image_free(data); //funkcja free z bibloteki
}

ImageType Image::get_file_type(const char* filename){ //@TODO: zrobić to poprawnie

    //std::string ext = std::string(filename, strlen(filename));
    //ext = ext.substr(ext.find("."), ext.length());
    const char* ext = strrchr(filename, '.');
    if (ext == nullptr)
        throw MyException("Nie udało się pobrać formatu!");
    else{
        if (strcmp(ext, ".png"))
            return PNG;
        else if (strcmp(ext, ".jpg"))
            return JPG;
        else if (strcmp(ext, ".bmp"))
            return BMP;
        else if (strcmp(ext, ".tga"))
            return TGA;
        else
            throw MyException("Nieznany format!");
    }
}

