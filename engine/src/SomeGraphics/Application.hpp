#pragma once

#include <string>

namespace sg {

class Application {
public:
    Application(const std::string& name);
    virtual ~Application();
protected:
    std::string m_name;
};

}
