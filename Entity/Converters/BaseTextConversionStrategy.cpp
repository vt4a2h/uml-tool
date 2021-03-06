/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 14/10/2018.
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
#include "BaseTextConversionStrategy.hpp"

#include <functional>

#include <Entity/Type.h>

#include <DB/ITypeSearcher.h>

#include <Models/IMessenger.h>

namespace Entity::Converters {

    template <class F, class ...Args>
    auto invokeSafe(const Models::SharedMessenger &messenger, F f, Args&&... args)
    {
        try {
            return std::invoke(f, std::forward<Args>(args)...);
        } catch (const Models::MessageException &e) {
            if (messenger)
                messenger->addMessage(e);
        } catch (...) {
            if (messenger)
                messenger->addMessage(Models::MessageType::Error,
                                      BaseTextConversionStrategy::tr("Unexpected error"));
        }

        return decltype(std::invoke(f, std::forward<Args>(args)...)){};
    }

    QString BaseTextConversionStrategy::toString(const Type &element) const noexcept
    {
        return invokeSafe(messenger(), &BaseTextConversionStrategy::toStringImpl, this, element);
    }

    bool BaseTextConversionStrategy::fromString(const QString &s, Type &element) const noexcept
    {
        return invokeSafe(messenger(), &BaseTextConversionStrategy::fromStringImpl, this, s, element);
    }

    void BaseTextConversionStrategy::registerTypeSearcher(const DB::WeakTypeSearcher &typeSearcher)
    {
        m_TypeSearchers << typeSearcher;
    }

    void BaseTextConversionStrategy::unregisterTypeSearcher(const DB::WeakTypeSearcher &typeSearcher)
    {
        m_TypeSearchers.remove(typeSearcher);
    }

    template <class Key, class Method>
    static SharedType typeSearchImpl(const DB::WeakTypeSearchersSet typeSearchers, const Key &key,
                                     Method m)
    {
        try {
            for (auto &&weakTs: typeSearchers)
                if (auto ts = weakTs.lock())
                    if (auto t = (*ts.*m)(key))
                        return t;
        } catch (...) {}

        return nullptr;
    }

    SharedType BaseTextConversionStrategy::typeByID(const Common::ID &id) const noexcept
    {
        return typeSearchImpl(m_TypeSearchers, id, &DB::ITypeSearcher::typeByID);
    }

    SharedType BaseTextConversionStrategy::typeByName(const QString &name) const noexcept
    {
        return typeSearchImpl(m_TypeSearchers, name, &DB::ITypeSearcher::typeByName);
    }

    Common::ID BaseTextConversionStrategy::typeIdByName(const QString &name) const
    {
        if (name.isEmpty())
            return Common::ID::nullID();

        if (auto type = typeByName(name))
            return type->id();

        throw Models::MessageException(Models::MessageType::Error, tr("Bad type name: ") + name);
    }

    Models::SharedMessenger BaseTextConversionStrategy::messenger() const noexcept
    {
        return m_Messenger;
    }

    QString BaseTextConversionStrategy::toStringImpl(const Type &/*element*/) const
    {
        return QString::null;
    }

    bool BaseTextConversionStrategy::fromStringImpl(const QString &/*s*/, Type &/*element*/) const
    {
        return false;
    }

    void BaseTextConversionStrategy::registerMessenger(const Models::SharedMessenger &messenger)
    {
        m_Messenger = messenger;
    }

} // namespace Entity::Converters
