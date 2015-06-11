/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/06/2015.
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
#include "signaturemaker.h"

#include <entity/field.h>
#include <entity/classmethod.h>
#include <entity/enum.h>

#include <models/applicationmodel.h>

#include <translator/projecttranslator.h>
#include <translator/code.h>

namespace gui {

    /**
     * @brief SignatureMaker::SignatureMaker
     */
    SignatureMaker::SignatureMaker()
        : SignatureMaker(nullptr, nullptr, nullptr, nullptr)
    {
    }

    /**
     * @brief SignatureMaker::SignatureMaker
     * @param model
     * @param project
     * @param scope
     * @param type
     */
    SignatureMaker::SignatureMaker(const models::SharedApplicationModel &model, const project::SharedProject &project,
                                   const entity::SharedScope &scope, const entity::SharedType &type)
        : m_Type(type)
        , m_Scope(scope)
        , m_Project(project)
        , m_ApplicationModel(model)
    {
        if (m_Project && m_ApplicationModel)
            m_Translator = std::make_unique<translator::ProjectTranslator>(m_ApplicationModel->globalDatabase(),
                                                                           m_Project->database());
    }

    /**
     * @brief SignatureMaker::~SignatureMaker
     */
    SignatureMaker::~SignatureMaker()
    {
    }

    /**
     * @brief SignatureMaker::signature
     * @param entity
     * @return
     */
    QString SignatureMaker::signature(const entity::SharedBasicEntity &component)
    {
        if (!m_Translator)
            m_Translator = std::make_unique<translator::ProjectTranslator>(m_ApplicationModel->globalDatabase(),
                                                                           m_Project->database());

        size_t hash = component->hashType();
        if (hash == entity::Field::staticHashType())
            return m_Translator->translate(std::static_pointer_cast<entity::Field>(component)).toHeader;
        else if (hash == entity::ClassMethod::staticHashType())
            return m_Translator->translate(std::static_pointer_cast<entity::ClassMethod>(component)).toHeader;
        // TODO: handle variable case

        return tr("Wrong component");
    }

    /**
     * @brief SignatureMaker::type
     * @return
     */
    entity::SharedType SignatureMaker::type() const
    {
        return m_Type;
    }

    /**
     * @brief SignatureMaker::setType
     * @param type
     */
    void SignatureMaker::setType(const entity::SharedType &type)
    {
        m_Type = type;
    }

    /**
     * @brief SignatureMaker::Scope
     * @return
     */
    entity::SharedScope SignatureMaker::scope() const
    {
        return m_Scope;
    }

    /**
     * @brief SignatureMaker::setScope
     * @param scope
     */
    void SignatureMaker::setScope(const entity::SharedScope &scope)
    {
        m_Scope = scope;
    }

    /**
     * @brief SignatureMaker::Project
     * @return
     */
    project::SharedProject SignatureMaker::project() const
    {
        return m_Project;
    }

    /**
     * @brief SignatureMaker::setProject
     * @param project
     */
    void SignatureMaker::setProject(const project::SharedProject &project)
    {
        m_Project = project;
    }

    /**
     * @brief SignatureMaker::ApplicationModel
     * @return
     */
    models::SharedApplicationModel SignatureMaker::applicationModel() const
    {
        return m_ApplicationModel;
    }

    /**
     * @brief SignatureMaker::setApplicationModel
     * @param applicationModel
     */
    void SignatureMaker::setApplicationModel(const models::SharedApplicationModel &applicationModel)
    {
        m_ApplicationModel = applicationModel;
    }

} // namespace gui

