#include "virtualdirectory.h"
#include <utility/helpfunctions.h>

namespace generator {

    VirtualDirectory::VirtualDirectory()
        : VirtualFileSystemAbstractItem()
    {
    }

    VirtualDirectory::VirtualDirectory(const VirtualDirectory &src)
        : VirtualFileSystemAbstractItem(src)
    {
        copyFrom(src);
    }

    VirtualDirectory::VirtualDirectory(VirtualDirectory &&src)
        : VirtualFileSystemAbstractItem(src)
    {
        moveFrom(src);
    }

    VirtualDirectory::VirtualDirectory(const QString &path)
        : VirtualFileSystemAbstractItem(path)
    {
    }

    VirtualDirectory &VirtualDirectory::operator =(VirtualDirectory rhs)
    {
        VirtualFileSystemAbstractItem::operator =(rhs);
        moveFrom(rhs);

        return *this;
    }

    VirtualDirectory &VirtualDirectory::operator =(VirtualDirectory &&rhs)
    {
        if (this != &rhs) {
            VirtualFileSystemAbstractItem::operator =(rhs);
            moveFrom(rhs);
        }

        return *this;
    }

    SharedVirtualFile VirtualDirectory::addFile(const QString &fileName)
    {
        return addItem<VirtualFile>(fileName);
    }

    SharedVirtualFile VirtualDirectory::getFile(const QString &fileName)
    {
        return getItem<VirtualFile>(fileName);
    }

    SharedVirtualDirectory VirtualDirectory::addDirectory(const QString &directoryName)
    {
        return addItem<VirtualDirectory>(directoryName);
    }

    SharedVirtualDirectory VirtualDirectory::getDirectory(const QString &directoryName)
    {
        return getItem<VirtualDirectory>(directoryName);
    }

    bool VirtualDirectory::constainsDirectory(const QString &name) const
    {
        return m_Files.contains(name) && std::dynamic_pointer_cast<VirtualDirectory>(*m_Files.find(name));
    }

    bool VirtualDirectory::removeNestedItem(const QString &name)
    {
        return m_Files.remove(name);
    }

    void VirtualDirectory::clearVirtualStructure()
    {
        m_Files.clear();
    }

    void VirtualDirectory::write() const
    {
        QDir().mkpath(m_FileInfo.filePath());

        if (m_FileInfo.isWritable())
            for (auto &&file : m_Files.values())
                file->write();
        else if (m_ErrorList)
            *m_ErrorList << QObject::tr("%1 is not writable.").arg(m_FileInfo.filePath());
    }

    bool VirtualDirectory::remove() const
    {
        bool result(QDir(m_FileInfo.filePath()).removeRecursively());

        if (!result && m_ErrorList)
            *m_ErrorList << QObject::tr("Cannot delete %1.").arg(m_FileInfo.filePath());

        return result;
    }

    void VirtualDirectory::moveFrom(VirtualDirectory &src)
    {
        m_Files = std::move(src.m_Files);
    }

    void VirtualDirectory::copyFrom(const VirtualDirectory &src)
    {
        utility::deepCopySharedPointerHash(src.m_Files, m_Files, &VirtualFileSystemAbstractItem::name);
    }

    bool VirtualDirectory::constainsFile(const QString &name) const
    {
        return m_Files.contains(name) && !std::dynamic_pointer_cast<VirtualDirectory>(*m_Files.find(name));
    }

} // namespace generator
