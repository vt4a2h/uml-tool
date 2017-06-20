/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/06/2015.
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

#include <QCoreApplication>
#include <QMap>

#include <Common/common_types.h>

#include <Models/ModelsTypes.hpp>

#include <Entity/entity_types.hpp>

#include <Project/ProjectTypes.hpp>

#include <DB/DBTypes.hpp>

namespace Translation {

    /// The SignatureMaker class
    class SignatureMaker
    {
        Q_DECLARE_TR_FUNCTIONS(SignatureMaker)

    public:
        SignatureMaker();
        SignatureMaker(const DB::SharedDatabase &globalDb, const DB::SharedProjectDatabase &projectDb,
                       const Entity::SharedScope &scope, const Entity::SharedType &type);

        ~SignatureMaker();

        QString signature(const Common::SharedBasicEntity &component);

        Entity::SharedType type() const;
        void setType(const Entity::SharedType &type);

        Entity::SharedScope scope() const;
        void setScope(const Entity::SharedScope &scope);

    private:
        QString makeType(const Entity::SharedType &type) const;
        QString makeExtType(const Entity::SharedExtendedType &type) const;
        QString makeTypeOrExtType(const Entity::SharedType &type) const;

        QString typeSignatureById(const Common::ID &id) const;

        QString makeField(const Entity::SharedField &field) const;
        QString makeMethod(const Entity::SharedMethod &method) const;
        QString makeProperty(const Entity::SharedProperty &property) const;

        Entity::SharedType findType(const Common::ID &typeId) const;
        Entity::SharedScope findScope(const Common::ID &scopeId) const;

        Entity::SharedType  m_Type;
        Entity::SharedScope m_Scope;

        DB::SharedDatabase m_GlobalDatabase;
        DB::SharedProjectDatabase m_ProjectDatabase;

        using MakerFunction = std::function<QString(const Common::SharedBasicEntity &)>;
        QMap<size_t, MakerFunction> m_MakersMap;
    };

} // namespace translation
