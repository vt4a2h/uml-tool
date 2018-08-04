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
        , m_Folded(false)
        , m_SectionName(sectionName)
    {
        m_ui->setupUi(this);

        setVisible(false);

        setFolded(false);
        setSectionHelp(help);
    }

    /**
     * @brief Section::mouseReleaseEvent
     * @param event
     */
    void Section::mouseReleaseEvent(QMouseEvent *event)
    {
        setFolded(!folded());
        QWidget::mouseReleaseEvent(event);
    }

    /**
     * @brief Section::setFolded
     * @param folded
     */
    void Section::setFolded(bool folded)
    {
        m_Folded = folded;
        m_ui->pteEditor->setVisible(!m_Folded);
        m_ui->lblSectionName->setText((this->folded() ? "►" : "▼") + m_SectionName);
    }

    /**
     * @brief Section::folded
     * @return
     */
    bool Section::folded() const
    {
        return m_Folded;
    }

    /**
     * @brief Section::modified
     * @return
     */
    bool Section::modified() const
    {
        return m_Modified;
    }

    /**
     * @brief Section::setModified
     * @param Modified
     */
    void Section::setModified(bool modified)
    {
        m_Modified = modified;
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
        m_ui->pteEditor->setToolTip("<p>" + m_SectionHelp + "</p>");
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


