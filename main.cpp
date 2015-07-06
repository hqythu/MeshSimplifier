#include <iostream>
#include <string>

#include "Simplifier.h"


using std::string;


int main(int argc, char** argv)
{
    Simplifier s;
    if (argc == 1) {
        s.load("fixed.perfect.dragon.100K.0.07.obj");
        s.simplify(0.05);
        s.save("output.obj");
    }
    else {
        string input_file(argv[1]);
        string output_file(argv[2]);
        double ratio = std::atof(argv[3]);
        s.load(input_file);
        s.simplify(ratio);
        s.save(output_file);
    }
    return 0;
}
