/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 02/01/2016.
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
#include "settings.h"

#include <QVariant>
#include <QDir>
#include <QSettings>
#include <QtGlobal>

#include <boost/range/algorithm/find_if.hpp>

#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>

namespace application {

    namespace {

        template <class Value>
        struct Setting
        {
            QString name;
            Value defaultValue;
        };

        // Constants
        const QString organization = "Q-UML";
        const QString app = "uml-tool";

        const QString mwGroup = "MainWindow";
        const Setting<QRect> mwRect {"main-window-rect", QRect(0, 0, 1024, 768)};

        const QString dbGroup = "DB";
        const Setting<QString> dbName {"global-db-name", "global"};
        const Setting<QString> dbPath {"global-db-path", QDir::currentPath()};

        const QString elGroup = "Elements";
        const QVector<Setting<QColor>> elColors =
        {
            { entity::Class::staticMarker()        , {245, 224, 119} },
            { entity::Enum::staticMarker()         , {83 , 185, 86 } },
            { entity::Union::staticMarker()        , {144, 199, 229} },
            { entity::TemplateClass::staticMarker(), {152, 131, 190} },
            { entity::ExtendedType::staticMarker() , {249, 181, 194} },
        };

        // Helpers
        template <class ValueType>
        inline ValueType read(const QString &prefix, const QString &key,
                              const ValueType &defaultValue)
        {
            QSettings s(organization, app);
            s.beginGroup(prefix);

            ValueType result = defaultValue;
            if (s.contains(key))
                result = s.value(key).value<ValueType>();
            else
                s.setValue(key, defaultValue);

            s.endGroup();
            return result;
        }

        void write(const QString &prefix, const QString &key, const QVariant &value)
        {
            QSettings s(organization, app);
            s.beginGroup(prefix);
            s.setValue(key, value);
            s.endGroup();
        }
    }

    namespace settings {

        /**
         * @brief mainWindowGeometry
         * @return
         */
        QRect mainWindowGeometry()
        {
            return read(mwGroup, mwRect.name, mwRect.defaultValue);
        }

        /**
         * @brief setMainWindowGeometry
         * @param rect
         */
        void writeMainWindowGeometry(const QRect &rect)
        {
            write(mwGroup, mwRect.name, rect);
        }

        /**
         * @brief globalDbPath
         * @return
         */
        QString globalDbPath()
        {
            return read(dbGroup, dbPath.name, dbPath.defaultValue);
        }

        /**
         * @brief setGlobalDbPath
         * @param path
         */
        void setGlobalDbPath(const QString &path)
        {
            write(dbGroup, dbPath.name, path);
        }

        /**
         * @brief globalDbName
         * @return
         */
        QString globalDbName()
        {
            return read(dbGroup, dbName.name, dbName.defaultValue);
        }

        /**
         * @brief setGlobalDbName
         * @param name
         */
        void setGlobalDbName(const QString &name)
        {
            write(dbGroup, dbName.name, name);
        }

        /**
         * @brief elementColor
         * @param hash
         * @return
         */
        QColor elementColor(const QString &marker)
        {
            auto it = boost::range::find_if(elColors, [&](auto&& s) { return s.name == marker; });
            if (it != elColors.end())
                return read(elGroup, it->name, it->defaultValue);

            return Qt::white;
        }

        /**
         * @brief setElementColor
         * @param marker
         * @param color
         */
        void setElementColor(const QString &marker, const QColor &color)
        {
            Q_ASSERT(boost::range::find_if(elColors, [&](auto&& s) { return s.name == marker; })
                     != elColors.end());
            write(elGroup, marker, color);
        }

    } // namespace settings

} // namespace application