#pragma once

namespace sg {

class SceneEntity;

class Properties {
public:
    Properties() = default;
    Properties(Properties&& other) = default;
    Properties(const Properties& other) = default;
    Properties& operator=(Properties&& other) = default;
    Properties& operator=(const Properties& other) = default;
    ~Properties() = default;

    void on_render();
    void on_render(SceneEntity& entity);
};

}
