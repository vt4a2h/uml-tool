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
#include <QSet>
#include <QVector>

#include <Common/BasicElement.h>
#include <Common/meta.h>

#include "entity_types.hpp"
#include "isectional.h"

class QJsonObject;

namespace Entity {

    class ExtendedType;
    class Field;
    enum Section : int;
    enum class RhsIdentificator : int;
    enum ClassMethodType : int;

    // TODO: add QString m_Body, for generate body if it's possible
    // TODO: add dtor and ctor types
    // TODO: add documentation if user would like it
    // TODO: how about template functions

    /**
     * @brief The ClassMethod class
     */
    class ClassMethod : public Common::BasicElement, public ISectional
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

        bool isConst() const;
        void setConstStatus(bool newStatus);

        bool isSlot() const;
        void setIsSlot(bool slotStatus);

        bool isSignal() const;
        void setIsSignal(bool signalStatus);

        RhsIdentificator rhsIdentificator() const;
        void setRhsIdentificator(RhsIdentificator identificator);

        Common::ID returnTypeId() const;
        void setReturnTypeId(const Common::ID &returnTypeId);

        LhsIdentificatorsList lhsIdentificators() const;
        void addLhsIdentificator(LhsIdentificator identificator);
        bool containsLhsIdentficator(LhsIdentificator identificator) const;
        bool hasLhsIdentificators() const;
        void removeLhsIdentificator(LhsIdentificator identificator);

        SharedField getParameter(const QString &name) const;
        SharedField addParameter(const QString &name, const Common::ID &typeId);
        bool containsParameter(const QString &name);
        bool hasParameters() const;
        void removeParameter(const QString &name);
        FieldsList parameters() const;

        ClassMethodType type() const;

        virtual bool isEqual(const ClassMethod &rhs) const;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(ClassMethod)

    public: // ISectional implementation
        Section section() const override;
        void setSection(Section section) override;

    protected:
        virtual void moveFrom(ClassMethod &&src);
        virtual void copyFrom(const ClassMethod &src);

        ClassMethodType m_Type;

    private:
        Section m_Section;
        bool    m_ConstStatus;
        bool    m_SlotStatus;
        bool    m_SignalStatus;
        Common::ID m_ReturnTypeId;

        FieldsList m_Parameters;

        RhsIdentificator  m_RhsIdentificator;
        LhsIdentificators m_LhsIdentificators;
    };

    inline bool operator==(const WeakMethod &lhs, const WeakMethod &rhs)
    {
        return lhs.lock() == rhs.lock();
    }

} // namespace entity
