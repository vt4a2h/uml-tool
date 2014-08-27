#include "virtualfile.h"
#include <QFileInfo>
#include <QTextStream>

namespace generator {

    VirtualFile::VirtualFile()
        : VirtualFile("")
    {
    }

    VirtualFile::VirtualFile(VirtualFile &&src)
    {
        moveFrom(src);
    }

    VirtualFile::VirtualFile(const VirtualFile &src)
    {
        copyFrom(src);
    }

    VirtualFile::VirtualFile(const QString &path)
        : m_FileInfo(path)
    {
    }

    VirtualFile::~VirtualFile()
    {
    }

    VirtualFile &VirtualFile::operator =(VirtualFile &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    VirtualFile &VirtualFile::operator =(VirtualFile rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    QString VirtualFile::path() const
    {
        return m_FileInfo.filePath();
    }

    void VirtualFile::setPath(const QString &path)
    {
        m_FileInfo = path;
    }

    QString VirtualFile::name() const
    {
        return m_FileInfo.fileName();
    }

    QString VirtualFile::baseName() const
    {
        return m_FileInfo.baseName();
    }

    QString VirtualFile::data() const
    {
        return m_Data;
    }

    void VirtualFile::setData(const QString &data)
    {
        m_Data = data;
    }

    void VirtualFile::write() const
    {
        if (!valid())
            return;

        QFile file(m_FileInfo.filePath());
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << m_Data;
        } else {
            if (m_ErrorList)
                *m_ErrorList << QObject::tr("Cannot create file %1.").arg(m_FileInfo.filePath());
        }
    }

    bool VirtualFile::valid() const
    {
        QFileInfo path(m_FileInfo.path());

        if (!path.isWritable()) {
            if (m_ErrorList)
                *m_ErrorList << QObject::tr("%1 is not writable.").arg(path.filePath());
            return false;
        }

        return true;
    }

    SharedErrorList VirtualFile::errorList() const
    {
        return m_ErrorList;
    }

    void VirtualFile::setErrorList(const SharedErrorList &errorList)
    {
        m_ErrorList = errorList;
    }

    void VirtualFile::copyFrom(const VirtualFile &src)
    {
        m_FileInfo  = src.m_FileInfo;
        m_Data      = src.m_Data;
        m_ErrorList = src.m_ErrorList;
    }

    void VirtualFile::moveFrom(VirtualFile &src)
    {
        m_FileInfo  = std::move(src.m_FileInfo);
        m_Data      = std::move(src.m_Data);
        m_ErrorList = std::move(src.m_ErrorList);
    }

} // namespace generator
