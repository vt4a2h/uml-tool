#include "virtualfilesystemabstractitem.h"

#include <QDir>

namespace generator {

    /**
     * @brief VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem
     */
    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem()
        : VirtualFileSystemAbstractItem("stub path")
    {
    }

    /**
     * @brief VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem
     * @param src
     */
    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(VirtualFileSystemAbstractItem &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem
     * @param src
     */
    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(const VirtualFileSystemAbstractItem &src)
    {
        copyFrom(src);
    }

    /**
     * @brief VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem
     * @param path
     */
    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(const QString &path)
        : m_FileInfo(path)
    {
        toNativeSeparators();
    }

    /**
     * @brief VirtualFileSystemAbstractItem::~VirtualFileSystemAbstractItem
     */
    VirtualFileSystemAbstractItem::~VirtualFileSystemAbstractItem()
    {
    }

    /**
     * @brief VirtualFileSystemAbstractItem::operator =
     * @param rhs
     * @return
     */
    VirtualFileSystemAbstractItem &VirtualFileSystemAbstractItem::operator =(VirtualFileSystemAbstractItem &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::operator =
     * @param rhs
     * @return
     */
    VirtualFileSystemAbstractItem &VirtualFileSystemAbstractItem::operator =(VirtualFileSystemAbstractItem rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::write
     */
    void VirtualFileSystemAbstractItem::write() const
    {
        // stub
    }

    /**
     * @brief VirtualFileSystemAbstractItem::remove
     * @return
     */
    bool VirtualFileSystemAbstractItem::remove() const
    {
        // stub
        return false;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::valid
     * @return
     */
    bool VirtualFileSystemAbstractItem::valid() const
    {
        QFileInfo path(m_FileInfo.path());

        if (!path.isWritable()) {
            if (m_ErrorList)
                *m_ErrorList << QObject::tr("%1 is not writable.").arg(path.filePath());
            return false;
        }

        return true;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::path
     * @return
     */
    QString VirtualFileSystemAbstractItem::path() const
    {
        return m_FileInfo.filePath();
    }

    /**
     * @brief VirtualFileSystemAbstractItem::setPath
     * @param path
     */
    void VirtualFileSystemAbstractItem::setPath(const QString &path)
    {
        m_FileInfo = path;
        toNativeSeparators();
    }

    /**
     * @brief VirtualFileSystemAbstractItem::name
     * @return
     */
    QString VirtualFileSystemAbstractItem::name() const
    {
        return m_FileInfo.fileName();
    }

    /**
     * @brief VirtualFileSystemAbstractItem::baseName
     * @return
     */
    QString VirtualFileSystemAbstractItem::baseName() const
    {
        return m_FileInfo.baseName();
    }

    /**
     * @brief VirtualFileSystemAbstractItem::errorList
     * @return
     */
    SharedErrorList VirtualFileSystemAbstractItem::errorList() const
    {
        return m_ErrorList;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::setErrorList
     * @param errorList
     */
    void VirtualFileSystemAbstractItem::setErrorList(const SharedErrorList &errorList)
    {
        m_ErrorList = errorList;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::copyFrom
     * @param src
     */
    void VirtualFileSystemAbstractItem::copyFrom(const VirtualFileSystemAbstractItem &src)
    {
        m_FileInfo  = src.m_FileInfo;
        m_ErrorList = src.m_ErrorList;
    }

    /**
     * @brief VirtualFileSystemAbstractItem::moveFrom
     * @param src
     */
    void VirtualFileSystemAbstractItem::moveFrom(VirtualFileSystemAbstractItem &src)
    {
        m_FileInfo  = std::move(src.m_FileInfo);
        m_ErrorList = std::move(src.m_ErrorList);
    }

    /**
     * @brief VirtualFileSystemAbstractItem::toNativeSeparators
     */
    void VirtualFileSystemAbstractItem::toNativeSeparators()
    {
        if (!m_FileInfo.isNativePath())
            m_FileInfo.setFile(QDir::toNativeSeparators(m_FileInfo.filePath()));
    }

} // namespace generator
