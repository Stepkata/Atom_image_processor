//
// Created by keste on 03.09.2022.
//
#include "Font.h"

Font::Font(const char *fontfile, uint16_t size) {
    if((sft.font = sft_loadfile(fontfile)) == NULL){
        throw MyException("Font reading failure!");
    }
    setSize(size);
}

void Font::setSize(uint16_t size) {
    sft.xScale = size;
    sft.yScale = size;
}

Font::~Font() {
    sft_freefont(sft.font);
}
