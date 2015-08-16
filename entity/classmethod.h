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

#include <QString>

#include "basicentity.h"
#include "entity_types.hpp"

class QJsonObject;

namespace entity {

    class ExtendedType;
    class Field;
    enum Section          : int;
    enum RhsIdentificator : int;
    enum LhsIdentificator : int;
    enum ClassMethodType  : int;

    // TODO: add QString m_Body, for generate body if it's possible
    // TODO: add dtor and ctor types
    // TODO: add documentation if user would like it
    // TODO: how about template functions

    /**
     * @brief The ClassMethod class
     */
    class ClassMethod : public BasicEntity
    {
    public:
        ClassMethod();
        ClassMethod(ClassMethod &&src);
        ClassMethod(const ClassMethod &src);
        ClassMethod(const QString &name);
        virtual ~ClassMethod();

        ClassMethod &operator =(ClassMethod &&rhs);
        ClassMethod &operator =(const ClassMethod &rhs);

        friend bool operator ==(const ClassMethod &lhs, const ClassMethod &rhs);

        Section section() const;
        void setSection(Section section);

        bool isConst() const;
        void setConstStatus(bool newStatus);

        RhsIdentificator rhsIdentificator() const;
        void setRhsIdentificator(RhsIdentificator identificator);

        QString returnTypeId() const;
        void setReturnTypeId(const QString &returnTypeId);

        LhsIdentificatorsList lhsIdentificators() const;
        void addLhsIdentificator(LhsIdentificator identificator);
        bool containsLhsIdentficator(LhsIdentificator identificator) const;
        bool hasLhsIdentificators() const;
        void removeLhsIdentificator(LhsIdentificator identificator);

        SharedField getParameter(const QString &name) const;
        SharedField addParameter(const QString &name, const QString &typeId);
        bool containsParameter(const QString &name);
        bool hasParameters() const;
        void removeParameter(const QString &name);
        FieldsList parameters() const;

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

        ClassMethodType type() const;

        virtual bool isEqual(const ClassMethod &rhs) const;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

    protected:
        virtual void moveFrom(ClassMethod &&src);
        virtual void copyFrom(const ClassMethod &src);

        ClassMethodType m_Type;

    private:
        QString m_ScopeId;
        Section m_Section;
        bool    m_ConstStatus;
        QString m_ReturnTypeId;

        FieldsList m_Parameters;

        RhsIdentificator  m_RhsIdentificator;
        LhsIdentificators m_LhsIdentificators;
    };

} // namespace entity
