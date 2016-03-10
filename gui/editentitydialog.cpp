/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18/03/2015.
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
#include "editentitydialog.h"
#include "ui_editentitydialog.h"

#include <QMessageBox>

#include <entity/type.h>
#include <entity/scope.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>
#include <entity/classmethod.h> // TODO: remove
#include <entity/components/componentsmaker.h>

#include <models/applicationmodel.h>
#include <models/componentsmodel.h>

#include <project/project.h>

#include <db/projectdatabase.h>

#include <utility/helpfunctions.h>

#include <commands/renameentity.h>
#include <commands/movetypetootherscope.h>
#include <commands/addcomponentscommands.h>
#include <commands/removecomponentscommands.h>

#include <translation/signaturemaker.h>

#include <helpers/generatorid.h>

#include "componentseditdelegate.h"
#include "signatureeditdelegate.h"
#include "editmethoddialog.h"

#include "enums.h"
#include "constants.h"

namespace gui {

    namespace {
        const QString newFieldName   = "newField"  ;
        const QString newMethodName  = "newMethod" ;
        const QString newElementName = "newElement";


        const QVector<QPair<QString, size_t>> names =
            {
                { EditEntityDialog::tr("Type"), entity::Type::staticHashType() },
                { EditEntityDialog::tr("Enum"), entity::Enum::staticHashType() },
                { EditEntityDialog::tr("Union"), entity::Union::staticHashType() },
                { EditEntityDialog::tr("Class"), entity::Class::staticHashType() },
                { EditEntityDialog::tr("Template class"), entity::TemplateClass::staticHashType() },
                { EditEntityDialog::tr("Extended type"), entity::ExtendedType::staticHashType() }
            };

        const QVector<QPair<QString, QString>> componentsNames =
            {
                { EditEntityDialog::tr("Properties"), EditEntityDialog::tr("property") },
                { EditEntityDialog::tr("Methods")   , EditEntityDialog::tr("method")   },
                { EditEntityDialog::tr("Fields")    , EditEntityDialog::tr("field")    },
                { EditEntityDialog::tr("Elements")  , EditEntityDialog::tr("element")  },
            };

        const QMap<size_t, QStringList> componentsPreset =
            {
                { entity::Class::staticHashType()        , { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { entity::TemplateClass::staticHashType(), { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { entity::Union::staticHashType()       , { EditEntityDialog::tr("Fields")   } },
                { entity::Enum::staticHashType()        , { EditEntityDialog::tr("Elements") } },
                { entity::Type::staticHashType()        , {} },
                { entity::ExtendedType::staticHashType(), {} }
            };

        const QVector<QPair<QString, models::DisplayPart>> display =
            {
                { EditEntityDialog::tr("Properties"), models::DisplayPart::Properties },
                { EditEntityDialog::tr("Methods")   , models::DisplayPart::Methods    },
                { EditEntityDialog::tr("Fields")    , models::DisplayPart::Fields     },
                { EditEntityDialog::tr("Elements")  , models::DisplayPart::Elements   },
            };

        const QMap<models::DisplayPart, std::function<void(const models::SharedClassComponentsModel &model,
                                                           QUndoStack * stack)>> componentMakers =
            {
                { models::DisplayPart::Methods,
                  [](const models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new commands::AddMethod(model));
                } },

                { models::DisplayPart::Fields,
                  [](const models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new commands::AddField(model));
                } },

                { models::DisplayPart::Elements,
                  [](const models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new commands::AddElement(model));
                } },

                { models::DisplayPart::Properties,
                  [](const models::SharedClassComponentsModel &model, QUndoStack * stack){ model->addProperty(); Q_UNUSED(stack); } },
            };

        QMap<models::DisplayPart, std::function<void(const QModelIndex &index)>> componentDeleters;

        enum class ComponentCustomRoles : int {
            Single = 300,
            Display,
        };

        void setButtonLabel(QPushButton *btn, QListWidgetItem *item)
        {
            Q_ASSERT(btn); Q_ASSERT(item);

            btn->setText(
                EditEntityDialog::tr("&Add new %1").arg(item->data(int(ComponentCustomRoles::Single)).toString())
            );
        }

        QListWidgetItem *itemByName(const QString &name, const QListWidget *wgt)
        {
            auto items = wgt->findItems(name, Qt::MatchFixedString);
            Q_ASSERT(items.count() == 1);

            return items.first();
        }

        void configure(const QScopedPointer<Ui::EditEntityDialog> &ui, const entity::SharedType &type)
        {
            for (int i = 0; i < ui->lstMembers->count(); ++i)
                ui->lstMembers->item(i)->setHidden(true);

            for (auto &&name : componentsPreset[type->hashType()])
                itemByName(name, ui->lstMembers)->setHidden(false);
        }

        template<class Command, class Component>
        void addRemoveCommand(const models::SharedClassComponentsModel &model, const QModelIndex &index, QUndoStack * stack)
        {
            auto component = model->data(index, models::ComponentsModel::InternalData);
            stack->push(new Command(model, component.value<Component>()));
        }
    }

    /**
     * @brief EditEntityDialog::EditEntityDialog
     * @param parent
     */
    EditEntityDialog::EditEntityDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::EditEntityDialog)
        , m_EditMethodDialog(new EditMethodDialog(this))
        , m_CommandsStack(new QUndoStack(this))
        , m_ComponentsModel(std::make_shared<models::ComponentsModel>(nullptr))
    {
        ui->setupUi(this);

        for(auto &&pair : names)
            ui->cbType->addItem(pair.first, QVariant::fromValue(pair.second));
        for (auto &&p : componentsNames)
            itemByName(p.first, ui->lstMembers)->setData(int(ComponentCustomRoles::Single), p.second);
        for (auto &&p : display)
            itemByName(p.first, ui->lstMembers)->setData(int(ComponentCustomRoles::Display),
                                                         QVariant::fromValue(p.second));

        ui->lstMembers->setCurrentRow(0);
        ui->viewMembers->setModel(m_ComponentsModel.get());

        auto componentsEditDelegat = new ComponentsEditDelegate(this);
        connect(componentsEditDelegat, &ComponentsEditDelegate::editButtonClicked,
                this, &EditEntityDialog::onEditComponentClicked);
        connect(componentsEditDelegat, &ComponentsEditDelegate::deleteButtonClicked,
                this, &EditEntityDialog::onDeleteComponentClicked);
        ui->viewMembers->setItemDelegateForColumn(models::ComponentsModel::Buttons, componentsEditDelegat);
        connect(m_ComponentsModel.get(), &models::ComponentsModel::showButtons,
                [view = ui->viewMembers](auto &&indexes) {
                    for (auto &&index: indexes) {
                        Q_ASSERT(index.isValid());
                        view->openPersistentEditor(index);
                    }
                });

        m_SignatureEditDelegate = std::make_unique<SignatureEditDelegate>();
        ui->viewMembers->setItemDelegateForColumn(models::ComponentsModel::ShortSignature, m_SignatureEditDelegate.get());

        QHeaderView * header = ui->viewMembers->horizontalHeader();
        header->setSectionResizeMode(models::ComponentsModel::ShortSignature, QHeaderView::Stretch);
        header->setSectionResizeMode(models::ComponentsModel::Buttons, QHeaderView::Fixed);
        ui->viewMembers->setColumnWidth(models::ComponentsModel::Buttons, componentsEditDelegat->size().width());

        connect(ui->pbAccept, &QPushButton::clicked, this, &EditEntityDialog::onAccepted);
        connect(ui->pbReject, &QPushButton::clicked, this, &EditEntityDialog::onRejected);
        connect(ui->pbCreateScope, &QPushButton::clicked, this, &EditEntityDialog::needNewScope);
        connect(ui->pbNewComponent, &QPushButton::clicked, this, &EditEntityDialog::onNewComponentClicked);
        connect(ui->lstMembers, &QListWidget::currentItemChanged, this, &EditEntityDialog::onCurrentItemChange);

        connect(m_CommandsStack.data(), &QUndoStack::canRedoChanged, ui->tbRedo, &QToolButton::setEnabled);
        connect(m_CommandsStack.data(), &QUndoStack::canUndoChanged, ui->tbUndo, &QToolButton::setEnabled);
        connect(ui->tbRedo, &QToolButton::clicked, m_CommandsStack.data(), &QUndoStack::redo);
        connect(ui->tbUndo, &QToolButton::clicked, m_CommandsStack.data(), &QUndoStack::undo);
    }

    /**
     * @brief EditEntityDialog::~EditEntityDialog
     */
    EditEntityDialog::~EditEntityDialog()
    {
    }

    /**
     * @brief EditEntityDialog::setData
     * @param model
     * @param id
     */
    void EditEntityDialog::setData(const models::SharedApplicationModel &model, const entity::SharedType &type)
    {
        Q_ASSERT(model);
        Q_ASSERT(type);

        m_ApplicationModel = model;
        m_Project = m_ApplicationModel->currentProject();
        m_Scope = m_Project->database()->getScope(type->scopeId());
        m_Type = type;

        components::ComponentsMaker &maker = m_SignatureEditDelegate->maker();
        maker.setModel(m_ApplicationModel);
        maker.setScope(m_Scope);
        maker.setEntity(m_Type);

        // Temporary {
        if (m_Type->hashType() == entity::Class::staticHashType()) {
            auto cl = std::static_pointer_cast<entity::Class>(m_Type);

            if (cl->methods().isEmpty()) {
                auto intType = m_Scope->addType("int");
                auto doubleType = m_Scope->addType("double");

                cl->makeMethod("foo")->setReturnTypeId(intType->id());
                cl->makeMethod("bar")->setReturnTypeId(doubleType->id());
                cl->makeMethod("baz");
            }

            if (cl->fields().isEmpty()) {
                cl->addField("f1", entity::GeneratorID::instance().genID());
                cl->addField("f2", entity::GeneratorID::instance().genID());
            }
        }
        // }

        auto signatureMaker = std::make_unique<translation::SignatureMaker>(
                                  m_ApplicationModel->globalDatabase(), m_Project->database(),
                                  m_Scope, m_Type
                              );
        m_ComponentsModel->setSignatureMaker(std::move(signatureMaker));
        m_ComponentsModel->setComponents(m_Type);

        fillMaps();
    }

    /**
     * @brief EditEntityDialog::onScopeAdded
     * @param scope
     */
    void EditEntityDialog::onScopeAdded(const entity::SharedScope &scope)
    {
        ui->cbScopes->addItem(scope->name(), QVariant::fromValue(scope));
    }

    /**
     * @brief EditEntityDialog::showEvent
     * @param ev
     */
    void EditEntityDialog::showEvent(QShowEvent *ev)
    {
        init();

        QDialog::showEvent(ev);
    }

    /**
     * @brief EditEntityDialog::closeEvent
     * @param ev
     */
    void EditEntityDialog::closeEvent(QCloseEvent *ev)
    {
        clear();

        QDialog::closeEvent(ev);
    }

    /**
     * @brief EditEntityDialog::onAccepted
     */
    void EditEntityDialog::onAccepted()
    {
        auto &&stack = m_Project->commandsStack();
        stack->beginMacro(tr("Edit entity dialog changes"));

        // Check name
        QString newName = ui->leName->text();
        if (m_Type->name() != newName)
            stack->push(std::make_unique<commands::RenameEntity>(m_Type, newName).release());

        // Check scope
        auto scope = ui->cbScopes->currentData().value<entity::SharedScope>();
        Q_ASSERT(scope);
        if (scope->id() != m_Type->scopeId())
        {
            auto db = m_Project->database();

            auto srcScope = db->getScope(m_Type->scopeId());
            Q_ASSERT(srcScope);

            auto dstScope = db->getScope(scope->id());
            Q_ASSERT(dstScope);

            Q_ASSERT(m_ApplicationModel);
            auto cmd = std::make_unique<commands::MoveTypeToOtherScope>(m_Type, m_ApplicationModel, srcScope, dstScope);
            stack->push(cmd.release());
        }

        stack->endMacro();

        clear();
        accept();
    }

    /**
     * @brief EditEntityDialog::onRejected
     */
    void EditEntityDialog::onRejected()
    {
        clear();
        reject();
    }

    /**
     * @brief EditEntityDialog::onNewComponentClicked
     */
    void EditEntityDialog::onNewComponentClicked()
    {
        componentMakers[m_ComponentsModel->display()](m_ComponentsModel, m_CommandsStack.data());
    }

    /**
     * @brief EditEntityDialog::onCurrentItemChange
     * @param current
     * @param previous
     */
    void EditEntityDialog::onCurrentItemChange(QListWidgetItem *current, QListWidgetItem *previous)
    {
        Q_UNUSED(previous);
        setButtonLabel(ui->pbNewComponent, current);
        m_ComponentsModel->setDisplay(current->data(int(ComponentCustomRoles::Display)).value<models::DisplayPart>());
    }

    /**
     * @brief EditEntityDialog::onEditComponentClicked
     * @param index
     */
    void EditEntityDialog::onEditComponentClicked(const QModelIndex &index)
    {
        // TODO: cover other components (fields, enum elements and properties)
        if (m_ComponentsModel->display() == models::DisplayPart::Methods) {
            auto data = index.data(models::ComponentsModel::InternalData);

            Q_ASSERT(data.isValid());
            Q_ASSERT(data.canConvert<entity::SharedMethod>());

            m_EditMethodDialog->setCurrentMethod(data.value<entity::SharedMethod>());
            m_EditMethodDialog->show(); // NOTE: maybe use exec to handle result
        }
    }

    /**
     * @brief EditEntityDialog::onDeleteComponentClicked
     * @param index
     */
    void EditEntityDialog::onDeleteComponentClicked(const QModelIndex &index)
    {
        Q_ASSERT(index.isValid());
        auto result = QMessageBox::question(this,
                                            tr("Removing component"),
                                            tr("Component will be removed.\n Are you sure that you like to remove it?"),
                                            QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            componentDeleters[m_ComponentsModel->display()](index);
        }
    }

    /**
     * @brief EditEntityDialog::init
     */
    void EditEntityDialog::init()
    {
        Q_ASSERT(m_Type);
        Q_ASSERT(m_Scope);
        Q_ASSERT(m_Project);

        ui->leName->setText(m_Type->name());

        setType();

        auto &&db = m_Project->database();
        for(auto &&scope : db->scopes())
            ui->cbScopes->addItem(scope->name(), QVariant::fromValue(scope));
        setScope();

        setButtonLabel(ui->pbNewComponent, ui->lstMembers->currentItem());
        configure(ui, m_Type);

        ui->tbRedo->setEnabled(false);
        ui->tbUndo->setEnabled(false);
    }

    /**
     * @brief EditEntityDialog::clear
     */
    void EditEntityDialog::clear()
    {
        ui->leName->clear();
        ui->cbScopes->clear();
        m_ComponentsModel->clear();
        m_CommandsStack->clear();

        componentDeleters.clear();
    }

    /**
     * @brief EditEntityDialog::setType
     */
    void EditEntityDialog::setType()
    {
        for (int i = 0; i < ui->cbType->count(); ++i) {
            if (m_Type->hashType() == ui->cbType->itemData(i).value<size_t>()) {
                ui->cbType->setCurrentIndex(i);
                break;
            }
        }
    }

    /**
     * @brief EditEntityDialog::setScope
     */
    void EditEntityDialog::setScope()
    {
        for (int i = 0; i < ui->cbScopes->count(); ++i) {
            if (m_Scope == ui->cbScopes->itemData(i).value<entity::SharedScope>()) {
                ui->cbScopes->setCurrentIndex(i);
                break;
            }
        }
    }

    /**
     * @brief EditEntityDialog::fillMaps
     */
    void EditEntityDialog::fillMaps() const
    {
        componentDeleters[models::DisplayPart::Elements] =
            [this](const QModelIndex &index){
                addRemoveCommand<commands::RemoveElement, entity::SharedEnumarator>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[models::DisplayPart::Fields] =
            [this](const QModelIndex &index){
                addRemoveCommand<commands::RemoveField, entity::SharedField>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[models::DisplayPart::Methods] =
            [this](const QModelIndex &index){
                addRemoveCommand<commands::RemoveMethod, entity::SharedMethod>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[models::DisplayPart::Properties] = [](const QModelIndex &){ /*Implement*/ };
    }

} // namespace gui
