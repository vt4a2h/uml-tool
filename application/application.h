#pragma once

#include "types.h"

namespace application {

    class Application
    {
    public:
        Application(const Application &) = delete;
        Application(Application &&) = delete;

        Application& operator= (const Application &)  = delete;
        Application& operator= (Application &&)  = delete;

        void init() const;

        bool hasErrors() const;
        SharedErrorList errors() const;

        static const Application& instance()
        {
            static Application singleInstance;
            return singleInstance;
        }

    private:
        Application();
        void readConfig() const;

        mutable SharedErrorList m_ErrorList;
    };

} // namespace application
