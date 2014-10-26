#include "virtualfilesystemabstractitem.h"

#include <QDir>

namespace generator {

    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem()
        : VirtualFileSystemAbstractItem("stub path")
    {
    }

    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(VirtualFileSystemAbstractItem &&src)
    {
        moveFrom(src);
    }

    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(const VirtualFileSystemAbstractItem &src)
    {
        copyFrom(src);
    }

    VirtualFileSystemAbstractItem::VirtualFileSystemAbstractItem(const QString &path)
        : m_FileInfo(path)
    {
        toNativeSeparators();
    }

    VirtualFileSystemAbstractItem::~VirtualFileSystemAbstractItem()
    {
    }

    VirtualFileSystemAbstractItem &VirtualFileSystemAbstractItem::operator =(VirtualFileSystemAbstractItem &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    VirtualFileSystemAbstractItem &VirtualFileSystemAbstractItem::operator =(VirtualFileSystemAbstractItem rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    void VirtualFileSystemAbstractItem::write() const
    {
        // stub
    }

    bool VirtualFileSystemAbstractItem::remove() const
    {
        // stub
        return false;
    }

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

    QString VirtualFileSystemAbstractItem::path() const
    {
        return m_FileInfo.filePath();
    }

    void VirtualFileSystemAbstractItem::setPath(const QString &path)
    {
        m_FileInfo = path;
        toNativeSeparators();
    }

    QString VirtualFileSystemAbstractItem::name() const
    {
        return m_FileInfo.fileName();
    }

    QString VirtualFileSystemAbstractItem::baseName() const
    {
        return m_FileInfo.baseName();
    }

    SharedErrorList VirtualFileSystemAbstractItem::errorList() const
    {
        return m_ErrorList;
    }

    void VirtualFileSystemAbstractItem::setErrorList(const SharedErrorList &errorList)
    {
        m_ErrorList = errorList;
    }

    void VirtualFileSystemAbstractItem::copyFrom(const VirtualFileSystemAbstractItem &src)
    {
        m_FileInfo  = src.m_FileInfo;
        m_ErrorList = src.m_ErrorList;
    }

    void VirtualFileSystemAbstractItem::moveFrom(VirtualFileSystemAbstractItem &src)
    {
        m_FileInfo  = std::move(src.m_FileInfo);
        m_ErrorList = std::move(src.m_ErrorList);
    }

    void VirtualFileSystemAbstractItem::toNativeSeparators()
    {
        if (!m_FileInfo.isNativePath())
            m_FileInfo.setFile(QDir::toNativeSeparators(m_FileInfo.filePath()));
    }

} // namespace generator
