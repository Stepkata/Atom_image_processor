#include <iostream>
#include <string>

#include "Processor.h"

int main() {
    Processor test("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\test.jpg");
    Font Mono("C:\\Users\\keste\\Downloads\\JetBrainsMono-2.242\\fonts\\ttf\\JetBrainsMono-Bold.ttf", 50);
    test.overlayText("mlem", Mono, 100, 100); // @TODO: nie działa
    //test.cut_horisontal(4);
    test.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\new17.png");


    return 0;
}
