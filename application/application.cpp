#include "application.h"

namespace application {

    void Application::init() const
    {
    }

    bool Application::hasErrors() const
    {
        return !m_ErrorList->isEmpty();
    }

    SharedErrorList Application::errors() const
    {
        return m_ErrorList;
    }

    Application::Application()
        : m_ErrorList(std::make_shared<ErrorList>())
    {
    }

} // namespace application
