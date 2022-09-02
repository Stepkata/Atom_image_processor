#include <iostream>
#include <string>

#include "Processor.h"

int main() {
    Processor test("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\test.png");
    test.rotate_right();
    test.write("C:\\Users\\keste\\CLionProjects\\ImageProcessor\\images\\new16.png");

    /*std::string tab[60] = { "A", "b", "c", "d", "A", "b", "c", "d", "A", "b", "c", "d",
                            "A", "b", "c", "d", "A", "b", "c", "d", "A", "b", "c", "d",
                            "A", "b", "c", "d", "A", "b", "c", "d", "A", "b", "c", "d",
                            "A", "b", "c", "d", "A", "b", "c", "d", "A", "b", "c", "d",
                            "A", "b", "c", "d", "A", "b", "c", "d", "A", "b", "c", "d",};
    int h=5, w=3, ch = 4, pom = 0, index = 0, pom2=0;
    int nw = h, nh = w;
    std::string new_tab[60];
    for(int i=0; i<w; i++, pom2+=ch){
        pom = pom2;
        for(int j=0; j<h; j++, pom += w*ch)
            for(int n=0; n<ch; n++, index++){
                new_tab[index] = tab[pom + n];
            }
    }
    index =0;
    for(int i=0; i<nh; i++){
        for(int j=0; j<nw*ch; j++){
            std::cout<<new_tab[index++]<<" ";
        }
        std::cout<<"\n";
    }*/


    return 0;
}
