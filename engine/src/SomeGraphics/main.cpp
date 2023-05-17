#include <iostream>

#include "SomeGraphics.hpp"

int main()
{
    std::unique_ptr<sg::Application> app = create_app();
    return 0;
}
