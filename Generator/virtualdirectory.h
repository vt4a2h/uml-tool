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

#include <QDir>

#include "virtualfilesystemabstractitem.h"
#include "virtualfile.h"

#include "generator_types.hpp"

namespace Generator {

    /**
     * @brief The VirtualDirectory class
     */
    class VirtualDirectory : public VirtualFileSystemAbstractItem
    {
    public:
        VirtualDirectory();
        VirtualDirectory(const VirtualDirectory &src);
        VirtualDirectory(VirtualDirectory &&src);
        VirtualDirectory(const QString &path);

        VirtualDirectory &operator =(VirtualDirectory rhs);
        VirtualDirectory &operator =(VirtualDirectory &&rhs);

        SharedVirtualFile addFile(const QString &fileName);
        SharedVirtualFile getFile(const QString &fileName);
        bool constainsFile(const QString &name) const;

        SharedVirtualDirectory addDirectory(const QString &directoryName);
        SharedVirtualDirectory getDirectory(const QString &directoryName);
        bool constainsDirectory(const QString &name) const;

        bool removeNestedItem(const QString &name);

        void clearVirtualStructure();

        void write() const override;
        bool remove() const override;

    protected:
        void moveFrom(VirtualDirectory &src);
        void copyFrom(const VirtualDirectory &src);

    private:
        template<class Item>
        std::shared_ptr<Item> addItem(const QString &name)
        {
            QChar sep(QDir::separator());
            QString path(m_FileInfo.filePath().append(sep).append(name));

            std::shared_ptr<Item> result(std::static_pointer_cast<Item>(*m_Files.insert(name, std::make_shared<Item>(path))));
            result->setErrorList(m_ErrorList);

            return result;
        }

        template<class Item>
        std::shared_ptr<Item> getItem(const QString &name)
        {
            auto it = m_Files.find(name);

            if (it == m_Files.end())
                return nullptr;

            return std::dynamic_pointer_cast<Item>(*it);
        }

        VirtualFiles m_Files;
    };

} // namespace generator
