#include "virtualdirectory.h"
#include <utility/helpfunctions.h>

namespace generator {

    /**
     * @brief VirtualDirectory::VirtualDirectory
     */
    VirtualDirectory::VirtualDirectory()
        : VirtualFileSystemAbstractItem()
    {
    }

    /**
     * @brief VirtualDirectory::VirtualDirectory
     * @param src
     */
    VirtualDirectory::VirtualDirectory(const VirtualDirectory &src)
        : VirtualFileSystemAbstractItem(src)
    {
        copyFrom(src);
    }

    /**
     * @brief VirtualDirectory::VirtualDirectory
     * @param src
     */
    VirtualDirectory::VirtualDirectory(VirtualDirectory &&src)
        : VirtualFileSystemAbstractItem(src)
    {
        moveFrom(src);
    }

    /**
     * @brief VirtualDirectory::VirtualDirectory
     * @param path
     */
    VirtualDirectory::VirtualDirectory(const QString &path)
        : VirtualFileSystemAbstractItem(path)
    {
    }

    /**
     * @brief VirtualDirectory::operator =
     * @param rhs
     * @return
     */
    VirtualDirectory &VirtualDirectory::operator =(VirtualDirectory rhs)
    {
        VirtualFileSystemAbstractItem::operator =(rhs);
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief VirtualDirectory::operator =
     * @param rhs
     * @return
     */
    VirtualDirectory &VirtualDirectory::operator =(VirtualDirectory &&rhs)
    {
        if (this != &rhs) {
            VirtualFileSystemAbstractItem::operator =(rhs);
            moveFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief VirtualDirectory::addFile
     * @param fileName
     * @return
     */
    SharedVirtualFile VirtualDirectory::addFile(const QString &fileName)
    {
        return addItem<VirtualFile>(fileName);
    }

    /**
     * @brief VirtualDirectory::getFile
     * @param fileName
     * @return
     */
    SharedVirtualFile VirtualDirectory::getFile(const QString &fileName)
    {
        return getItem<VirtualFile>(fileName);
    }

    /**
     * @brief VirtualDirectory::addDirectory
     * @param directoryName
     * @return
     */
    SharedVirtualDirectory VirtualDirectory::addDirectory(const QString &directoryName)
    {
        return addItem<VirtualDirectory>(directoryName);
    }

    /**
     * @brief VirtualDirectory::getDirectory
     * @param directoryName
     * @return
     */
    SharedVirtualDirectory VirtualDirectory::getDirectory(const QString &directoryName)
    {
        return getItem<VirtualDirectory>(directoryName);
    }

    /**
     * @brief VirtualDirectory::constainsDirectory
     * @param name
     * @return
     */
    bool VirtualDirectory::constainsDirectory(const QString &name) const
    {
        return m_Files.contains(name) && std::dynamic_pointer_cast<VirtualDirectory>(*m_Files.find(name));
    }

    /**
     * @brief VirtualDirectory::removeNestedItem
     * @param name
     * @return
     */
    bool VirtualDirectory::removeNestedItem(const QString &name)
    {
        return m_Files.remove(name);
    }

    /**
     * @brief VirtualDirectory::clearVirtualStructure
     */
    void VirtualDirectory::clearVirtualStructure()
    {
        m_Files.clear();
    }

    /**
     * @brief VirtualDirectory::write
     */
    void VirtualDirectory::write() const
    {
        QDir().mkpath(m_FileInfo.filePath());

        if (m_FileInfo.isWritable())
            for (auto &&file : m_Files.values())
                file->write();
        else if (m_ErrorList)
            *m_ErrorList << QObject::tr("%1 is not writable.").arg(m_FileInfo.filePath());
    }

    /**
     * @brief VirtualDirectory::remove
     * @return
     */
    bool VirtualDirectory::remove() const
    {
        bool result(QDir(m_FileInfo.filePath()).removeRecursively());

        if (!result && m_ErrorList)
            *m_ErrorList << QObject::tr("Cannot delete %1.").arg(m_FileInfo.filePath());

        return result;
    }

    /**
     * @brief VirtualDirectory::moveFrom
     * @param src
     */
    void VirtualDirectory::moveFrom(VirtualDirectory &src)
    {
        m_Files = std::move(src.m_Files);
    }

    /**
     * @brief VirtualDirectory::copyFrom
     * @param src
     */
    void VirtualDirectory::copyFrom(const VirtualDirectory &src)
    {
        utility::deepCopySharedPointerHash(src.m_Files, m_Files, &VirtualFileSystemAbstractItem::name);
    }

    /**
     * @brief VirtualDirectory::constainsFile
     * @param name
     * @return
     */
    bool VirtualDirectory::constainsFile(const QString &name) const
    {
        return m_Files.contains(name) && !std::dynamic_pointer_cast<VirtualDirectory>(*m_Files.find(name));
    }

} // namespace generator
