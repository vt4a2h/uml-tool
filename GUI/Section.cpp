/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18.
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
#include "Section.h"
#include "ui_Section.h"

namespace GUI {

    /**
     * @brief Section::Section
     * @param sectionName
     * @param help
     * @param parent
     */
    Section::Section(const QString &sectionName, const QString &help, QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::Section)
        , m_State(Opened)
        , m_SectionName(sectionName)
    {
        m_ui->setupUi(this);

        setVisible(false);

        setState(Opened);
        setSectionHelp(help);
    }

    /**
     * @brief Section::mouseReleaseEvent
     * @param event
     */
    void Section::mouseReleaseEvent(QMouseEvent *event)
    {
        setState(m_State == Opened ? Closed : Opened);
        QWidget::mouseReleaseEvent(event);
    }

    /**
     * @brief Section::setState
     * @param state
     */
    void Section::setState(State state)
    {
        m_State = state;
        m_ui->tbText->setVisible(m_State == Opened ? true : false);
        m_ui->lblSectionName->setText((m_State == Opened ? "▼" : "►") + m_SectionName);
    }

    /**
     * @brief Section::sectionHelp
     * @return
     */
    QString Section::sectionHelp() const
    {
        return m_SectionHelp;
    }

    /**
     * @brief Section::setSectionHelp
     * @param sectionHelp
     */
    void Section::setSectionHelp(const QString &sectionHelp)
    {
        m_SectionHelp = sectionHelp;
        m_ui->tbText->setToolTip("<p>" + m_SectionHelp + "</p>");
    }

    /**
     * @brief Section::text
     * @return
     */
    QString Section::text() const
    {
        return m_ui->tbText->toPlainText();
    }

    /**
     * @brief Section::setText
     * @param text
     */
    void Section::setText(const QString &text)
    {
        m_ui->tbText->setPlainText(text);
    }

    /**
     * @brief Section::sectionName
     * @return
     */
    QString Section::sectionName() const
    {
        return m_SectionName;
    }

    /**
     * @brief Section::setSectionName
     * @param sectionName
     */
    void Section::setSectionName(const QString &sectionName)
    {
        m_SectionName = sectionName;
    }

    /**
     * @brief Section::~Section
     */
    Section::~Section()
    {
    }

} // namespace GUI


