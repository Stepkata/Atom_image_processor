#include <iostream>

#include "Processor.h"

int main() {
    Processor test("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\pogkim.jpg");
    test.resize(400, 500);
    test.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\new15.png");


    /*Image copy = test;
    for(int i=copy.get_width()*copy.get_channels()*30; i<copy.get_width()*copy.get_channels()*50; ++i){
        copy.set_data(i, 0);
    }
    copy.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\copy_test.png");

    Image blank(100, 100, 3);
    blank.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\blank.jpg");*/
    return 0;
}
