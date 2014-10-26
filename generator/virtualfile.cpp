#include "virtualfile.h"

#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace generator {

    VirtualFile::VirtualFile()
        : VirtualFileSystemAbstractItem()
    {
    }

    VirtualFile::VirtualFile(const QString &path)
        : VirtualFileSystemAbstractItem(path)
    {
    }

    QString VirtualFile::data() const
    {
        return m_Data;
    }

    void VirtualFile::setData(const QString &data)
    {
        m_Data = data;
    }

    VirtualFile &VirtualFile::appendData(const QString &data, const QString &sep)
    {
        m_Data.append(sep).append(data);
        return *this;
    }

    VirtualFile &VirtualFile::prependData(const QString &data, const QString &sep)
    {
        m_Data.prepend(sep).prepend(data);
        return *this;
    }

    void VirtualFile::write() const
    {
        if (!valid())
            return;

        QFile file(m_FileInfo.filePath());
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << m_Data;
        } else if (m_ErrorList)
                *m_ErrorList << QObject::tr("Cannot create file %1.").arg(m_FileInfo.filePath());
    }

    bool VirtualFile::remove() const
    {
        bool result(QFile::remove(m_FileInfo.filePath()));

        if (!result && m_ErrorList)
            *m_ErrorList << QObject::tr("Cannot delete file %1.").arg(m_FileInfo.filePath());

        return result;
    }

} // namespace generator
