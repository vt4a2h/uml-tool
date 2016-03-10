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

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

#include <boost/range/algorithm/find_if.hpp>

#include <utility/helpfunctions.h>

#include "extendedtype.h"
#include "field.h"
#include "enums.h"
#include "constants.h"

namespace
{
    const QString nameMark = "Name";
    const QString sectionMark  = "Section";
    const QString typeMark  = "Type";
    const QString csMark  = "Const status";
    const QString slotMark  = "Slot";
    const QString signalMark  = "Signal";
    const QString rtMark  = "Return type id";
    const QString rhsiMark  = "Rhs identificator";
    const QString paramsMark  = "Parameters";
    const QString lhsMark = "Lhs identificators";
}

using namespace boost;

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
        : BasicElement(std::move(src))
    {
        moveFrom(std::move(src));
    }

    /**
     * @brief ClassMethod::ClassMethod
     * @param src
     */
    ClassMethod::ClassMethod(const ClassMethod &src)
        : BasicElement(src)
    {
        copyFrom(src);
    }

    /**
     * @brief ClassMethod::ClassMethod
     * @param name
     */
    ClassMethod::ClassMethod(const QString &name)
        : BasicElement(name)
        , m_Type(SimpleMethod)
        , m_Section(Public)
        , m_ConstStatus(false)
        , m_SlotStatus(false)
        , m_SignalStatus(false)
        , m_ReturnTypeId(EntityID::nullID())
        , m_RhsIdentificator(RhsIdentificator::None)
    {
        static int r = qRegisterMetaType<entity::ClassMethod>("entity::ClassMethod"); Q_UNUSED(r);
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
            static_cast<BasicElement*>(this)->operator =(std::forward<ClassMethod>(rhs));
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
            static_cast<BasicElement*>(this)->operator =(rhs);
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
        return static_cast<const BasicElement&>(lhs) == static_cast<const BasicElement&>(rhs) &&
               lhs.m_Section           == rhs.m_Section           &&
               lhs.m_ConstStatus       == rhs.m_ConstStatus       &&
               lhs.m_SlotStatus        == rhs.m_SlotStatus        &&
               lhs.m_SignalStatus      == rhs.m_SignalStatus      &&
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
     * @brief ClassMethod::isSignal
     * @return
     */
    bool ClassMethod::isSignal() const
    {
        return m_SignalStatus;
    }

    /**
     * @brief ClassMethod::setIsSignal
     * @param signalStatus
     */
    void ClassMethod::setIsSignal(bool signalStatus)
    {
        m_SignalStatus = signalStatus;
        if (m_SignalStatus) {
            m_Section = Section::None;
            m_SlotStatus = false;
        } else
            m_Section = Section::Public;
    }

    /**
     * @brief ClassMethod::isSlot
     * @return
     */
    bool ClassMethod::isSlot() const
    {
        return m_SlotStatus;
    }

    /**
     * @brief ClassMethod::setIsSlot
     * @param slotStatus
     */
    void ClassMethod::setIsSlot(bool slotStatus)
    {
        m_SlotStatus = slotStatus;
        if (m_SlotStatus)
            m_SignalStatus = false;

        if (m_Section == Section::None && !isSignal())
            m_Section = Section::Public;
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
        return m_LhsIdentificators.values().toVector();
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
        Q_ASSERT(!name.isEmpty());
        auto it = range::find_if(m_Parameters, [&name](auto &f){ return f->name() == name; });
        return it != m_Parameters.end() ? *it : SharedField();
    }

    /**
     * @brief ClassMethod::addParameter
     * @param name
     * @param typeId
     * @return
     */
    SharedField ClassMethod::addParameter(const QString &name, const EntityID &typeId)
    {
        if (!name.isEmpty()) {
            auto existField = getParameter(name);
            if (existField)
                removeParameter(name);
        }

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
        QJsonObject result = BasicElement::toJson();

        result.insert(sectionMark, m_Section);
        result.insert(typeMark, m_Type);
        result.insert(csMark, m_ConstStatus);
        result.insert(slotMark, m_SlotStatus);
        result.insert(signalMark, m_SignalStatus);
        result.insert(rtMark, m_ReturnTypeId.toJson());
        result.insert(rhsiMark, int(m_RhsIdentificator));

        QJsonArray parameters;
        for (auto &&value : m_Parameters)
            parameters.append(value->toJson());
        result.insert(paramsMark, parameters);

        QJsonArray lhsIdentificators;
        for (auto &&id : m_LhsIdentificators)
            lhsIdentificators.append(int(id));
        result.insert(lhsMark, lhsIdentificators);

        return result;
    }

    /**
     * @brief ClassMethod::fromJson
     * @param src
     * @param errorList
     */
    void ClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        BasicElement::fromJson(src, errorList);

        utility::checkAndSet(src, sectionMark, errorList, [&src, this](){
            m_Section = static_cast<Section>(src[sectionMark].toInt());
        });
        utility::checkAndSet(src, typeMark, errorList, [&src, this](){
            m_Type = static_cast<ClassMethodType>(src[typeMark].toInt());
        });
        utility::checkAndSet(src, csMark, errorList, [&src, this](){
            m_ConstStatus = src[csMark].toBool();
        });
        utility::checkAndSet(src, signalMark, errorList, [&src, this](){
            m_SignalStatus = src[signalMark].toBool();
        });
        utility::checkAndSet(src, slotMark, errorList, [&src, this](){
            m_SlotStatus = src[slotMark].toBool();
        });
        utility::checkAndSet(src, rtMark, errorList, [&src, &errorList, this](){
            m_ReturnTypeId.fromJson(src[rtMark], errorList);
        });
        utility::checkAndSet(src, rhsiMark, errorList, [&src, this](){
            m_RhsIdentificator = static_cast<RhsIdentificator>(src[rhsiMark].toInt());
        });

        m_Parameters.clear();
        utility::checkAndSet(src, paramsMark, errorList, [&src, &errorList, this](){
            if (src[paramsMark].isArray()) {
                SharedField parameter;
                for (auto &&value : src[paramsMark].toArray()) {
                    parameter = std::make_shared<Field>();
                    parameter->fromJson(value.toObject(), errorList);
                    m_Parameters << parameter;
                }
            } else {
                errorList << "Error: \"Parameters\" is not array";
            }
        });

        m_LhsIdentificators.clear();
        utility::checkAndSet(src, lhsMark, errorList, [&src, &errorList, this](){
            if (src[lhsMark].isArray()) {
                for (auto &&value : src[lhsMark].toArray())
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

        m_Section = std::move(src.m_Section);
        m_ConstStatus = std::move(src.m_ConstStatus);
        m_SlotStatus = std::move(src.m_SlotStatus);
        m_SignalStatus = std::move(src.m_SignalStatus);
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

        m_Section = src.m_Section;
        m_ConstStatus = src.m_ConstStatus;
        m_SlotStatus = src.m_SlotStatus;
        m_SignalStatus = src.m_SignalStatus;
        m_ReturnTypeId = src.m_ReturnTypeId;

        utility::deepCopySharedPointerList(src.m_Parameters, m_Parameters);

        m_RhsIdentificator  = src.m_RhsIdentificator;
        m_LhsIdentificators = src.m_LhsIdentificators;
    }

    /**
     * @brief ClassMethod::returnTypeId
     * @return
     */
    EntityID ClassMethod::returnTypeId() const
    {
       return m_ReturnTypeId;
    }

    /**
     * @brief ClassMethod::setReturnTypeId
     * @param returnTypeId
     */
    void ClassMethod::setReturnTypeId(const EntityID &returnTypeId)
    {
        m_ReturnTypeId = returnTypeId;
    }

} // namespace entity
