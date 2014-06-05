#include "class.h"
#include "enums.h"
#include "field.h"
#include "classmethod.h"
#include "enums.h"
#include "extendedtype.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <utility>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    Class::Class()
        : Class(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Class::Class(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_Kind(ClassType)
        , m_FinalStatus(false)
    {
        m_KindOfType = UserClassType;
    }

    Parent Class::addParent(const QString &typeId, Section section)
    {
        auto parent = Parent(typeId, section);
        if (containsParent(typeId)) removeParent(typeId);
        m_Parents.append(parent);

        return parent;
    }

    ParentsList Class::getParents(const QString &typeId)
    {
        ParentsList result;

        for (auto &&p : m_Parents)
            if (p.first == typeId) result << p;

        return result;
    }

    bool Class::containsParent(const QString &typeId)
    {
        auto it = std::find_if(m_Parents.begin(), m_Parents.end(),
                               [&](const Parent &p) { return p.first == typeId; });
        return (it != m_Parents.end());
    }

    void Class::removeParent(const QString &typeId)
    {
        auto it = std::find_if(m_Parents.begin(), m_Parents.end(),
                               [&](const Parent &p) { return p.first == typeId; });
        if (it != m_Parents.end()) m_Parents.erase(it);
    }

    bool Class::anyParents() const
    {
        return !m_Parents.isEmpty();
    }

    ParentsList Class::parents() const
    {
        return m_Parents;
    }

    void Class::addMethod(SharedMethod method)
    {
        m_Methods << method;
    }

    MethodsList Class::getMethod(const QString &name)
    {
        MethodsList result;
        for (auto && m : m_Methods)
            if (m->name() == name) result << m;

        return result;
    }

    bool Class::containsMethod(const QString &name)
    {
        return !getMethod(name).isEmpty();
    }

    void Class::removeMethods(const QString &name)
    {
        auto methods = getMethod(name);
        for (auto &&m : methods) m_Methods.removeAll(m);
    }

    void Class::removeMethod(const SharedMethod &method)
    {
        m_Methods.removeOne(method);
    }

    MethodsList Class::methods() const
    {
        return m_Methods;
    }

    bool Class::anyMethods() const
    {
        return !m_Methods.isEmpty();
    }

    bool Class::containsMethods(Section section) const
    {
        bool result(false);

        for (auto &&method : m_Methods) {
            if (method->section() == section) {
                result = true;
                break;
            }
        }

        return result;
    }

    MethodsList Class::methods(Section section) const
    {
        MethodsList result;

        for (auto &&method : m_Methods)
            if (method->section() == section) result << method;

        return result;
    }

    SharedField Class::addField(const QString &name, const QString &typeId)
    {
        auto field = std::make_shared<Field>(name, typeId);
        if (containsField(name)) removeField(name);
        m_Fields.append(field);

        return field;
    }

    SharedField Class::getField(const QString &name) const
    {
        auto it = std::find_if(m_Fields.begin(), m_Fields.end(),
                               [&name](const SharedField &f){ return f->name() == name; });
        return (it != m_Fields.end() ? *it : nullptr);
    }

    bool Class::containsField(const QString &name) const
    {
        return getField(name).operator bool();
    }

    void Class::removeField(const QString &name)
    {
        auto f = getField(name);
        if (f) m_Fields.removeAt(m_Fields.indexOf(f));
    }

    FieldsList Class::fields() const
    {
        return m_Fields;
    }

    bool Class::anyFields() const
    {
        return !m_Fields.empty();
    }

    bool Class::containsFields(Section section) const
    {
        bool result(false);

        for (auto &&field : m_Fields) {
            if (field->section() == section) {
                result = true;
                break;
            }
        }

        return result;
    }

    FieldsList Class::fields(Section section) const
    {
        FieldsList result;

        for (auto &&field : m_Fields)
            if (field->section() == section) result << field;

        return result;
    }

    Kind Class::kind() const
    {
        return m_Kind;
    }

    void Class::setKind(Kind kind)
    {
        m_Kind = kind;
    }

    bool Class::isFinal() const
    {
        return m_FinalStatus;
    }

    void Class::setFinalStatus(bool status)
    {
        m_FinalStatus = status;
    }

    QJsonObject Class::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Kind", m_Kind);
        result.insert("Final status", m_FinalStatus);

        QJsonArray parents;
        QJsonObject parent;
        for (auto &&p: m_Parents) {
            parent.insert("Id", p.first);
            parent.insert("Section", p.second);
            parents.append(parent);
        }
        result.insert("Parents", parents);

        QJsonArray methods;
        for (auto &&value : m_Methods) methods.append(value->toJson());
        result.insert("Methods", methods);

        QJsonArray fields;
        for (auto &&value : m_Fields) fields.append(value->toJson());
        result.insert("Fields", fields);

        return result;
    }

    void Class::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Kind", errorList, [&src, this](){
            m_Kind = static_cast<Kind>(src["Kind"].toInt());
        });
        utility::checkAndSet(src, "Final status", errorList, [&src, this](){
            m_FinalStatus = src["Final status"].toBool();
        });

        m_Parents.clear();
        utility::checkAndSet(src, "Parents", errorList, [&src, &errorList, this](){
            Parent p;
            QJsonObject o;
            if (src["Parents"].isArray()) {
                for (auto &&value : src["Parents"].toArray()) {
                    o = value.toObject();
                    utility::checkAndSet(o, "Id", errorList, [&o, &p, this](){
                        p.first = o["Id"].toString();
                    });
                    utility::checkAndSet(o, "Section", errorList, [&o, &p, this](){
                        p.second = static_cast<Section>(o["Section"].toInt());
                    });
                    m_Parents.append(p);
                }
            } else {
                errorList << "Error: \"Parents\" is not array";
            }
        });

        m_Methods.clear();
        utility::checkAndSet(src, "Methods", errorList, [&src, &errorList, this](){
            if (src["Methods"].isArray()) {
                SharedMethod method;
                QJsonObject obj;
                for (auto &&value : src["Methods"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Type", errorList,
                                         [&obj, &errorList, &method, this](){
                        method = utility::makeMethod(static_cast<ClassMethodType>(obj["Type"].toInt()));
                        method->fromJson(obj, errorList);
                        m_Methods << method;
                    });
                }
            } else {
                errorList << "Error: \"Methods\" is not array";
            }
        });

        m_Fields.clear();
        utility::checkAndSet(src, "Fields", errorList, [&src, &errorList, this](){
            if (src["Fields"].isArray()) {
                SharedField field;
                for (auto &&value : src["Fields"].toArray()) {
                    field = std::make_shared<Field>();
                    field->fromJson(value.toObject(), errorList);
                    m_Fields << field;
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

} // namespace entity
