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
#include <QDebug>

#include <models/componentsmodel.h>

#include "componentsmaker.h"

namespace gui {

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
        , m_ComponentsMaker(std::make_unique<ComponentsMaker>())
        , m_DisplayPart(models::DisplayPart::Invalid)
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
    const ComponentsMaker &SignatureEditDelegate::maker() const
    {
        return *m_ComponentsMaker.get();
    }

    /**
     * @brief SignatureEditDelegate::maker
     * @return
     */
    ComponentsMaker &SignatureEditDelegate::maker()
    {
        return *m_ComponentsMaker.get();
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
        auto model = static_cast<const models::ComponentsModel*>(index.model());
        auto le = static_cast<QLineEdit*>(editor);

        le->setText(model->data(index, models::ComponentsModel::ShortSignature).toString());
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
        auto currentSignature = model->data(index, models::ComponentsModel::ShortSignature).toString();
        if (currentSignature != le->text()) {
            auto e = static_cast<QLineEdit*>(editor);
            auto optionalEntity = m_ComponentsMaker->makeComponent(e->text(), m_DisplayPart);
            if (optionalEntity.second)
                qDebug() << "ok";
            else
                qDebug() << "Fail: " << optionalEntity.first;

            e->setStyleSheet("");

            qDebug() << "set model data";
        } else {
            qDebug() << "data is not changed";
        }
    }

    /**
     * @brief SignatureEditDelegate::onTextEdited
     */
    void SignatureEditDelegate::onTextEdited()
    {
        auto editor = qobject_cast<QLineEdit*>(sender());
        if (m_ComponentsMaker->signatureValid(editor->text(), m_DisplayPart))
            editor->setStyleSheet(validSignature);
        else
            editor->setStyleSheet(invalidSignature);
    }

} // namespace gui
