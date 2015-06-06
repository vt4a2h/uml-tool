/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/05/2015.
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
#include "classcomponentsmodel.h"

#include <type_traits>

#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/enum.h>
#include <entity/field.h>

#include <gui/signaturemaker.h>

namespace models {

    namespace  {
        const int colCount = 2;

        int count(const entity::SharedComponents &components, DisplayPart part)
        {
            switch (part) {
                case DisplayPart::Fields:
                    return components->fields().count();

                case DisplayPart::Methods:
                    return components->methods().count();

                case DisplayPart::Elements:
                    return components->variables().count();

                case DisplayPart::Properties:
                    return 0;

                case DisplayPart::Invalid:
                    return 0;
            }

            return 0;
        }
    }

    /**
     * @brief ClassComponentsModel::ClassComponentsModel
     * @param parent
     */
    ClassComponentsModel::ClassComponentsModel(const entity::SharedComponents &components, QObject *parent)
        : QAbstractTableModel(parent)
        , m_Components(components)
    {
    }

    /**
     * @brief ClassComponentsModel::setSignatureMaker
     * @param maker
     */
    void ClassComponentsModel::setSignatureMaker(gui::UniqueSignatureMaker &&maker)
    {
        m_SignatureMaker = std::move(maker);
    }

    /**
     * @brief ClassComponentsModel::clear
     */
    void ClassComponentsModel::clear()
    {
        beginResetModel();
        m_Components.reset();
        endResetModel();
    }

    /**
     * @brief ClassComponentsModel::~ClassComponentsModel
     */
    ClassComponentsModel::~ClassComponentsModel()
    {

    }

    /**
     * @brief ClassComponentsModel::rowCount
     * @param parent
     * @return
     */
    int ClassComponentsModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return m_Components ? count(m_Components, m_display) : 0;
    }

    /**
     * @brief ClassComponentsModel::columnCount
     * @param parent
     * @return
     */
    int ClassComponentsModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return colCount;
    }

    /**
     * @brief ClassComponentsModel::data
     * @param index
     * @param role
     * @return
     */
    QVariant ClassComponentsModel::data(const QModelIndex &index, int role) const
    {
        Q_ASSERT(m_SignatureMaker);

        if (role == Qt::DisplayRole && m_Components) {
            if (index.column() == 0) {
                switch (m_display) {
                    case DisplayPart::Methods:
                        return m_SignatureMaker->signature(m_Components->methods()[index.row()]);

                    case DisplayPart::Fields:
                        return m_SignatureMaker->signature(m_Components->fields()[index.row()]);

                    case DisplayPart::Elements:
                        return m_Components->variables()[index.row()].first; // TODO: add variables to signature maker

                    case DisplayPart::Properties:
                        return "stub";

                    case DisplayPart::Invalid:
                        return tr("Invalid display value");
                }
            }
        }

        return QVariant();
    }

    /**
     * @brief ClassComponentsModel::flags
     * @param index
     * @return
     */
    Qt::ItemFlags ClassComponentsModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = Qt::ItemIsEnabled;

        if (index.isValid() && index.column() == ShortSignature)
            flags |= Qt::ItemIsSelectable;

        return  flags;
    }

    /**
     * @brief ClassComponentsModel::components
     * @return
     */
    entity::SharedComponents ClassComponentsModel::components() const
    {
        return m_Components;
    }

    /**
     * @brief ClassComponentsModel::setComponents
     * @param components
     */
    void ClassComponentsModel::setComponents(const entity::SharedComponents &components)
    {
        beginResetModel();
        m_Components = components;
        endResetModel();

        for (int i = 0; i < count(m_Components, m_display); ++i)
            emit showButtonsForIndex(index(i, 1));
    }

    /**
     * @brief ClassComponentsModel::display
     * @return
     */
    DisplayPart ClassComponentsModel::display() const
    {
        return m_display;
    }

    /**
     * @brief ClassComponentsModel::setDisplay
     * @param display
     */
    void ClassComponentsModel::setDisplay(const DisplayPart &display)
    {
        if (display != m_display) {
            beginResetModel();
            m_display = display;
            endResetModel();

            for (int i = 0; i < count(m_Components, m_display); ++i)
                emit showButtonsForIndex(index(i, 1));
        }
    }

} // namespace models
