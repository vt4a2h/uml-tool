#include "helpfunctions.h"

#include "enums.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "association.h"
#include "dependency.h"
#include "generalization.h"
#include "multiplyassociation.h"
#include "realization.h"
#include "relation.h"
#include "classmethod.h"
#include "templateclassmethod.h"
#include "enum.h"
#include "database.h"
#include "scope.h"

#include <functional>

#include <QUuid>
#include <QString>
#include <QJsonObject>
#include <QStringList>

namespace utility {

    QString genId()
    {
        return QUuid::createUuid().toString();
    }

    void checkAndSet(const QJsonObject &object, const QString &key, QStringList &lst, std::function<void ()> func)
    {
        object.contains(key) ? func() : lst.append(QObject::tr("Key \"%1\" not found!").arg(key));
    }

    std::shared_ptr<entity::Type> makeType(entity::UserType type)
    {
        using namespace entity;
        switch (type) {
            case BasicType:
                return std::make_shared<Type>();
            case UserClassType:
                return std::make_shared<Class>();
            case TemplateClassType:
                return std::make_shared<TemplateClass>();
            case UnionType:
                return std::make_shared<Union>();
            case EnumType:
                return std::make_shared<Enum>();
            default:
                return std::make_shared<Type>();
        }
    }

    std::shared_ptr<relationship::Relation> makeRelation(relationship::RelationType relation)
    {
        using namespace relationship;
        switch (relation) {
            case AggregationRelation:
            case CompositionRelation:
            case MultiRelation:
                return std::make_shared<MultiplyAssociation>();
            case AssociationRelation:
                return std::make_shared<Association>();
            case DependencyRelation:
                return std::make_shared<Dependency>();
            case GeneralizationRelation:
                return std::make_shared<Generalization>();
            case RealizationRelation:
                return std::make_shared<Realization>();
            case SimpleRelation:
                return std::make_shared<Relation>();
            default:
                return std::make_shared<Relation>();
        }
    }

    std::shared_ptr<entity::ClassMethod> makeMethod(entity::ClassMethodType methodType)
    {
        using namespace entity;
        switch (methodType) {
            case SimpleMethod:
                return std::make_shared<ClassMethod>();
            case TemplateMethod:
                return std::make_shared<TemplateClassMethod>();
            default:
                return std::make_shared<ClassMethod>();
        }
    }

    QString fieldKeywordToString(entity::FieldKeyword keyword)
    {
        switch (keyword) {
        case entity::Volatile:
            return "volatile";
        case entity::Mutable:
            return "mutable";
        case entity::FieldStatic:
            return "static";
        default:
            return "unknown";
        }
    }

    QString methodLhsIdToString(entity::LhsIdentificator id)
    {
        switch (id) {
        case entity::Explicit:
            return "explicit";
        case entity::Inline:
            return "inline";
        case entity::MethodStatic:
            return "static";
        case entity::Virtual:
            return "virtual";
        case entity::Friend:
            return "friend";
        default:
            return "unknown";
        }
    }

    QString methodRhsIdToString(entity::RhsIdentificator id)
    {
        switch (id) {
        case entity::None:
            return "";
        case entity::Override:
            return "override";
        case entity::Final:
            return "final";
        case entity::Delete:
            return "= delete";
        case entity::Default:
            return "= default";
        case entity::PureVirtual:
            return "= 0";
        default:
            return "unknown";
        }
    }

    QString sectionToString(entity::Section section)
    {
        switch (section) {
        case entity::Public:
            return "public";
        case entity::Protected:
            return "protected";
        case entity::Private:
            return "private";
        default:
            return "unknown";
        }
    }

}
