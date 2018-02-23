/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20 .
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
#include "SectionalTextConverter.hpp"

#include <functional>
#include <optional>

#include <Common/BasicElement.h>

#include <Entity/Enum.h>

#include "IMessenger.h"
#include "QtHelpers.h"

namespace Models
{

    /**
     * @brief SectionalTextConverter::SectionalTextConverter
     * @param parent
     */
    SectionalTextConverter::SectionalTextConverter(SharedMessenger const & messenger)
        : m_Messenger(G_ASSERT(messenger))
    {
    }

    template <class T>
    T const & to(Common::BasicElement const & elem)
    {
        Q_ASSERT(T::staticHashType() == elem.hashType());
        return static_cast<T const &>(elem);
    }


    static QString enumToString(Common::BasicElement const & elem, DB::WeakTypeSearchers const & ts)
    {
        auto e = to<Entity::Enum>(elem);
    }

    using ToStringConverter =
        std::function<QString(Common::BasicElement const&, DB::WeakTypeSearchers const &)>;
    static const QHash<size_t, ToStringConverter> toStrConvById =
    {
        { Entity::Enum::staticHashType(), &enumToString },
    };

    /**
     * @brief SectionalTextConverter::toString
     * @param element
     * @return
     */
    QString SectionalTextConverter::toString(Common::BasicElement const & element) const noexcept
    {
        if (auto it = toStrConvById.find(element.hashType()); it != toStrConvById.end())
            return (*it)(element, m_TypeSearchers);
        else
            m_Messenger->addMessage(MessageType::Error, tr("Cannot get string representation"),
                                    tr("There is no an appropriate converter"));

        return QString::null;
    }

    /**
     * @brief SectionalTextConverter::fromString
     * @param s
     * @param element
     * @return
     */
    void SectionalTextConverter::fromString(QString const & s,
                                            Common::BasicElement & element) const noexcept
    {

    }

    /**
     * @brief SectionalTextConverter::registerTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::registerTypeSearcher(DB::SharedTypeSearcher const & typeSearcher)
    {

    }

    /**
     * @brief SectionalTextConverter::unregisterTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::unregisterTypeSearcher(DB::SharedTypeSearcher const & typeSearcher)
    {

    }

} // namespace Models
