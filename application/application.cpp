#include "application.h"

#include <QFileInfo>
#include <QDir>

#include "constants.cpp"

namespace application {

    void Application::init() const
    {
        readConfig();
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

    void Application::readConfig() const
    {
        QString filePath(QDir::toNativeSeparators(QDir::currentPath() + "/" + CFG_FILE_NAME));

        if ( QFileInfo(filePath).exists() ) {
            // read config
        } else {
            *m_ErrorList << "Configuration file is not found.";
        }
    }

} // namespace application
