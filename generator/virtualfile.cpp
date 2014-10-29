#include "virtualfile.h"

#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace generator {

    /**
     * @brief VirtualFile::VirtualFile
     */
    VirtualFile::VirtualFile()
        : VirtualFileSystemAbstractItem()
    {
    }

    /**
     * @brief VirtualFile::VirtualFile
     * @param path
     */
    VirtualFile::VirtualFile(const QString &path)
        : VirtualFileSystemAbstractItem(path)
    {
    }

    /**
     * @brief VirtualFile::data
     * @return
     */
    QString VirtualFile::data() const
    {
        return m_Data;
    }

    /**
     * @brief VirtualFile::setData
     * @param data
     */
    void VirtualFile::setData(const QString &data)
    {
        m_Data = data;
    }

    /**
     * @brief VirtualFile::appendData
     * @param data
     * @param sep
     * @return
     */
    VirtualFile &VirtualFile::appendData(const QString &data, const QString &sep)
    {
        m_Data.append(sep).append(data);
        return *this;
    }

    /**
     * @brief VirtualFile::prependData
     * @param data
     * @param sep
     * @return
     */
    VirtualFile &VirtualFile::prependData(const QString &data, const QString &sep)
    {
        m_Data.prepend(sep).prepend(data);
        return *this;
    }

    /**
     * @brief VirtualFile::write
     */
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

    /**
     * @brief VirtualFile::remove
     * @return
     */
    bool VirtualFile::remove() const
    {
        bool result(QFile::remove(m_FileInfo.filePath()));

        if (!result && m_ErrorList)
            *m_ErrorList << QObject::tr("Cannot delete file %1.").arg(m_FileInfo.filePath());

        return result;
    }

} // namespace generator
