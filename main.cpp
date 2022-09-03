#include <iostream>
#include <string>

#include "Processor.h"

int main() {
    Processor test("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\test.jpg");
    test.change_hue(30);
    test.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\new17.png");

    return 0;
}
