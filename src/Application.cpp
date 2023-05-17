#include <iostream>

#include "Application.hpp"

Application::Application(const std::string& name) :
    m_name(name)
{
    std::cout << "App '" << name << "' created !" << std::endl;
}

Application::~Application()
{
}
