#include "virtualdirectory.h"

namespace generator {

    VirtualDirectory::VirtualDirectory()
        : VirtualDirectory("stub path")
    {
    }

    VirtualDirectory::VirtualDirectory(const QString &path)
        : VirtualFile(path)
    {
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

    bool VirtualDirectory::remove(const QString &name)
    {
        return m_Files.remove(name);
    }

    bool VirtualDirectory::constains(const QString &name) const
    {
        return m_Files.contains(name);
    }

} // namespace generator
