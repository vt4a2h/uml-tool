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

    namespace {
        using KeywordsString = const std::map<entity::FieldKeyword,     QString>;
        using LhsIdString    = const std::map<entity::LhsIdentificator, QString>;
        using RhsIdString    = const std::map<entity::RhsIdentificator, QString>;
        using SectionString  = const std::map<entity::Section,          QString>;

        KeywordsString kKeywords { {entity::Volatile, "volatile"}, {entity::Mutable, "mutable"},
                                   {entity::FieldStatic, "static"} };

        LhsIdString    kLhsId    { {entity::Explicit, "explicit"},   {entity::Inline, "inline"},
                                   {entity::MethodStatic, "static"}, {entity::Virtual, "virtual"},
                                   {entity::Friend, "friend"} };

        RhsIdString    kRhsId    { {entity::None, ""}, {entity::Override, "override"}, {entity::Final, "final"},
                                   {entity::Delete, "= delete"}, {entity::Default, "= default"},
                                   {entity::PureVirtual, "= 0"} };

        SectionString  kSection  { {entity::Public, "public"}, {entity::Protected, "protected"},
                                   {entity::Private, "private"} };

        using MakerT  = std::function<entity::SharedType()>;
        using MakersT = const std::map<entity::UserType, MakerT>;
        using MakerM  = std::function<entity::SharedMethod()>;
        using MakersM = const std::map<entity::ClassMethodType, MakerM>;
        using MakerR  = std::function<relationship::SharedRelation()>;
        using MakersR = const std::map<relationship::RelationType, MakerR>;

        MakersT kType = { {entity::BasicType,        [](){ return std::make_shared<entity::Type>();          }},
                         {entity::UserClassType,     [](){ return std::make_shared<entity::Class>();         }},
                         {entity::TemplateClassType, [](){ return std::make_shared<entity::TemplateClass>(); }},
                         {entity::UnionType,         [](){ return std::make_shared<entity::Union>();         }},
                         {entity::EnumType,          [](){ return std::make_shared<entity::Enum>();          }}
                        };

        MakersR kRelation = {
            {relationship::AggregationRelation,    [](){ return std::make_shared<relationship::MultiplyAssociation>(); }},
            {relationship::CompositionRelation,    [](){ return std::make_shared<relationship::MultiplyAssociation>(); }},
            {relationship::MultiRelation,          [](){ return std::make_shared<relationship::MultiplyAssociation>(); }},
            {relationship::AssociationRelation,    [](){ return std::make_shared<relationship::Association>();         }},
            {relationship::DependencyRelation ,    [](){ return std::make_shared<relationship::Dependency>();          }},
            {relationship::GeneralizationRelation, [](){ return std::make_shared<relationship::Generalization>();      }},
            {relationship::RealizationRelation,    [](){ return std::make_shared<relationship::Realization>();         }},
            {relationship::SimpleRelation,         [](){ return std::make_shared<relationship::Relation>();            }}
        };

        MakersM kMethod = { {entity::SimpleMethod,   [](){ return std::make_shared<entity::ClassMethod>();         }},
                            {entity::TemplateMethod, [](){ return std::make_shared<entity::TemplateClassMethod>(); }}
                          };
    }

    std::shared_ptr<entity::Type> makeType(entity::UserType type)
    {
        MakerT defaultMaker([](){ return std::make_shared<entity::Type>(); });

        return mapSearchHelper(kType, type, defaultMaker)();
    }

    std::shared_ptr<relationship::Relation> makeRelation(relationship::RelationType relation)
    {
        MakerR defaultMaker([](){ return std::make_shared<relationship::Relation>(); });

        return mapSearchHelper(kRelation, relation, defaultMaker)();
    }  

    std::shared_ptr<entity::ClassMethod> makeMethod(entity::ClassMethodType methodType)
    {
        MakerM defaultMaker([](){ return std::make_shared<entity::ClassMethod>(); });

        return mapSearchHelper(kMethod, methodType, defaultMaker)();
    }

    QString fieldKeywordToString(entity::FieldKeyword keyword)
    {
        return mapSearchHelper(kKeywords, keyword, QString("unknown"));
    }

    QString methodLhsIdToString(entity::LhsIdentificator id)
    {
        return mapSearchHelper(kLhsId, id, QString("unknown"));
    }

    QString methodRhsIdToString(entity::RhsIdentificator id)
    {
        return mapSearchHelper(kRhsId, id, QString("unknown"));
    }

    QString sectionToString(entity::Section section)
    {
        return mapSearchHelper(kSection, section, QString("unknown"));
    }

}
