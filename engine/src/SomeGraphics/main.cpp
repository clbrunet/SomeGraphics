#include "SomeGraphics.hpp"

int main()
{
    std::unique_ptr<sg::Application> app = create_app();
    app->run();
    return 0;
}
