/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 11/07/2015.
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
#include "signatureeditdelegate.h"

#include <QLineEdit>
#include <QMessageBox>

#include <Models/ComponentsModel.h>

#include <Entity/Components/componentsmaker.h>
#include <Entity/Components/componentsignatureparser.h>

namespace GUI {

    namespace {
        const QString validSignature   = "background-color: rgb(111, 255, 111);";
        const QString invalidSignature = "background-color: rgb(255, 111, 111);";
    }

    /**
     * @brief SignatureEditDelegate::SignatureEditDelegate
     * @param parent
     */
    SignatureEditDelegate::SignatureEditDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
        , m_ComponentsMaker(std::make_unique<Components::ComponentsMaker>())
        , m_SignatureParser(std::make_unique<Components::ComponentSignatureParser>())
        , m_DisplayPart(Models::DisplayPart::Invalid)
    {}

    /**
     * @brief SignatureEditDelegate::~SignatureEditDelegate
     */
    SignatureEditDelegate::~SignatureEditDelegate()
    {}

    /**
     * @brief SignatureEditDelegate::maker
     * @return
     */
    const Components::ComponentsMaker &SignatureEditDelegate::maker() const
    {
        return *m_ComponentsMaker;
    }

    /**
     * @brief SignatureEditDelegate::maker
     * @return
     */
    Components::ComponentsMaker &SignatureEditDelegate::maker()
    {
        return *m_ComponentsMaker;
    }

    /**
     * @brief SignatureEditDelegate::createEditor
     * @param parent
     * @param option
     * @param index
     * @return
     */
    QWidget *SignatureEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);

        auto le = new QLineEdit(parent);
        connect(le, &QLineEdit::textEdited, this, &SignatureEditDelegate::onTextEdited);

        return le;
    }

    /**
     * @brief SignatureEditDelegate::setEditorData
     * @param editor
     * @param index
     */
    void SignatureEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        Q_ASSERT(editor);
        auto model = static_cast<const Models::ComponentsModel*>(index.model());
        auto le = static_cast<QLineEdit*>(editor);

        le->setText(model->data(index, Models::ComponentsModel::ShortSignature).toString());
        le->setStyleSheet(validSignature);
        m_DisplayPart = model->display();
    }

    /**
     * @brief SignatureEditDelegate::setModelData
     * @param editor
     * @param model
     * @param index
     */
    void SignatureEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        Q_ASSERT(editor);
        auto le = static_cast<QLineEdit*>(editor);
        auto currentSignature = model->data(index, Models::ComponentsModel::ShortSignature).toString();
        if (currentSignature != le->text()) {
            // Parse operation called each onTextEdited()
            const auto &tokens = m_SignatureParser->tokens();
            auto optionalEntity = m_ComponentsMaker->makeComponent(tokens, m_DisplayPart);

            if (optionalEntity.resultEntity){
                model->setData(index, QVariant::fromValue(optionalEntity.resultEntity),
                               Models::ComponentsModel::UpdateSignature);
            } else
                QMessageBox::warning(le, tr("Signature changed error"), optionalEntity.errorMessage, QMessageBox::Ok);
        }
    }

    /**
     * @brief SignatureEditDelegate::onTextEdited
     */
    void SignatureEditDelegate::onTextEdited()
    {
        auto editor = qobject_cast<QLineEdit*>(sender());
        if (m_SignatureParser->parse(editor->text(), m_DisplayPart))
            editor->setStyleSheet(validSignature);
        else
            editor->setStyleSheet(invalidSignature);
    }

} // namespace gui
