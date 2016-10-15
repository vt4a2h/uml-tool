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

#include <common/common_types.h>

#include <models/models_types.hpp>

#include <entity/entity_types.hpp>

#include <project/ProjectTypes.hpp>

#include <db/db_types.hpp>

namespace translation {

    /// The SignatureMaker class
    class SignatureMaker
    {
        Q_DECLARE_TR_FUNCTIONS(SignatureMaker)

    public:
        SignatureMaker();
        SignatureMaker(const db::SharedDatabase &globalDb, const db::SharedProjectDatabase &projectDb,
                       const entity::SharedScope &scope, const entity::SharedType &type);

        ~SignatureMaker();

        QString signature(const common::SharedBasicEntity &component);

        entity::SharedType type() const;
        void setType(const entity::SharedType &type);

        entity::SharedScope scope() const;
        void setScope(const entity::SharedScope &scope);

    private:
        QString makeType(const entity::SharedType &type) const;
        QString makeExtType(const entity::SharedExtendedType &type) const;
        QString makeTypeOrExtType(const entity::SharedType &type) const;

        QString typeSignatureById(const common::ID &id) const;

        QString makeField(const entity::SharedField &field) const;
        QString makeMethod(const entity::SharedMethod &method) const;
        QString makeProperty(const entity::SharedProperty &property) const;

        entity::SharedType findType(const common::ID &typeId) const;
        entity::SharedScope findScope(const common::ID &scopeId) const;

        entity::SharedType  m_Type;
        entity::SharedScope m_Scope;

        db::SharedDatabase m_GlobalDatabase;
        db::SharedProjectDatabase m_ProjectDatabase;

        using MakerFunction = std::function<QString(const common::SharedBasicEntity &)>;
        QMap<size_t, MakerFunction> m_MakersMap;
    };

} // namespace translation
