/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07/06/2015.
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
#include "editmethoddialog.h"
#include "ui_editmethoddialog.h"

namespace GUI {

    /**
     * @brief EditMethodDialog::EditMethodDialog
     * @param parent
     */
    EditMethodDialog::EditMethodDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::EditMethodDialog)
    {
        ui->setupUi(this);
    }

    /**
     * @brief EditMethodDialog::~EditMethodDialog
     */
    EditMethodDialog::~EditMethodDialog()
    {
    }

    /**
     * @brief EditMethodDialog::currentMethod
     * @return
     */
    Entity::SharedMethod EditMethodDialog::currentMethod() const
    {
        return m_CurrentMethod;
    }

    /**
     * @brief EditMethodDialog::setCurrentMethod
     * @param currentMethod
     */
    void EditMethodDialog::setCurrentMethod(const Entity::SharedMethod &currentMethod)
    {
        m_CurrentMethod = currentMethod;
    }

    /**
     * @brief EditMethodDialog::closeEvent
     * @param ev
     */
    void EditMethodDialog::closeEvent(QCloseEvent *ev)
    {
        clear();
        QDialog::closeEvent(ev);
    }

    /**
     * @brief EditMethodDialog::clear
     */
    void EditMethodDialog::clear()
    {
        // clear exists data
    }

} // namespace gui
