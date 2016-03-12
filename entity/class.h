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

#include <QMap>
#include <QVector>

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
        Q_OBJECT

    public:
        Class();
        Class(Class &&src) noexcept = default;
        Class(const Class &src);
        Class(const QString &name, const EntityID &scopeId);

        Class &operator =(Class &&rhs) noexcept = default;
        Class &operator =(const Class &rhs);

        friend bool operator ==(const Class &lhs, const Class &rhs);

        Parent addParent(const EntityID &typeId, Section section);
        Parent parent(const EntityID &typeId) const;
        bool containsParent(const EntityID &typeId);
        void removeParent(const EntityID &typeId);
        bool anyParents() const;
        ParentsList parents() const;

        template <class T = ClassMethod> std::shared_ptr<T> makeMethod(const QString &name);
        MethodsList getMethod(const QString &name);
        bool containsMethod(const QString &name);
        void removeMethods(const QString &name);
        bool anyMethods() const;

        bool containsMethods(Section section) const;
        MethodsList methods(Section s) const;

        MethodsList optionalMethods(Section s) const;
        FieldsList optionalFields(Section s) const;

        MethodsList allMethods(Section s) const;
        FieldsList allFields(Section s) const;

        SharedField addField(const QString &name, const EntityID &typeId,
                             const QString &prefix = "", Section section = Public);
        SharedField getField(const QString &name) const;
        bool containsField(const QString &name) const;
        void removeField(const QString &name);
        bool anyFields() const;

        SharedProperty addProperty(const QString &name, const EntityID &typeId);
        ConstSharedProperty property(const QString &name) const;
        SharedProperty property(const QString &name);
        bool containsProperty(const QString &name) const;
        void removeProperty(const QString &name);
        bool anyProperties() const;

        bool containsFields(Section section) const;
        FieldsList fields(Section section) const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

    public: // IComponent omplementation
        entity::SharedMethod addNewMethod() override;
        void addExistsMethod(const SharedMethod &method, int pos = -1) override;
        int removeMethod(const SharedMethod &method) override;
        MethodsList methods() const override;

        SharedField addNewField() override;
        void addExistsField(const SharedField &field, int pos = -1) override;
        int removeField(const SharedField &field) override;
        FieldsList fields() const override;

        SharedProperty addNewProperty() override;
        void addExistsProperty(const SharedProperty &property, int pos = -1) override;
        int removeProperty(const SharedProperty &property) override;
        PropertiesList properties() const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        QString defaultName() const override;
        static QString staticDefaultName();

        add_meta(Class)

    signals:
        void typeUserAdded(const SharedTypeUser& tu);

    protected:
        void copyFrom(const Class &src);

    private Q_SLOTS:
        void onOptionalMethodAdded(const entity::SharedProperty &, const entity::SharedMethod &);
        void onOptionalMethodRemoved(const entity::SharedProperty &, const entity::SharedMethod &);

        void onOptionalFieldAdded(const entity::SharedProperty &, const entity::SharedField &);
        void onOptionalFieldRemoved(const entity::SharedProperty &, const entity::SharedField &);

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        ParentsList m_Parents;
        MethodsList m_Methods;
        WeakMethodsMap m_OptionalMethods;
        WeakFieldsMap m_OptionalFields;
        FieldsList  m_Fields;
        PropertiesList m_Properties;
    };

    template <class T>
    std::shared_ptr<T> Class::makeMethod(const QString &name)
    {
        using ResultType =
            typename std::conditional<std::is_class<T>::value && std::is_base_of<ClassMethod, T>::value,
                                      T, ClassMethod>::type;

        auto value = std::make_shared<ResultType>(name);
        value->setScopeId(scopeId());
        m_Methods << value;

        return value;
    }

} // namespace entity
