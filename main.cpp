#include <iostream>

#include "Simplifier.h"


int main()
{
    Simplifier s;
    s.load("fixed.perfect.dragon.100K.0.07.obj");
    s.simplify(0.005);
    s.save("output.obj");
    return 0;
}
