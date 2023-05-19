#pragma once

namespace sg {

class Renderer {
public:
    Renderer();
    ~Renderer();
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity);
private:
};

}
