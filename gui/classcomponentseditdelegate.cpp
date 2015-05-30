/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 30/05/2015.
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
#include "classcomponentseditdelegate.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QWidget>
#include <QIcon>

namespace gui {

    namespace {

        constexpr QSize buttonSize = {32, 32};
        constexpr int space = 2;

        struct Buttons : public QWidget
        {
            Buttons(QWidget * parent = nullptr)
                : QWidget(parent)
            {
                m_Edit->setToolTip(tr("Edit component"));
                m_Edit->setIcon(QIcon(":/icons/pic/icon_component_edit.png"));
                m_Edit->resize(buttonSize);

                m_Delete->setToolTip(tr("Delete component"));
                m_Delete->setIcon(QIcon(":/icons/pic/icon_component_delete.png"));
                m_Delete->resize(buttonSize);

                auto hbl = new QHBoxLayout(this);
                hbl->setSpacing(space);
                hbl->addWidget(m_Edit);
                hbl->addWidget(m_Delete);
                hbl->setContentsMargins(0, 0, 0, 0);

                setLayout(hbl);
            }

            QPushButton * m_Edit = new QPushButton(this);
            QPushButton * m_Delete = new QPushButton(this);
        };
    }

    /**
     * @brief ClassComponentsEditDelegate::ClassComponentsEditDelegate
     * @param parent
     */
    ClassComponentsEditDelegate::ClassComponentsEditDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
    {
    }

    /**
     * @brief ClassComponentsEditDelegate::size
     * @return
     */
    QSize ClassComponentsEditDelegate::size() const
    {
        return {buttonSize.width() * 2 + space, buttonSize.height() * 2};
    }

    /**
     * @brief ClassComponentsEditDelegate::createEditor
     * @param parent
     * @param option
     * @param index
     * @return
     */
    QWidget *ClassComponentsEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                       const QModelIndex &index) const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);

        return new Buttons(parent);
    }

} // namespace gui
