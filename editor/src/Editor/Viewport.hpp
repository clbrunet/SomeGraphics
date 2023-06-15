#pragma once

#include <memory>

namespace sg {

class Window;
class Renderer;
class FrameBuffer;
class Program;
class VertexArray;
class EditorCamera;

class Viewport {
public:
    Viewport();
    ~Viewport();

    void on_update(const Window& window, float delta_time);
    void on_render(const Renderer& renderer);
private:
    std::unique_ptr<FrameBuffer> m_frame_buffer;
    std::unique_ptr<Program> m_program;
    std::unique_ptr<VertexArray> m_vertex_array;
    std::unique_ptr<EditorCamera> m_camera;
    bool m_is_hovered = false;
};

}
