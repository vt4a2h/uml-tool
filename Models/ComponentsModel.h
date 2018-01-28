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
#pragma once

#include <functional>

#include <QAbstractTableModel>

#include <Entity/EntityTypes.hpp>
#include <GUI/GuiTypes.hpp>
#include <Translation/translator_types.hpp>

namespace Models {

    /// The DisplayPart enum
    enum class DisplayPart : int {
        Methods,    ///< Methods
        Fields,     ///< Fields
        Elements,   ///< Elements
        Properties, ///< Properties
        Invalid,    ///< Invalid
    };

    /// The ClassComponentsModel class
    class ComponentsModel : public QAbstractTableModel
    {
        Q_OBJECT

    public: // types
        /// The ComponentColName enum
        enum ComponentColName {
            ShortSignature, ///< ShortSignature
            Buttons,        ///< Buttons
        };

        /// The DataRoles enum
        enum DataRoles {
            InternalData = 0x0101, ///< InternalData
        };

        /// The DataChangedRoles enum
        enum DataChangedRoles {
            UpdateSignature = 0x0102, ///< UpdateSignature
        };

    public:
        ComponentsModel(const Entity::SharedComponents &components, QObject *parent = nullptr);
        void setSignatureMaker(Translation::UniqueSignatureMaker &&maker);
        void clear();

        // TODO: try to use templates {
        Entity::SharedMethod addMethod();
        void addExistsMethod(const Entity::SharedMethod &method, int pos = -1);
        int removeMethod(const Entity::SharedMethod &method);
        int removeMethod(const QModelIndex &index);

        Entity::SharedField addField();
        void addExistsField(const Entity::SharedField &field, int pos = -1);
        int removeField(const Entity::SharedField &field);
        int removeField(const QModelIndex &index);

        Entity::SharedEnumarator addElement();
        void addExistsElement(const Entity::SharedEnumarator &element, int pos = -1);
        int removeElement(const Entity::SharedEnumarator &element);
        int removeElement(const QModelIndex &index);

        void addProperty();
        // }

        ~ComponentsModel();

    public: // QAbstractItemModel implementation
        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        Entity::SharedComponents components() const;
        void setComponents(const Entity::SharedComponents &components);

        DisplayPart display() const;
        void setDisplay(const DisplayPart &display);

    signals:
        void showButtons(const QModelIndexList &index);

    private:
        template<class Component>
        decltype(auto) add(const std::function<Component()> &componentMaker, int count)
        {
            beginInsertRows(QModelIndex(), count, count);
            auto component = componentMaker();
            endInsertRows();

            showButtons(QModelIndexList() << index(count, 1));

            return component;
        }

        void addExists(const std::function<void(int)> &inserter, int count, int pos);
        int remove(const std::function<int(int)> &deleter, const QModelIndex &index);
        void updateAllComponents();

    private:
        Entity::SharedComponents m_Components;
        Translation::UniqueSignatureMaker m_SignatureMaker;
        DisplayPart m_display = DisplayPart::Invalid;
    };

} // namespace models

Q_DECLARE_METATYPE(Models::DisplayPart)
