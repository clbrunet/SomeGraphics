#pragma once

#include <string>

class Application {
public:
    Application(const std::string& name);
    ~Application();
private:
    std::string m_name;
};
