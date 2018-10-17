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
#pragma once

#include <QObject>
#include <QSet>

#include <Entity/Converters/ITextConversionStrategy.hpp>

namespace Entity::Converters {

    class BaseTextConversionStrategy
        : public QObject
        , public ITextConversionStrategy
    {
        Q_OBJECT

    public: // ITextConversionStrategy interface
        QString toString(const Type &element) const noexcept final;
        bool fromString(const QString &s, Type &element) const noexcept final;

        void registerMessenger(const Models::SharedMessenger &messenger) override;

    public slots:
        void registerTypeSearcher(const DB::WeakTypeSearcher &typeSearcher) override;
        void unregisterTypeSearcher(const DB::WeakTypeSearcher &typeSearcher) override;

    protected:
        Entity::SharedType typeByID(const Common::ID &id) const noexcept;
        Entity::SharedType typeByName(const QString &name) const noexcept;

        Models::SharedMessenger messenger() const noexcept;

    protected: // ITextConversionStrategy interface
        QString toStringImpl(const Entity::Type &element) const override;
        bool fromStringImpl(const QString &s, Entity::Type &element) const override;

    private:
        DB::WeakTypeSearchersSet m_TypeSearchers;
        Models::SharedMessenger m_Messenger;
    };

} // namespace Entity::Converters
