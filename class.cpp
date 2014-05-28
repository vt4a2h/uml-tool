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
        return *m_Parents.insert(typeId, std::make_pair(typeId, section));
    }

    ParentsList Class::getParents(const QString &typeId)
    {
        return m_Parents.values(typeId);
    }

    bool Class::containsParent(const QString &typeId)
    {
        return m_Parents.contains(typeId);
    }

    void Class::removeParent(const QString &typeId)
    {
        m_Parents.remove(typeId);
    }

    ParentsList Class::parents() const
    {
        return m_Parents.values();
    }

    void Class::addMethod(SharedMethod method)
    {
        m_Methods.insert(method->name(), method);
    }

    MethodsList Class::getMethod(const QString &name)
    {
        return m_Methods.values(name);
    }

    bool Class::containsMethod(const QString &name)
    {
        return m_Methods.contains(name);
    }

    void Class::removeMethods(const QString &name)
    {
        m_Methods.remove(name);
    }

    void Class::removeMethod(const SharedMethod &method)
    {
        m_Methods.remove(method->name(), method);
    }

    MethodsList Class::methods() const
    {
        return m_Methods.values();
    }

    bool Class::containsMethods(Section section) const
    {
        bool result(false);

        for (auto &&method : m_Methods.values()) {
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

        for (auto &&method : m_Methods.values())
            if (method->section() == section) result << method;

        return result;
    }

    SharedField Class::addField(const QString &name, const QString &typeId)
    {
        return *m_Fields.insert(name, std::make_shared<Field>(name, typeId));
    }

    SharedField Class::getField(const QString &name)
    {
        return m_Fields.value(name);
    }

    bool Class::containsField(const QString &name)
    {
        return m_Fields.contains(name);
    }

    void Class::removeField(const QString &name)
    {
        m_Fields.remove(name);
    }

    FieldsList Class::fields() const
    {
        return m_Fields.values();
    }

    bool Class::containsFields(Section section) const
    {
        bool result(false);

        for (auto &&field : m_Fields.values()) {
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

        for (auto &&field : m_Fields.values())
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
        for (auto &&id : m_Parents.keys()) {
            parent.insert("Id", m_Parents[id].first);
            parent.insert("Section", m_Parents[id].second);
            parents.append(parent);
        }
        result.insert("Parents", parents);

        QJsonArray methods;
        for (auto &&value : m_Methods.values()) methods.append(value->toJson());
        result.insert("Methods", methods);

        QJsonArray fields;
        for (auto &&value : m_Fields.values()) fields.append(value->toJson());
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
                    m_Parents.insert(p.first, p);
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
                        m_Methods.insertMulti(method->name(), method);
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
                    m_Fields.insert(field->name(), field);
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

} // namespace entity
