#include <iostream>
#include <string>

#include "Processor.h"

int main() {
    //*
    Processor test(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\pogkim.jpg)");
    //Font Mono(R"(C:\Users\keste\Downloads\JetBrainsMono-2.242\fonts\ttf\JetBrainsMono-Bold.ttf)", 500);
    //test.overlayText("mlem", Mono, 500, 500, 120, 24, 30, 70);
    Processor test2(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\test2.png)");
    test2.resize(500, 1000);
    test2.write(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\new29.png)");//*

    return 0;
}
