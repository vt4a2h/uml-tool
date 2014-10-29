#include "application.h"

#include <QFileInfo>
#include <QDir>

#include "constants.cpp"

namespace application {

    /**
     * @brief Application::init
     */
    void Application::init() const
    {
        readConfig();
    }

    /**
     * @brief Application::hasErrors
     * @return
     */
    bool Application::hasErrors() const
    {
        return !m_ErrorList->isEmpty();
    }

    /**
     * @brief Application::errors
     * @return
     */
    SharedErrorList Application::errors() const
    {
        return m_ErrorList;
    }

    /**
     * @brief Application::Application
     */
    Application::Application()
        : m_ErrorList(std::make_shared<ErrorList>())
    {
    }

    /**
     * @brief Application::readConfig
     */
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
