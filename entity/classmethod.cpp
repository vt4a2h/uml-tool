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

#include "classmethod.h"
#include "extendedtype.h"
#include "field.h"
#include "enums.h"
#include "constants.h"

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief ClassMethod::ClassMethod
     */
    ClassMethod::ClassMethod()
        : ClassMethod(DEFAULT_NAME)
    {
    }

    /**
     * @brief ClassMethod::ClassMethod
     * @param src
     */
    ClassMethod::ClassMethod(ClassMethod &&src)
        : BasicEntity(std::forward<ClassMethod>(src))
    {
        moveFrom(std::forward<ClassMethod>(src));
    }

    /**
     * @brief ClassMethod::ClassMethod
     * @param src
     */
    ClassMethod::ClassMethod(const ClassMethod &src)
        : BasicEntity(src)
    {
        copyFrom(src);
    }

    /**
     * @brief ClassMethod::ClassMethod
     * @param name
     */
    ClassMethod::ClassMethod(const QString &name)
        : BasicEntity(name)
        , m_Type(SimpleMethod)
        , m_ScopeId(STUB_ID)
        , m_Section(Public)
        , m_ConstStatus(false)
        , m_ReturnTypeId(VOID_ID)
        , m_RhsIdentificator(None)
    {
    }

    /**
     * @brief ClassMethod::~ClassMethod
     */
    ClassMethod::~ClassMethod()
    {
    }

    /**
     * @brief ClassMethod::operator =
     * @param rhs
     * @return
     */
    ClassMethod &ClassMethod::operator =(ClassMethod &&rhs)
    {
        if (this != &rhs) {
            static_cast<BasicEntity*>(this)->operator =(std::forward<ClassMethod>(rhs));
            moveFrom(std::forward<ClassMethod>(rhs));
        }

        return *this;
    }

    /**
     * @brief ClassMethod::operator =
     * @param rhs
     * @return
     */
    ClassMethod &ClassMethod::operator =(const ClassMethod &rhs)
    {
        if (this != &rhs) {
            static_cast<BasicEntity*>(this)->operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const ClassMethod &lhs, const ClassMethod &rhs)
    {
        return static_cast<const BasicEntity&>(lhs) == static_cast<const BasicEntity&>(rhs) &&
               lhs.m_ScopeId           == rhs.m_ScopeId           &&
               lhs.m_Section           == rhs.m_Section           &&
               lhs.m_ConstStatus       == rhs.m_ConstStatus       &&
               lhs.m_ReturnTypeId      == rhs.m_ReturnTypeId      &&
               lhs.m_RhsIdentificator  == rhs.m_RhsIdentificator  &&
               lhs.m_LhsIdentificators == rhs.m_LhsIdentificators &&
               utility::seqSharedPointerEq(lhs.m_Parameters, rhs.m_Parameters);

    }

    /**
     * @brief ClassMethod::section
     * @return
     */
    Section ClassMethod::section() const
    {
        return m_Section;
    }

    /**
     * @brief ClassMethod::setSection
     * @param section
     */
    void ClassMethod::setSection(Section section)
    {
        m_Section = section;
    }

    /**
     * @brief ClassMethod::isConst
     * @return
     */
    bool ClassMethod::isConst() const
    {
        return m_ConstStatus;
    }

    /**
     * @brief ClassMethod::setConstStatus
     * @param newStatus
     */
    void ClassMethod::setConstStatus(bool newStatus)
    {
        m_ConstStatus = newStatus;
    }

    /**
     * @brief ClassMethod::rhsIdentificator
     * @return
     */
    RhsIdentificator ClassMethod::rhsIdentificator() const
    {
        return m_RhsIdentificator;
    }

    /**
     * @brief ClassMethod::setRhsIdentificator
     * @param identificator
     */
    void ClassMethod::setRhsIdentificator(RhsIdentificator identificator)
    {
        m_RhsIdentificator = identificator;
    }

    /**
     * @brief ClassMethod::lhsIdentificators
     * @return
     */
    LhsIdentificatorsList ClassMethod::lhsIdentificators() const
    {
        return m_LhsIdentificators.values();
    }

    /**
     * @brief ClassMethod::addLhsIdentificator
     * @param identificator
     */
    void ClassMethod::addLhsIdentificator(LhsIdentificator identificator)
    {
        m_LhsIdentificators << identificator;
    }

    /**
     * @brief ClassMethod::containsLhsIdentficator
     * @param identificator
     * @return
     */
    bool ClassMethod::containsLhsIdentficator(LhsIdentificator identificator) const
    {
        return m_LhsIdentificators.contains(identificator);
    }

    /**
     * @brief ClassMethod::hasLhsIdentificators
     * @return
     */
    bool ClassMethod::hasLhsIdentificators() const
    {
        return !m_LhsIdentificators.isEmpty();
    }

    /**
     * @brief ClassMethod::removeLhsIdentificator
     * @param identificator
     */
    void ClassMethod::removeLhsIdentificator(LhsIdentificator identificator)
    {
        m_LhsIdentificators.remove(identificator);
    }

    /**
     * @brief ClassMethod::getParameter
     * @param name
     * @return
     */
    SharedField ClassMethod::getParameter(const QString &name) const
    {
        auto it = utility::find_if(m_Parameters, [&name](const SharedField &f){ return f->name() == name; });
        return it != m_Parameters.end() ? *it : SharedField();
    }

    /**
     * @brief ClassMethod::addParameter
     * @param name
     * @param typeId
     * @return
     */
    SharedField ClassMethod::addParameter(const QString &name, const QString &typeId)
    {
        auto existField = getParameter(name);
        if (existField)
            removeParameter(name);

        auto f = std::make_shared<Field>(name, typeId);
        m_Parameters << f;

        return f;
    }

    /**
     * @brief ClassMethod::containsParameter
     * @param name
     * @return
     */
    bool ClassMethod::containsParameter(const QString &name)
    {
        return getParameter(name).operator bool();
    }

    /**
     * @brief ClassMethod::hasParameters
     * @return
     */
    bool ClassMethod::hasParameters() const
    {
        return !m_Parameters.isEmpty();
    }

    /**
     * @brief ClassMethod::removeParameter
     * @param name
     */
    void ClassMethod::removeParameter(const QString &name)
    {
        auto parameter = getParameter(name);
        if (parameter)
            m_Parameters.removeOne(parameter);
    }

    /**
     * @brief ClassMethod::parameters
     * @return
     */
    FieldsList ClassMethod::parameters() const
    {
        return m_Parameters;
    }

    /**
     * @brief ClassMethod::toJson
     * @return
     */
    QJsonObject ClassMethod::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("ScopeID", m_ScopeId);
        result.insert("Section", m_Section);
        result.insert("Type", m_Type);
        result.insert("Const status", m_ConstStatus);
        result.insert("Return type id", m_ReturnTypeId);
        result.insert("Rhs identificator", m_RhsIdentificator);

        QJsonArray parameters;
        for (auto &&value : m_Parameters)
            parameters.append(value->toJson());
        result.insert("Parameters", parameters);

        QJsonArray lhsIdentificators;
        for (auto &&id : m_LhsIdentificators)
            lhsIdentificators.append(id);
        result.insert("Lhs identificators", lhsIdentificators);

        return result;
    }

    /**
     * @brief ClassMethod::fromJson
     * @param src
     * @param errorList
     */
    void ClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
        });
        utility::checkAndSet(src, "ScopeID", errorList, [&src, this](){
           m_ScopeId = src["ScopeID"].toString();
        });
        utility::checkAndSet(src, "Section", errorList, [&src, this](){
            m_Section = static_cast<Section>(src["Section"].toInt());
        });
        utility::checkAndSet(src, "Type", errorList, [&src, this](){
            m_Type = static_cast<ClassMethodType>(src["Type"].toInt());
        });
        utility::checkAndSet(src, "Const status", errorList, [&src, this](){
            m_ConstStatus = src["Const status"].toBool();
        });
        utility::checkAndSet(src, "Return type id", errorList, [&src, this](){
            m_ReturnTypeId = src["Return type id"].toString();
        });
        utility::checkAndSet(src, "Rhs identificator", errorList, [&src, this](){
            m_RhsIdentificator = static_cast<RhsIdentificator>(src["Rhs identificator"].toInt());
        });

        m_Parameters.clear();
        utility::checkAndSet(src, "Parameters", errorList, [&src, &errorList, this](){
            if (src["Parameters"].isArray()) {
                SharedField parameter;
                for (auto &&value : src["Parameters"].toArray()) {
                    parameter = std::make_shared<Field>();
                    parameter->fromJson(value.toObject(), errorList);
                    m_Parameters << parameter;
                }
            } else {
                errorList << "Error: \"Parameters\" is not array";
            }
        });

        m_LhsIdentificators.clear();
        utility::checkAndSet(src, "Lhs identificators", errorList, [&src, &errorList, this](){
            if (src["Lhs identificators"].isArray()) {
                for (auto &&value : src["Lhs identificators"].toArray())
                    m_LhsIdentificators.insert(static_cast<LhsIdentificator>(value.toInt()));
            } else {
                errorList << "Error: \"Lhs identificators\" is not array";
            }
        });
    }

    /**
     * @brief ClassMethod::hashType
     * @return
     */
    size_t ClassMethod::hashType() const
    {
        return ClassMethod::staticHashType();
    }

    /**
     * @brief ClassMethod::staticHashType
     * @return
     */
    size_t ClassMethod::staticHashType()
    {
        return typeid(ClassMethod).hash_code();
    }

    /**
     * @brief ClassMethod::type
     * @return
     */
    ClassMethodType ClassMethod::type() const
    {
        return m_Type;
    }

    /**
     * @brief ClassMethod::isEqual
     * @param rhs
     * @return
     */
    bool ClassMethod::isEqual(const ClassMethod &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief ClassMethod::moveFrom
     * @param src
     */
    void ClassMethod::moveFrom(ClassMethod &&src)
    {
        m_Type = std::move(src.m_Type);

        m_ScopeId = std::move(src.m_ScopeId);
        m_Section = std::move(src.m_Section);
        m_ConstStatus = std::move(src.m_ConstStatus);
        m_ReturnTypeId = std::move(src.m_ReturnTypeId);

        m_Parameters = std::move(src.m_Parameters);

        m_RhsIdentificator  = std::move(src.m_RhsIdentificator );
        m_LhsIdentificators = std::move(src.m_LhsIdentificators);
    }

    /**
     * @brief ClassMethod::copyFrom
     * @param src
     */
    void ClassMethod::copyFrom(const ClassMethod &src)
    {
        m_Type = src.m_Type;

        m_ScopeId = src.m_ScopeId;
        m_Section = src.m_Section;
        m_ConstStatus = src.m_ConstStatus;
        m_ReturnTypeId = src.m_ReturnTypeId;

        utility::deepCopySharedPointerList(src.m_Parameters, m_Parameters);

        m_RhsIdentificator  = src.m_RhsIdentificator;
        m_LhsIdentificators = src.m_LhsIdentificators;
    }

    /**
     * @brief ClassMethod::scopeId
     * @return
     */
    QString ClassMethod::scopeId() const
    {
       return m_ScopeId;
    }

    /**
     * @brief ClassMethod::setScopeId
     * @param scopeId
     */
    void ClassMethod::setScopeId(const QString &scopeId)
    {
       m_ScopeId = scopeId;
    }

    /**
     * @brief ClassMethod::returnTypeId
     * @return
     */
    QString ClassMethod::returnTypeId() const
    {
       return m_ReturnTypeId;
    }

    /**
     * @brief ClassMethod::setReturnTypeId
     * @param returnTypeId
     */
    void ClassMethod::setReturnTypeId(const QString &returnTypeId)
    {
        m_ReturnTypeId = returnTypeId;
    }

} // namespace entity
