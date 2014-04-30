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
}
