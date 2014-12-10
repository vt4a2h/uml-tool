/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 19/11/2014.
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

#include <QObject>
#include <QJsonObject>

#include "types.h"

/**
 * @brief qml_adaptors
 */
namespace qml_adaptors {

    /**
     * @brief The ProjectAdaptor class
     */
    class ProjectAdaptor : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString id   READ id)
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    public:
        explicit ProjectAdaptor(QObject *parent = 0);
        explicit ProjectAdaptor(const project::SharedProject &project, QObject *parent = 0);
        ProjectAdaptor(const ProjectAdaptor &src);
        ~ProjectAdaptor();

        project::SharedProject project() const;
        void setProject(const project::SharedProject &project);

        QString name() const;
        QString id()   const;
        QString path() const;

        Q_INVOKABLE QJsonObject toJson() const;
        Q_INVOKABLE void fromJson(const QJsonObject &src, QStringList &errorList);

    signals:
        void nameChanged(const QString& newName);
        void pathChanged(const QString& newPath);

    public slots:
        void setName(const QString &name);
        void setPath(const QString &path);

    private:
        project::SharedProject m_Project;
    };

} // namespace qml_adaptors

Q_DECLARE_METATYPE(qml_adaptors::ProjectAdaptor)
