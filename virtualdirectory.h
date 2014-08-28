#pragma once

#include "virtualfile.h"

#include <QDir>

namespace generator {

    class VirtualDirectory : public VirtualFile
    {
    public:
        // TODO: add move methods
        VirtualDirectory();
        VirtualDirectory(const QString &path);

        SharedVirtualFile addFile(const QString &fileName);
        SharedVirtualFile getFile(const QString &fileName);

        SharedVirtualDirectory addDirectory(const QString &directoryName);
        SharedVirtualDirectory getDirectory(const QString &directoryName);

        bool remove(const QString &name);
        bool constains(const QString &name) const;

    private:
        template<class Item>
        std::shared_ptr<Item> addItem(const QString &name)
        {
            QChar sep(QDir::separator());
            QString path(m_FileInfo.filePath().append(sep).append(name));

            return std::static_pointer_cast<Item>(*m_Files.insert(name, std::make_shared<Item>(path)));
        }

        template<class Item>
        std::shared_ptr<Item> getItem(const QString &name)
        {
            auto it = m_Files.find(name);

            if (it == m_Files.end())
                return nullptr;

            return std::dynamic_pointer_cast<Item>(*it);
        }

        VirtualFiles m_Files;
    };

} // namespace generator
