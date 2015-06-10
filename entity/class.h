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

#include "type.h"
#include "types.h"
#include "enums.h"

/**
 * @brief entity
 */
namespace entity {

    // TODO: add methods for generate dtor, ctor, getter and setter
    // TODO: add includes to header and to source
    // TODO: add forward declaration possibilities

    // TODO: add empty string between methods and fields
    // TODO: add insert comment posibility

    /**
     * @brief The Class class
     */
    class Class : public Type
    {
    public:
        Class();
        Class(Class &&src);
        Class(const Class &src);
        Class(const QString &name, const QString &scopeId);

        Class &operator =(Class &&rhs);
        Class &operator =(Class rhs);

        friend bool operator ==(const Class &lhs, const Class &rhs);

        Parent addParent(const QString &typeId, Section section);
        ParentsList getParents(const QString &typeId);
        bool containsParent(const QString &typeId);
        void removeParent(const QString &typeId);
        bool anyParents() const;
        ParentsList parents() const;

        template <class T = ClassMethod> std::shared_ptr<T> makeMethod(const QString &name);
        void addMethod(SharedMethod method);
        MethodsList getMethod(const QString &name);
        bool containsMethod(const QString &name);
        void removeMethods(const QString &name);
        void removeMethod(const SharedMethod &method);
        bool anyMethods() const;

        bool containsMethods(Section section) const;
        MethodsList methods(Section section) const;

        SharedField addField(const QString &name, const QString &typeId,
                             const QString &prefix = "", Section section = Public);
        SharedField getField(const QString &name) const;
        bool containsField(const QString &name) const;
        void removeField(const QString &name);
        bool anyFields() const;

        bool containsFields(Section section) const;
        FieldsList fields(Section section) const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

        bool isEqual(const Class &rhs) const;

    public: // IComponent omplementation
        entity::SharedMethod addNewMethod() override;
        MethodsList methods() const override;
        FieldsList fields() const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

    protected:
        void moveFrom(Class &src);
        void copyFrom(const Class &src);

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        ParentsList m_Parents;
        MethodsList m_Methods;
        FieldsList  m_Fields;
    };

    template <class T>
    std::shared_ptr<T> Class::makeMethod(const QString &name)
    {
        typedef typename std::conditional<std::is_class<T>::value && std::is_base_of<ClassMethod, T>::value, T, ClassMethod>::type ResultType;
        auto value = std::make_shared<ResultType>(name);
        value->setScopeId(scopeId());
        m_Methods << value;

        return value;
    }

} // namespace entity
