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

#pragma once

#include "types.h"

#include <QFileInfo>

namespace Generator {

    /**
     * @brief The VirtualFileSystemAbstractItem class
     */
    class VirtualFileSystemAbstractItem
    {
    public:
        VirtualFileSystemAbstractItem();
        VirtualFileSystemAbstractItem(VirtualFileSystemAbstractItem &&src) noexcept;
        VirtualFileSystemAbstractItem(const VirtualFileSystemAbstractItem &src);
        VirtualFileSystemAbstractItem(const QString &path);
        virtual ~VirtualFileSystemAbstractItem();

        VirtualFileSystemAbstractItem &operator =(VirtualFileSystemAbstractItem &&rhs) noexcept;
        VirtualFileSystemAbstractItem &operator =(VirtualFileSystemAbstractItem rhs);

        virtual void write() const;
        virtual bool remove() const;
        virtual bool valid() const;

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        QString baseName() const;

        SharedErrorList errorList() const;
        void setErrorList(const SharedErrorList &errorList);

    protected:
        virtual void copyFrom(const VirtualFileSystemAbstractItem &src);
        virtual void moveFrom(VirtualFileSystemAbstractItem &&src) noexcept;
        void toNativeSeparators(); // just for better displaying

        QFileInfo m_FileInfo;
        SharedErrorList m_ErrorList;
    };

} // namespace generator

