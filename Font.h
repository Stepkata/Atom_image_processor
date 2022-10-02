//
// Created by keste on 03.09.2022.
//

#ifndef IMAGEPROCESSOR_FONT_H
#define IMAGEPROCESSOR_FONT_H

#include "schrift.h"
#include "image.h"

class Font {
public:
    Font(const char* fontfile, uint16_t size);
    ~Font();
public:
    SFT sft = {NULL, 12, 12, 0, 0, SFT_DOWNWARD_Y|SFT_RENDER_IMAGE };
private:
    void setSize(uint16_t size);
};


#endif //IMAGEPROCESSOR_FONT_H
