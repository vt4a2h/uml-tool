#include "extendedtype.h"
#include "helpfunctions.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    ExtendedType::ExtendedType()
        : ExtendedType(DEFAULT_NAME, STUB_ID)
    {
    }

    ExtendedType::ExtendedType(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_ConstStatus(false)
        , m_TypeId(STUB_ID)
    {
        m_KindOfType = ExtendedTypeType;
    }

    bool ExtendedType::isLink() const
    {
        return m_PointersAndLinks.last().first == "&";
    }

    void ExtendedType::addPointerStatus(bool pointerToConst)
    {
        m_PointersAndLinks.append(std::make_pair("*", pointerToConst));
    }

    void ExtendedType::removePointerStatus()
    {
        if (isPointer()) m_PointersAndLinks.removeLast();
    }

    bool ExtendedType::isPointer() const
    {
        return m_PointersAndLinks.last().first == "*";
    }

    void ExtendedType::addLinkStatus()
    {
        m_PointersAndLinks.append(std::make_pair("&", false));
    }

    void ExtendedType::removeLinkStatus()
    {
        if (isLink()) m_PointersAndLinks.removeLast();
    }

    ExtendedType::PlList ExtendedType::pl() const
    {
        return m_PointersAndLinks;
    }

    bool ExtendedType::isConst() const
    {
        return m_ConstStatus;
    }

    void ExtendedType::setConstStatus(bool status)
    {
        m_ConstStatus = status;
    }

    void ExtendedType::addTemplateParameter(const QString &typeId)
    {
        m_TemplateParameters << typeId;
    }

    bool ExtendedType::containsTemplateParameter(const QString &typeId) const
    {
        return m_TemplateParameters.contains(typeId);
    }

    void ExtendedType::removeTemplateParameters(const QString &typeId)
    {
        m_TemplateParameters.removeAll(typeId);
    }

    ExtendedTypesIdList ExtendedType::templateParameters() const
    {
        return m_TemplateParameters;
    }

    QString ExtendedType::typeId() const
    {
        return m_TypeId;
    }

    void ExtendedType::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

    QJsonObject ExtendedType::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Const status", m_ConstStatus);
        result.insert("Type id", m_TypeId);

        QJsonArray pointersAndLinks;
        QJsonObject obj;
        for (auto p : m_PointersAndLinks) {
            obj.insert("Pl", p.first);
            obj.insert("Const pl status", p.second);
            pointersAndLinks.append(obj);
        }
        result.insert("Pointers and links", pointersAndLinks);

        QJsonArray templateParameters;
        for (auto parameterId : m_TemplateParameters) templateParameters.append(parameterId);
        result.insert("Template parameters", templateParameters);

        return result;
    }

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
               for (auto value : src["Pointers and links"].toArray()) {
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
               for (auto value : src["Template parameters"].toArray()) m_TemplateParameters << value.toString();
           } else {
               errorList << "Error: \"Template parameters\" is not array";
           }
       });
    }
    
} // namespace entity
