#pragma once

#include "virtualfilesystemabstractitem.h"
#include "virtualfile.h"

#include <QDir>

namespace generator {

    class VirtualDirectory : public VirtualFileSystemAbstractItem
    {
    public:
        VirtualDirectory();
        VirtualDirectory(const VirtualDirectory &src);
        VirtualDirectory(VirtualDirectory &&src);
        VirtualDirectory(const QString &path);

        VirtualDirectory &operator =(VirtualDirectory rhs);
        VirtualDirectory &operator =(VirtualDirectory &&rhs);

        SharedVirtualFile addFile(const QString &fileName);
        SharedVirtualFile getFile(const QString &fileName);
        bool constainsFile(const QString &name) const;

        SharedVirtualDirectory addDirectory(const QString &directoryName);
        SharedVirtualDirectory getDirectory(const QString &directoryName);
        bool constainsDirectory(const QString &name) const;

        bool removeNestedItem(const QString &name);

        void clearVirtualStructure();

        void write() const override;
        bool remove() const override;

    protected:
        void moveFrom(VirtualDirectory &src);
        void copyFrom(const VirtualDirectory &src);

    private:
        template<class Item>
        std::shared_ptr<Item> addItem(const QString &name)
        {
            QChar sep(QDir::separator());
            QString path(m_FileInfo.filePath().append(sep).append(name));

            std::shared_ptr<Item> result(std::static_pointer_cast<Item>(*m_Files.insert(name, std::make_shared<Item>(path))));
            result->setErrorList(m_ErrorList);

            return result;
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
