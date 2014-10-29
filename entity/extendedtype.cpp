#include "extendedtype.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief ExtendedType::ExtendedType
     */
    ExtendedType::ExtendedType()
        : ExtendedType(DEFAULT_NAME, STUB_ID)
    {
    }

    /**
     * @brief ExtendedType::ExtendedType
     * @param name
     * @param scopeId
     */
    ExtendedType::ExtendedType(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_ConstStatus(false)
        , m_TypeId(STUB_ID)
    {
        m_KindOfType = ExtendedTypeType;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const ExtendedType &lhs, const ExtendedType &rhs)
    {
        return static_cast<const Type&>(lhs).isEqual(rhs)         &&
               lhs.m_ConstStatus        == rhs.m_ConstStatus      &&
               lhs.m_TypeId             == rhs.m_TypeId           &&
               lhs.m_PointersAndLinks   == rhs.m_PointersAndLinks &&
               lhs.m_TemplateParameters == rhs.m_TemplateParameters;
    }

    /**
     * @brief ExtendedType::isLink
     * @return
     */
    bool ExtendedType::isLink() const
    {
        return !m_PointersAndLinks.isEmpty() && m_PointersAndLinks.last().first == "&";
    }

    /**
     * @brief ExtendedType::addPointerStatus
     * @param pointerToConst
     */
    void ExtendedType::addPointerStatus(bool pointerToConst)
    {
        m_PointersAndLinks.append(std::make_pair("*", pointerToConst));
    }

    /**
     * @brief ExtendedType::removePointerStatus
     */
    void ExtendedType::removePointerStatus()
    {
        if (isPointer()) m_PointersAndLinks.removeLast();
    }

    /**
     * @brief ExtendedType::isPointer
     * @return
     */
    bool ExtendedType::isPointer() const
    {
        return !m_PointersAndLinks.isEmpty() && m_PointersAndLinks.last().first == "*";
    }

    /**
     * @brief ExtendedType::addLinkStatus
     */
    void ExtendedType::addLinkStatus()
    {
        m_PointersAndLinks.append(std::make_pair("&", false));
    }

    /**
     * @brief ExtendedType::removeLinkStatus
     */
    void ExtendedType::removeLinkStatus()
    {
        if (isLink()) m_PointersAndLinks.removeLast();
    }

    /**
     * @brief ExtendedType::pl
     * @return
     */
    ExtendedType::PlList ExtendedType::pl() const
    {
        return m_PointersAndLinks;
    }

    /**
     * @brief ExtendedType::isConst
     * @return
     */
    bool ExtendedType::isConst() const
    {
        return m_ConstStatus;
    }

    /**
     * @brief ExtendedType::setConstStatus
     * @param status
     */
    void ExtendedType::setConstStatus(bool status)
    {
        m_ConstStatus = status;
    }

    /**
     * @brief ExtendedType::addTemplateParameter
     * @param typeId
     */
    void ExtendedType::addTemplateParameter(const QString &typeId)
    {
        m_TemplateParameters << typeId;
    }

    /**
     * @brief ExtendedType::containsTemplateParameter
     * @param typeId
     * @return
     */
    bool ExtendedType::containsTemplateParameter(const QString &typeId) const
    {
        return m_TemplateParameters.contains(typeId);
    }

    /**
     * @brief ExtendedType::removeTemplateParameters
     * @param typeId
     */
    void ExtendedType::removeTemplateParameters(const QString &typeId)
    {
        m_TemplateParameters.removeAll(typeId);
    }

    /**
     * @brief ExtendedType::templateParameters
     * @return
     */
    ExtendedTypesIdList ExtendedType::templateParameters() const
    {
        return m_TemplateParameters;
    }

    /**
     * @brief ExtendedType::typeId
     * @return
     */
    QString ExtendedType::typeId() const
    {
        return m_TypeId;
    }

    /**
     * @brief ExtendedType::setTypeId
     * @param typeId
     */
    void ExtendedType::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

    /**
     * @brief ExtendedType::toJson
     * @return
     */
    QJsonObject ExtendedType::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Const status", m_ConstStatus);
        result.insert("Type id", m_TypeId);

        QJsonArray pointersAndLinks;
        QJsonObject obj;
        for (auto &&p : m_PointersAndLinks) {
            obj.insert("Pl", p.first);
            obj.insert("Const pl status", p.second);
            pointersAndLinks.append(obj);
        }
        result.insert("Pointers and links", pointersAndLinks);

        QJsonArray templateParameters;
        for (auto &&parameterId : m_TemplateParameters) templateParameters.append(parameterId);
        result.insert("Template parameters", templateParameters);

        return result;
    }

    /**
     * @brief ExtendedType::fromJson
     * @param src
     * @param errorList
     */
    void ExtendedType::fromJson(const QJsonObject &src, QStringList &errorList)
    {
       Type::fromJson(src, errorList);
       utility::checkAndSet(src, "Const status", errorList, [&src, this](){ m_ConstStatus = src["Const status"].toBool(); });
       utility::checkAndSet(src, "Type id", errorList, [&src, this](){ m_TypeId = src["Type id"].toString(); });

       m_PointersAndLinks.clear();
       utility::checkAndSet(src, "Pointers and links", errorList, [&src, &errorList, this](){
           if (src["Pointers and links"].isArray()) {
               Pl pl;
               QJsonObject obj;
               for (auto &&value : src["Pointers and links"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Pl", errorList, [&obj, &pl](){ pl.first = obj["Pl"].toString(); });
                    utility::checkAndSet(obj, "Const pl status", errorList, [&obj, &pl](){ pl.second = obj["Const pl status"].toBool(); });
                    m_PointersAndLinks << pl;
               }
           } else {
               errorList << "Error: \"Pointers and links\" is not array";
           }
       });

       m_TemplateParameters.clear();
       utility::checkAndSet(src, "Template parameters", errorList, [&src, &errorList, this](){
           if (src["Template parameters"].isArray()) {
               for (auto &&value : src["Template parameters"].toArray()) m_TemplateParameters << value.toString();
           } else {
               errorList << "Error: \"Template parameters\" is not array";
           }
       });
    }

    /**
     * @brief ExtendedType::isEqual
     * @param rhs
     * @return
     */
    bool ExtendedType::isEqual(const ExtendedType &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
