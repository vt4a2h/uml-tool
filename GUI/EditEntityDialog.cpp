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
#include "EditEntityDialog.h"
#include "ui_editentitydialog.h"

#include <QMessageBox>

#include <Entity/Type.h>
#include <Entity/Scope.h>
#include <Entity/Enum.h>
#include <Entity/Union.h>
#include <Entity/Class.h>
#include <Entity/TemplateClass.h>
#include <Entity/ExtendedType.h>
#include <Entity/ClassMethod.h> // TODO: remove
#include <Entity/Components/componentsmaker.h>

#include <Models/ApplicationModel.h>
#include <Models/ComponentsModel.h>

#include <Project/Project.h>

#include <DB/ProjectDatabase.h>

#include <Utility/helpfunctions.h>

#include <Commands/RenameEntity.h>
#include <Commands/MoveTypeToAnotherScope.h>
#include <Commands/AddComponentsCommands.h>
#include <Commands/RemoveComponentsCommands.h>

#include <Translation/signaturemaker.h>

#include <Helpers/GeneratorID.h>

#include "componentseditdelegate.h"
#include "signatureeditdelegate.h"
#include "editmethoddialog.h"

#include "enums.h"
#include "Constants.h"

namespace GUI {

    namespace {
        const QString newFieldName   = "newField"  ;
        const QString newMethodName  = "newMethod" ;
        const QString newElementName = "newElement";


        const QVector<QPair<QString, size_t>> names =
            {
                { EditEntityDialog::tr("Type"), Entity::Type::staticHashType() },
                { EditEntityDialog::tr("Enum"), Entity::Enum::staticHashType() },
                { EditEntityDialog::tr("Union"), Entity::Union::staticHashType() },
                { EditEntityDialog::tr("Class"), Entity::Class::staticHashType() },
                { EditEntityDialog::tr("Template class"), Entity::TemplateClass::staticHashType() },
                { EditEntityDialog::tr("Extended type"), Entity::ExtendedType::staticHashType() }
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
                { Entity::Class::staticHashType()        , { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { Entity::TemplateClass::staticHashType(), { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { Entity::Union::staticHashType()       , { EditEntityDialog::tr("Fields")   } },
                { Entity::Enum::staticHashType()        , { EditEntityDialog::tr("Elements") } },
                { Entity::Type::staticHashType()        , {} },
                { Entity::ExtendedType::staticHashType(), {} }
            };

        const QVector<QPair<QString, Models::DisplayPart>> display =
            {
                { EditEntityDialog::tr("Properties"), Models::DisplayPart::Properties },
                { EditEntityDialog::tr("Methods")   , Models::DisplayPart::Methods    },
                { EditEntityDialog::tr("Fields")    , Models::DisplayPart::Fields     },
                { EditEntityDialog::tr("Elements")  , Models::DisplayPart::Elements   },
            };

        const QMap<Models::DisplayPart, std::function<void(const Models::SharedClassComponentsModel &model,
                                                           QUndoStack * stack)>> componentMakers =
            {
                { Models::DisplayPart::Methods,
                  [](const Models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new Commands::AddMethod(model));
                } },

                { Models::DisplayPart::Fields,
                  [](const Models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new Commands::AddField(model));
                } },

                { Models::DisplayPart::Elements,
                  [](const Models::SharedClassComponentsModel &model, QUndoStack * stack){
                      stack->push(new Commands::AddElement(model));
                } },

                { Models::DisplayPart::Properties,
                  [](const Models::SharedClassComponentsModel &model, QUndoStack * stack){ model->addProperty(); Q_UNUSED(stack); } },
            };

        QMap<Models::DisplayPart, std::function<void(const QModelIndex &index)>> componentDeleters;

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

        void configure(const QScopedPointer<Ui::EditEntityDialog> &ui, const Entity::SharedType &type)
        {
            for (int i = 0; i < ui->lstMembers->count(); ++i)
                ui->lstMembers->item(i)->setHidden(true);

            for (auto &&name : componentsPreset[type->hashType()])
                itemByName(name, ui->lstMembers)->setHidden(false);
        }

        template<class Command, class Component>
        void addRemoveCommand(const Models::SharedClassComponentsModel &model, const QModelIndex &index, QUndoStack * stack)
        {
            auto component = model->data(index, Models::ComponentsModel::InternalData);
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
        , m_ComponentsModel(std::make_shared<Models::ComponentsModel>(nullptr))
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
        ui->viewMembers->setItemDelegateForColumn(Models::ComponentsModel::Buttons, componentsEditDelegat);
        connect(m_ComponentsModel.get(), &Models::ComponentsModel::showButtons,
                [view = ui->viewMembers](auto &&indexes) {
                    for (auto &&index: indexes) {
                        Q_ASSERT(index.isValid());
                        view->openPersistentEditor(index);
                    }
                });

        m_SignatureEditDelegate = std::make_unique<SignatureEditDelegate>();
        ui->viewMembers->setItemDelegateForColumn(Models::ComponentsModel::ShortSignature, m_SignatureEditDelegate.get());

        QHeaderView * header = ui->viewMembers->horizontalHeader();
        header->setSectionResizeMode(Models::ComponentsModel::ShortSignature, QHeaderView::Stretch);
        header->setSectionResizeMode(Models::ComponentsModel::Buttons, QHeaderView::Fixed);
        ui->viewMembers->setColumnWidth(Models::ComponentsModel::Buttons, componentsEditDelegat->size().width());

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
    void EditEntityDialog::setData(const Models::SharedApplicationModel &model, const Entity::SharedType &type)
    {
        Q_ASSERT(model);
        Q_ASSERT(type);

        m_ApplicationModel = model;
        m_Project = m_ApplicationModel->currentProject();
        m_Scope = m_Project->database()->scope(type->scopeId());
        m_Type = type;

        Components::ComponentsMaker &maker = m_SignatureEditDelegate->maker();
        maker.setModel(m_ApplicationModel);
        maker.setScope(m_Scope);
        maker.setEntity(m_Type);

        auto signatureMaker = std::make_unique<Translation::SignatureMaker>(
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
    void EditEntityDialog::onScopeAdded(const Entity::SharedScope &scope)
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
            stack->push(std::make_unique<Commands::RenameEntity>(m_Type, newName).release());

        // Check scope
        auto scope = ui->cbScopes->currentData().value<Entity::SharedScope>();
        Q_ASSERT(scope);
        if (scope->id() != m_Type->scopeId())
        {
            auto db = m_Project->database();

            auto srcScope = db->scope(m_Type->scopeId());
            Q_ASSERT(srcScope);

            auto dstScope = db->scope(scope->id());
            Q_ASSERT(dstScope);

            Q_ASSERT(m_ApplicationModel);
            auto cmd = std::make_unique<Commands::MoveTypeToAnotherScope>(m_Type, m_ApplicationModel, srcScope, dstScope);
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
        m_ComponentsModel->setDisplay(current->data(int(ComponentCustomRoles::Display)).value<Models::DisplayPart>());
    }

    /**
     * @brief EditEntityDialog::onEditComponentClicked
     * @param index
     */
    void EditEntityDialog::onEditComponentClicked(const QModelIndex &index)
    {
        // TODO: cover other components (fields, enum elements and properties)
        if (m_ComponentsModel->display() == Models::DisplayPart::Methods) {
            auto data = index.data(Models::ComponentsModel::InternalData);

            Q_ASSERT(data.isValid());
            Q_ASSERT(data.canConvert<Entity::SharedMethod>());

            m_EditMethodDialog->setCurrentMethod(data.value<Entity::SharedMethod>());
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
            if (m_Scope == ui->cbScopes->itemData(i).value<Entity::SharedScope>()) {
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
        componentDeleters[Models::DisplayPart::Elements] =
            [this](const QModelIndex &index){
                addRemoveCommand<Commands::RemoveElement, Entity::SharedEnumarator>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[Models::DisplayPart::Fields] =
            [this](const QModelIndex &index){
                addRemoveCommand<Commands::RemoveField, Entity::SharedField>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[Models::DisplayPart::Methods] =
            [this](const QModelIndex &index){
                addRemoveCommand<Commands::RemoveMethod, Entity::SharedMethod>(m_ComponentsModel, index, m_CommandsStack.data());
            };
        componentDeleters[Models::DisplayPart::Properties] = [](const QModelIndex &){ /*Implement*/ };
    }

} // namespace gui
