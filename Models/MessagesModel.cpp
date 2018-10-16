/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28.
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
#include "MessagesModel.h"

#include <QGradient>
#include <QDebug>

namespace Models {

    static const int iconSize = 24;

    static QPixmap scaledIcon(const QString &path)
    {
        return QPixmap(path)
                   .scaled(iconSize, iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    static const QHash<MessageType, QBrush> brushes = {
        {MessageType::Information, QBrush(QColor(120, 255, 111)) },
        {MessageType::Warning    , QBrush(QColor(248, 255, 110)) },
        {MessageType::Error      , QBrush(QColor(255, 130, 130)) },
    };

    static QString toHtml(const QString &in)
    {
        return in.toHtmlEscaped().replace("\n", "<br>");
    }

    static const Message &messageForIndex(const Messages &messages, int index)
    {
        return *(messages.crbegin() + index);
    }

    static const QString messageWithDescriptionTemplate    = "<b>%1</b><br><i>%2</i>";
    static const QString messageWithoutDescriptionTemplate = "<b>%1</b>";

    static const QString dateWithDescriptionTemplate    = "%1\n%2";
    static const QString dateWithoutDescriptionTemplate = "%1 (%2)";

    MessagesModel::MessagesModel(QObject * parent)
        : QAbstractTableModel(parent)
        , m_UnreadMessagesCount(0)
        , m_CachedIcons {
              {MessageType::Information, scaledIcon(":/icons/pic/icon_information.png")},
              {MessageType::Warning    , scaledIcon(":/icons/pic/icon_warning.png")    },
              {MessageType::Error      , scaledIcon(":/icons/pic/icon_error.png")      },
          }
    {
    }

    /**
     * @brief MessagesModel::addMessage
     * @param type
     * @param text
     */
    void MessagesModel::addMessage(MessageType type, const QString &summary,
                                   const QString &description)
    {
        if (summary.isEmpty() && description.isEmpty())
            return;

        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
        m_Messages.append({type, toHtml(summary), toHtml(description), QDateTime::currentDateTime()});
        endInsertRows();

        if ((m_IsViewing && !m_IsViewing()) || !m_IsViewing)
            ++m_UnreadMessagesCount;
        emit newMessageAdded();
    }

    /**
     * @brief MessagesModel::messages
     * @return
     */
    Messages MessagesModel::messages() const
    {
        return m_Messages;
    }

    /**
     * @brief MessagesModel::rowCount
     * @return
     */
    int MessagesModel::rowCount(const QModelIndex &/*parent*/) const
    {
        return m_Messages.count();
    }

    /**
     * @brief MessagesModel::columnCount
     * @return
     */
    int MessagesModel::columnCount(const QModelIndex &/*parent*/) const
    {
        return static_cast<int>(ColumnType::EnumSize);
    }

    /**
     * @brief MessagesModel::data
     * @param index
     * @param role
     * @return
     */
    QVariant MessagesModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        switch (role) {
            case Qt::DecorationRole:
                return processDecorationRole(index);

            case Qt::DisplayRole:
                return processDisplayRole(index);

            case Qt::TextAlignmentRole:
                return processTextAligmentRole(index);

            case Qt::BackgroundRole:
                return processBackgroundRole(index);

            default: ;
        }

        return QVariant();
    }

    /**
     * @brief MessagesModel::unreadMessagesCount
     * @return
     */
    uint MessagesModel::unreadMessagesCount() const
    {
        return m_UnreadMessagesCount;
    }

    /**
     * @brief MessagesModel::clean
     */
    void MessagesModel::clear()
    {
        beginResetModel();
        m_Messages.clear();
        endResetModel();
    }

    /**
     * @brief MessagesModel::addMessage
     * @param e
     */
    void MessagesModel::addMessage(const MessageException &e)
    {
       addMessage(e.type, e.summary, e.description);
    }

    /**
     * @brief MessagesModel::markAllMessagesRead
     */
    void MessagesModel::markAllMessagesRead()
    {
        m_UnreadMessagesCount = 0;
    }

    /**
     * @brief MessagesModel::setViewStatusFunction
     * @param f
     */
    void MessagesModel::setViewStatusFunction(ViewStatusFunc f)
    {
        m_IsViewing = std::move(f);
    }

    /**
     * @brief MessagesModel::processDisplayRole
     * @param index
     * @return
     */
    QVariant MessagesModel::processDisplayRole(const QModelIndex &index) const
    {
        switch (index.column()) {
            case static_cast<int>(ColumnType::Text):
            {
                const auto &message = messageForIndex(m_Messages, index.row());
                return (message.description.isEmpty()
                           ? messageWithoutDescriptionTemplate
                           : messageWithDescriptionTemplate).arg(message.summary, message.description);
            }

            case static_cast<int>(ColumnType::Date):
            {
                 const auto &message = messageForIndex(m_Messages, index.row());
                 const auto &date = message.date;
                 auto timeStr = date.toString("hh:mm:s");
                 auto dateStr = date.toString("dd/MM/yyyy");

                 return (message.description.isEmpty()
                            ? dateWithoutDescriptionTemplate
                            : dateWithDescriptionTemplate).arg(timeStr, dateStr);
            }

            default:
                return QVariant();
        }
    }

    /**
     * @brief MessagesModel::processDecorationRole
     * @param index
     * @return
     */
    QVariant MessagesModel::processDecorationRole(const QModelIndex &index) const
    {
        Q_ASSERT(m_Messages.count() - 1 >= index.row());
        return index.column() == static_cast<int>(ColumnType::Text) ?
                    m_CachedIcons[messageForIndex(m_Messages, index.row()).type] : QVariant();
    }

    /**
     * @brief MessagesModel::processTextAligmentRole
     * @param index
     * @return
     */
    QVariant MessagesModel::processTextAligmentRole(const QModelIndex &index) const
    {
        return index.column() == static_cast<int>(ColumnType::Text) ? Qt::AlignVertical_Mask
                                                                    : Qt::AlignVCenter;
    }

    /**
     * @brief MessagesModel::processBackgroundRole
     * @param index
     * @return
     */
    QVariant MessagesModel::processBackgroundRole(const QModelIndex &index) const
    {
        return brushes[messageForIndex(m_Messages, index.row()).type];
    }

    /**
     * @brief MessagesModel::resetInternalData
     */
    void MessagesModel::resetInternalData()
    {
       clear();
    }

} // namespace Models
