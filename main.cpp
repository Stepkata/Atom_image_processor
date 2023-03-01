#include <iostream>
#include <string>

#include "Processor.h"
#include "Resdiv.h"

int main() {
    Processor test(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\new20.png)");
    //Font Mono(R"(C:\Users\keste\Downloads\JetBrainsMono-2.242\fonts\ttf\JetBrainsMono-Bold.ttf)", 500);
    //test.overlayText("mlem", Mono, 500, 500, 120, 24, 30,20); // @TODO: nie działają kolory
    Processor test2(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\test2.png)");
    test.overlay(test2, 200, 200);
    test.write(R"(C:\Users\keste\CLionProjects\ImageProcessor\images\new24.png)");



    return 0;
}
