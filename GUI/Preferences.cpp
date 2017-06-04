/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04.
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
#include "Preferences.h"

#include "ui_Preferences.h"

namespace GUI {

    /**
     * @brief Preferences::Preferences
     * @param parent
     */
    Preferences::Preferences(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Preferences)
    {
        ui->setupUi(this);
    }

    /**
     * @brief Preferences::~Preferences
     */
    Preferences::~Preferences()
    {
    }

} // namespace GUI
