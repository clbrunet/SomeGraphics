#pragma once

#include <memory>

#include "imgui.h"

namespace sg {

class Renderer;
class Window;
class Renderer;
class FrameBuffer;
class Program;
class Model;
class EditorCamera;

class Viewport {
public:
    Viewport(const Renderer& renderer);
    ~Viewport();

    void on_update(const Window& window, float delta_time);
    void on_render(const Renderer& renderer);
private:
    ImVec2 m_dimension = ImVec2(800.0f, 450.0f);
    std::unique_ptr<FrameBuffer> m_frame_buffer;
    std::unique_ptr<EditorCamera> m_camera;
    std::unique_ptr<Program> m_program;
    std::unique_ptr<Model> m_model;
    bool m_is_hovered = false;
};

}
