/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 26/02/2015.
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
#include "addscope.h"
#include "ui_addscope.h"

namespace {
    const QString defaultWidgetName = GUI::AddScope::tr("Create new scope");
}

namespace GUI {

    /**
     * @brief AddScope::AddScope
     * @param parent
     */
    AddScope::AddScope(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::AddScope)
    {
        ui->setupUi(this);
        ui->leScopeName->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+"), this));

        connect(ui->leScopeName, &QLineEdit::textChanged, this, &AddScope::onTextChanged);
    }

    /**
     * @brief AddScope::~AddScope
     */
    AddScope::~AddScope()
    {
    }

    /**
     * @brief AddScope::setProjectName
     * @param name
     */
    void AddScope::setProjectName(const QString &name)
    {
        setWindowTitle( !name.isEmpty() ? defaultWidgetName + tr(" in project %1").arg(name) : name);
    }

    /**
     * @brief AddScope::addExistingScopesNames
     * @param scopes
     */
    void AddScope::addExistingScopesNames(const QStringList &scopes)
    {
        m_ExistingScopes = scopes;
    }

    /**
     * @brief AddScope::scopeName
     * @return
     */
    QString AddScope::scopeName()
    {
        return m_ScopeName;
    }

    /**
     * @brief AddScope::onCreateScope
     */
    void AddScope::onCreateScope()
    {
        m_ScopeName = ui->leScopeName->text();
    }

    /**
     * @brief AddScope::showEvent
     * @param ev
     */
    void AddScope::showEvent(QShowEvent *ev)
    {
        ui->leScopeName->clear();
        ui->leScopeName->setFocus();
        ui->pbCreateScope->setEnabled(false);

        QDialog::showEvent(ev);
    }

    /**
     * @brief AddScope::onTextChanged
     * @param text
     */
    void AddScope::onTextChanged(const QString &text)
    {
        bool valid = !text.isEmpty() && m_ExistingScopes.indexOf(text) == -1;
        ui->pbCreateScope->setEnabled(valid);
        ui->leScopeName->setStyleSheet(valid ? "" : "color: rgb(255, 0, 0);");
    }

} // namespace gui
