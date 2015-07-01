#include <iostream>

#include "Simplifier.h"


int main()
{
    Simplifier s;
    s.load("sphere.obj");
    s.simplify(0.25);
    s.save("output.obj");
    return 0;
}
