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

#include <QString>
#include <QFileInfo>

#include "virtualfilesystemabstractitem.h"

namespace generator {

    /**
     * @brief The VirtualFile class
     */
    class VirtualFile : public VirtualFileSystemAbstractItem
    {
    public:
        VirtualFile();
        VirtualFile(const QString &path);

        QString data() const;
        void setData(const QString &data);
        VirtualFile &appendData(const QString &data, const QString &sep = "\n");
        VirtualFile &prependData(const QString &data, const QString &sep = "\n");

        void write() const override;
        bool remove() const override;

    private:
        QString m_Data;
    };

} // namespace generator
