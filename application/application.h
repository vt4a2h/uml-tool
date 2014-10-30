#pragma once

#include "types.h"

/**
 * @brief namespace application
 */
namespace application {

    /**
     * @brief The Application class
     */
    class Application
    {
    public:
        Application(const Application &) = delete;
        Application(Application &&) = delete;

        Application& operator= (const Application &)  = delete;
        Application& operator= (Application &&)  = delete;

        void init();

        bool hasErrors() const;
        SharedErrorList errors() const;

        project::SharedProject createProject(const QString &name, const QString &path);
        project::SharedProject findProjectById(const QString &id) const;
        project::SharedProject findProjectByName(const QString &name) const;
        project::ProjectsList projects() const;
        uint projectsCount() const;
        void removeProjectById(const QString &id);
        void removeProjectByName(const QString &name);

        static Application& instance()
        {
            static Application singleInstance;
            return singleInstance;
        }

    private:
        Application();
        void readConfig();

        project::Projects m_Projects;
        SharedErrorList m_ErrorList;
    };

} // namespace application
