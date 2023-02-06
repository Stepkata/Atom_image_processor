#include <iostream>
#include <string>

#include "Processor.h"
#include "Resdiv.h"

int main() {
    Processor test(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\pogkim.jpg)");
    //Font Mono(R"(C:\Users\keste\Downloads\JetBrainsMono-2.242\fonts\ttf\JetBrainsMono-Bold.ttf)", 500);
    //test.overlayText("mlem", Mono, 500, 200, 120, 24, 30, 100); // @TODO: nie działają kolory

    test.purple_chromatic_aberration();
    test.write(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\new21.png)");



    return 0;
}
