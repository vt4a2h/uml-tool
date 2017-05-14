/*****************************************************************************
** 
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
** 
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>. 
**
*****************************************************************************/

#include "virtualfile.h"

#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace Generator {

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
