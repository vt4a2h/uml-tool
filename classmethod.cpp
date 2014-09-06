#include "classmethod.h"
#include "extendedtype.h"
#include "field.h"
#include "enums.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

namespace entity {

    ClassMethod::ClassMethod()
        : ClassMethod(DEFAULT_NAME)
    {
    }

    ClassMethod::ClassMethod(ClassMethod &&src)
    {
        moveFrom(src);
    }

    ClassMethod::ClassMethod(const ClassMethod &src)
    {
        copyFrom(src);
    }

    ClassMethod::ClassMethod(const QString &name)
        : m_Type(SimpleMethod)
        , m_Name(name)
        , m_Section(Public)
        , m_ConstStatus(false)
        , m_ReturnTypeId(VOID_ID)
        , m_RhsIdentificator(None)
    {
    }

    ClassMethod::~ClassMethod()
    {
    }

    ClassMethod &ClassMethod::operator =(ClassMethod &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    ClassMethod &ClassMethod::operator =(ClassMethod rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    bool operator ==(const ClassMethod &lhs, const ClassMethod &rhs)
    {
        return lhs.m_Name              == rhs.m_Name              &&
               lhs.m_Section           == rhs.m_Section           &&
               lhs.m_ConstStatus       == rhs.m_ConstStatus       &&
               lhs.m_ReturnTypeId      == rhs.m_ReturnTypeId      &&
               lhs.m_RhsIdentificator  == rhs.m_RhsIdentificator  &&
               lhs.m_LhsIdentificators == rhs.m_LhsIdentificators &&
               utility::listSharedPointerEq(lhs.m_Parameters, rhs.m_Parameters);

    }

    QString ClassMethod::name() const
    {
        return m_Name;
    }

    void ClassMethod::setName(const QString &name)
    {
        m_Name = name;
    }

    Section ClassMethod::section() const
    {
        return m_Section;
    }

    void ClassMethod::setSection(Section section)
    {
        m_Section = section;
    }

    bool ClassMethod::isConst() const
    {
        return m_ConstStatus;
    }

    void ClassMethod::setConstStatus(bool newStatus)
    {
        m_ConstStatus = newStatus;
    }

    RhsIdentificator ClassMethod::rhsIdentificator() const
    {
        return m_RhsIdentificator;
    }

    void ClassMethod::setRhsIdentificator(RhsIdentificator identificator)
    {
        m_RhsIdentificator = identificator;
    }

    LhsIdentificatorsList ClassMethod::lhsIdentificators() const
    {
        return m_LhsIdentificators.values();
    }

    void ClassMethod::addLhsIdentificator(LhsIdentificator identificator)
    {
        m_LhsIdentificators << identificator;
    }

    bool ClassMethod::containsLhsIdentficator(LhsIdentificator identificator) const
    {
        return m_LhsIdentificators.contains(identificator);
    }

    bool ClassMethod::hasLhsIdentificators() const
    {
        return m_LhsIdentificators.empty();
    }

    void ClassMethod::removeLhsIdentificator(LhsIdentificator identificator)
    {
        m_LhsIdentificators.remove(identificator);
    }

    SharedField ClassMethod::getParameter(const QString &name)
    {
        auto it = std::find_if(m_Parameters.begin(), m_Parameters.end(),
                               [&name](const SharedField &f){ return f->name() == name; });

        return (it != m_Parameters.end() ? *it : nullptr);
    }

    SharedField ClassMethod::addParameter(const QString &name, const QString &typeId)
    {
        auto existField = getParameter(name);
        if (existField.operator bool()) removeParameter(name);

        auto f = std::make_shared<Field>(name, typeId);
        m_Parameters << f;

        return f;
    }

    bool ClassMethod::containsParameter(const QString &name)
    {
        return getParameter(name).operator bool();
    }

    bool ClassMethod::hasParameters() const
    {
        return m_Parameters.empty();
    }

    void ClassMethod::removeParameter(const QString &name)
    {
        auto parameter = getParameter(name);
        if (parameter.operator bool()) m_Parameters.removeOne(parameter);
    }

    FieldsList ClassMethod::parameters() const
    {
        return m_Parameters;
    }

    QJsonObject ClassMethod::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
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

    void ClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
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
    ClassMethodType ClassMethod::type() const
    {
        return m_Type;
    }

    void ClassMethod::setType(const ClassMethodType &type)
    {
        m_Type = type;
    }

    bool ClassMethod::isEqual(const ClassMethod &rhs) const
    {
        return *this == rhs;
    }

    void ClassMethod::moveFrom(ClassMethod &src)
    {
        m_Type = std::move(src.m_Type);

        m_Name = std::move(src.m_Name);
        m_Section = std::move(src.m_Section);
        m_ConstStatus = std::move(src.m_ConstStatus);
        m_ReturnTypeId = std::move(src.m_ReturnTypeId);

        m_Parameters = std::move(src.m_Parameters);

        m_RhsIdentificator  = std::move(src.m_RhsIdentificator );
        m_LhsIdentificators = std::move(src.m_LhsIdentificators);
    }

    void ClassMethod::copyFrom(const ClassMethod &src)
    {
        m_Type = src.m_Type;

        m_Name = src.m_Name;
        m_Section = src.m_Section;
        m_ConstStatus = src.m_ConstStatus;
        m_ReturnTypeId = src.m_ReturnTypeId;

        utility::deepCopySharedPointerList(src.m_Parameters, m_Parameters);

        m_RhsIdentificator  = src.m_RhsIdentificator;
        m_LhsIdentificators = src.m_LhsIdentificators;
    }

    QString ClassMethod::returnTypeId() const
    {
        return m_ReturnTypeId;
    }

    void ClassMethod::setReturnTypeId(const QString &returnTypeId)
    {
        m_ReturnTypeId = returnTypeId;
    }


} // namespace entity
