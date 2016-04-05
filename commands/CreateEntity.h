/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/04/2015.
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
#pragma once

#include <QPointF>
#include <QGraphicsScene>

#include <entity/entity_types.hpp>
#include <entity/EntityFactory.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/extendedtype.h>
#include <entity/templateclass.h>

#include <gui/graphics/entity.h>

#include <models/models_types.hpp>
#include <models/applicationmodel.h>

#include <utility/helpfunctions.h>

#include "constants.h"
#include "enums.h"
#include "basecommand.h"
#include "qthelpers.h"

class QGraphicsItem;
class QGraphicsScene;

namespace commands {

//    const QMap<size_t, QString> hashName = {
//        { entity::Type::staticHashType(), BaseCommand::tr("type") },
//        { entity::Class::staticHashType(), BaseCommand::tr("class (struct)") },
//        { entity::Enum::staticHashType(), BaseCommand::tr("enum") },
//        { entity::Union::staticHashType(), BaseCommand::tr("union") },
//        { entity::ExtendedType::staticHashType(), BaseCommand::tr("type alias") },
//        { entity::TemplateClass::staticHashType(), BaseCommand::tr("template class") },
//    };

//    /// The class CreateEntity
//    template<class Type>
//    class CreateEntity : public BaseCommand
//    {
//    public:
//        CreateEntity(const models::SharedApplicationModel &model, const common::ID &scopeID,
//                     QGraphicsScene &scene, const QPointF &pos, QUndoCommand *parent = nullptr)
//            : BaseCommand(tr("Add %1").arg(hashName[typeid(Type).hash_code()]), parent)
//            , m_Model(model)
//            , m_ProjectName(model && model->currentProject() ? model->currentProject()->name()
//                                                             : "")
//            , m_ScopeID(scopeID)
//            , m_Pos(pos)
//            , m_Scene(scene)
//        {
//        }

//        void redo() override
//        {
//            if (m_Done) {
//                m_Model->addExistsType(m_ProjectName, m_ScopeID, m_TypeItem);

//                m_Scene.addItem(m_Item);
//                static_cast<graphics::Entity *>(m_Item)->setTypeObject(m_TypeItem);
//            } else {
////                auto const& factory = entity::EntityFactory::instance();
//                // FIXME:
////                auto entity = factory.makeEntity<Type>(m_Model, m_ScopeID, m_Scene, m_Pos);

////                m_TypeItem = G_ASSERT(entity.first);
////                m_Item = G_ASSERT(entity.second);

//                m_Done = true;
//            }

//            m_CleaningRequired = false;
//        }

//        void undo() override
//        {
//            Q_ASSERT(m_Model);
//            Q_ASSERT(m_TypeItem);

//            m_Scene.removeItem(m_Item);
//            m_Model->removeType(m_ProjectName, m_ScopeID, m_TypeItem->id());
//            m_CleaningRequired = true;
//        }

//        std::shared_ptr<Type> entity() const { return m_TypeItem; }

//    protected:
//        void cleanup() override
//        {
//            if (m_CleaningRequired && m_Item)
//                delete m_Item;
//        }

//    private:
//        bool m_CleaningRequired = false;
//        models::SharedApplicationModel m_Model;
//        QString m_ProjectName;
//        common::ID m_ScopeID;
//        QPointF m_Pos;
//        QGraphicsItem  * m_Item = nullptr;
//        QGraphicsScene & m_Scene;
//        std::shared_ptr<Type> m_TypeItem = nullptr;
//    };

//    using MakeClass = CreateEntity<entity::Class>;
//    using MakeAlias = CreateEntity<entity::ExtendedType>;
//    using MakeEnum  = CreateEntity<entity::Enum>;
//    using MakeUnion = CreateEntity<entity::Union>;
//    using MakeTemplate = CreateEntity<entity::TemplateClass>;

} // namespace commands
