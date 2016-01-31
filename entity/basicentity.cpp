/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09/02/2015.
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
#include "basicentity.h"

#include <utility/helpfunctions.h>

#include <helpers/generatorid.h>

#include "enums.h"

namespace entity {

    namespace {
        const QString nameMark = "Name";
        const QString idMark = "ID";
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param name
     */
    BasicEntity::BasicEntity(const QString &name)
        : m_Name(name)
        , m_Id(GeneratorID::instance().genID())
    {
    }

    BasicEntity::BasicEntity(const QString &name, const EntityID &id)
        : m_Name(name)
        , m_Id(id)
    {
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param src
     */
    BasicEntity::BasicEntity(const BasicEntity &src)
        : QObject()
        , m_Name(src.m_Name)
        , m_Id(src.m_Id)
    {
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param src
     */
    BasicEntity::BasicEntity(BasicEntity &&src)
    {
        m_Name = std::move(src.m_Name);
        m_Id = std::move(src.m_Id);
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param src
     * @param errorList
     */
    BasicEntity::BasicEntity(const QJsonObject &src, QStringList &errorList)
    {
        fromJson(src, errorList);
    }

    /**
     * @brief BasicEntity::operator =
     * @param rhs
     * @return
     */
    BasicEntity &BasicEntity::operator =(const BasicEntity &rhs)
    {
        if (this != &rhs) {
            setName(rhs.name());
            m_Id = rhs.m_Id;
        }

        return *this;
    }

    /**
     * @brief BasicEntity::BasicEntity
     */
    BasicEntity::BasicEntity()
        : m_Name("")
        , m_Id(GeneratorID::instance().genID())
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const BasicEntity &lhs, const BasicEntity &rhs)
    {
        return lhs.m_Name == rhs.m_Name && lhs.m_Id == rhs.m_Id;
    }

    /**
     * @brief BasicEntity::operator =
     * @param rhs
     * @return
     */
    BasicEntity &BasicEntity::operator =(BasicEntity &&rhs)
    {
        if (this != &rhs) {
            m_Name = std::move(rhs.m_Name);
            m_Id = std::move(rhs.m_Id);
        }

        return *this;
    }

    /**
     * @brief BasicEntity::id
     * @return
     */
    EntityID BasicEntity::id() const
    {
        return m_Id;
    }

    /**
     * @brief BasicEntity::setId
     * @param id
     */
    void BasicEntity::setId(const EntityID &id)
    {
        m_Id = id;
    }

    /**
     * @brief BasicEntity::section
     * @return
     */
    Section BasicEntity::section() const
    {
        return Section::Public;
    }

    /**
     * @brief BasicEntity::name
     * @return
     */
    QString BasicEntity::name() const
    {
        return m_Name;
    }

    /**
     * @brief BasicEntity::setName
     * @param name
     */
    void BasicEntity::setName(const QString &name)
    {
        if (m_Name != name) {
            m_Name = name;
            emit nameChanged(m_Name);
        }
    }

    /**
     * @brief BasicEntity::writeToFile
     * @param fileName
     */
    void BasicEntity::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief BasicEntity::readFromFile
     * @param fileName
     * @return
     */
    bool BasicEntity::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief BasicEntity::toJson
     * @return
     */
    QJsonObject BasicEntity::toJson() const
    {
        QJsonObject result;

        result.insert(nameMark, m_Name);

        // Workaround for correct saving
        result.insert(idMark, QString::number(m_Id));

        return result;
    }

    /**
     * @brief BasicEntity::fromJson
     * @param src
     * @param errorList
     */
    void BasicEntity::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        using namespace utility;

        checkAndSet(src, nameMark, errorList, [&](){ m_Name = src[nameMark].toString(); });
        checkAndSet(src, idMark, errorList, [&](){ m_Id = src[idMark].toString().toULongLong(); });
    }

    /**
     * @brief BasicEntity::hashType
     * @return
     */
    size_t BasicEntity::hashType() const
    {
        return BasicEntity::staticHashType();
    }

    /**
     * @brief BasicEntity::staticHashType
     * @return
     */
    size_t BasicEntity::staticHashType()
    {
        return typeid(BasicEntity).hash_code();
    }

    /**
     * @brief BasicEntity::marker
     * @return
     */
    QString BasicEntity::marker() const
    {
        return BasicEntity::staticMarker();
    }

    /**
     * @brief BasicEntity::staticMarker
     * @return
     */
    QString BasicEntity::staticMarker()
    {
        return "BasicEntity";
    }

    /**
     * @brief BasicEntity::defaultName
     * @return
     */
    QString BasicEntity::defaultName() const
    {
       return staticDefaultName();
    }

    /**
     * @brief BasicEntity::staticDefaultName
     * @return
     */
    QString BasicEntity::staticDefaultName()
    {
        return tr("Basic entity");
    }

}
