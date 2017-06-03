/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03.
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
#include "HtmlDelegate.h"

#include <QPainter>
#include <QTextDocument>

namespace GUI {

    static const int padding = 6;

    /**
     * @brief GUI::HtmlDelegate::paint
     * @param painter
     * @param option
     * @param index
     */
    void GUI::HtmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
    {
        QStyleOptionViewItem options = option;
        initStyleOption(&options, index);

        painter->save();

        QTextDocument doc;
        doc.setHtml(options.text);

        options.text.clear();
        options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

        QSize iconSize{options.icon.actualSize(options.rect.size())};
        painter->translate(options.rect.left() + iconSize.width() + padding, options.rect.top());
        QRect clip(0, 0, options.rect.width() + iconSize.width() + padding, options.rect.height());
        doc.drawContents(painter, clip);

        painter->restore();
    }

    /**
     * @brief GUI::HtmlDelegate::sizeHint
     * @param option
     * @param index
     * @return
     */
    QSize GUI::HtmlDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
    {
        QStyleOptionViewItem options = option;
        initStyleOption(&options, index);

        QTextDocument doc;
        doc.setHtml(options.text);
        doc.setTextWidth(options.rect.width());

        return QSize(doc.idealWidth(), doc.size().height());
    }

} // namespace GUI
