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
#include <entity/extendedtype.h>

#include <models/applicationmodel.h>

#include <translator/projecttranslator.h>
#include <translator/code.h>

#include <utility/helpfunctions.h>

#include "constants.h"

namespace gui {

    namespace {
        const QString noSignature = SignatureMaker::tr("Not available.");
        const QSet<QString> globalIds = {GLOBAL_SCOPE_ID, PROJECT_GLOBAL_SCOPE_ID, GLOBAL, STUB_ID};
    }

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
            return makeField(std::static_pointer_cast<entity::Field>(component));
        else if (hash == entity::ClassMethod::staticHashType())
            return m_Translator->translate(std::static_pointer_cast<entity::ClassMethod>(component)).toHeader;
        // TODO: handle variable case
        // TODO: don't use translator. We need a separate methods.

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

    /**
     * @brief SignatureMaker::makeType
     * @param type
     * @return
     */
    QString SignatureMaker::makeType(const entity::SharedType &type) const
    {
        if (!type)
            return "";

        QString result;

        QStringList scopes;
        auto scopeId = type->scopeId();
        while (!globalIds.contains(scopeId)) {
            if (auto scope = findScope(scopeId)) {
                if (!scope->name().isEmpty() && scope->id() != m_Project->database()->defaultScopeID())
                    scopes.prepend(scope->name());
                scopeId = scope->parentScopeId();
            } else {
                return "";
            }
        }

        scopes.removeAll("");
        if (!scopes.isEmpty())
            result.prepend(scopes.join("::").append("::"));

        result.append(type->name());

        return result;
    }

    /**
     * @brief SignatureMaker::makeSharedType
     * @param type
     * @return
     */
    QString SignatureMaker::makeExtType(const entity::SharedExtendedType &type) const
    {
        if (!type)
            return "";

        // Has alias... Just return it.
        if (type->name() != DEFAULT_NAME && type->name() != BASE_TYPE_NAME)
            return type->name();

        if (auto baseType = findType(type->typeId())) {
            QString result = makeTypeOrExtType(baseType);
            if (result.isEmpty())
                return result;

            if (type->isConst())
                result.prepend(QChar::Space).prepend("const");

            const auto& paramIds = type->templateParameters();
            if (!paramIds.isEmpty()) {
                QStringList parameters;
                parameters.reserve(paramIds.size());
                for (auto &&id : paramIds) {
                    const auto& type = findType(id);
                    if (type)
                        parameters << makeTypeOrExtType(type);
                    else
                        return "";

                    parameters.removeAll("");
                    if (!parameters.isEmpty())
                        result.append("<").append(parameters.join(", ")).append(">");
                }
            }

            const auto& pl = type->pl();
            if (!pl.isEmpty()) {
                QStringList tmpPl;
                for (auto &&e :  pl) {
                    // Append * or &
                    tmpPl << e.first;
                    if (e.second)
                        tmpPl << "const";
                }

                result.append(QChar::Space).append(tmpPl.join(QChar::Space));
            }

            return result;
        } else {
            return "";
        }
    }

    /**
     * @brief SignatureMaker::makeTypeOrExt
     * @param type
     * @return
     */
    QString SignatureMaker::makeTypeOrExtType(const entity::SharedType &type) const
    {
        if (!type)
            return "";

        if (type->hashType() == entity::ExtendedType::staticHashType())
            // Return extended type which may be alias (with optioanl *, &, const)
            return makeExtType(std::static_pointer_cast<entity::ExtendedType>(type));
        else
            // Or return name (with namespaces) of type, class, enum or union
            return makeType(type);
    }

    /**
     * @brief SignatureMaker::makeField
     * @param field
     * @return
     */
    QString SignatureMaker::makeField(const entity::SharedField &field) const
    {
        if (!field)
            return tr("No field");

        QString result = makeTypeOrExtType(findType(field->typeId()));
        if (result.isEmpty())
            return tr("Type is not found");

        result.append(QChar::Space).append(field->fullName());

        const auto& keywords = field->keywords();
        if (!keywords.isEmpty()) {
            QStringList kw;
            kw.reserve(keywords.size());
            for (auto &&keyword : keywords)
                kw << utility::fieldKeywordToString(keyword);

            result.prepend(QChar::Space).prepend(kw.join(QChar::Space));
        }

        return result;
    }

    /**
     * @brief SignatureMaker::findScope
     * @param scope
     * @return
     */
    entity::SharedScope SignatureMaker::findScope(const QString &scopeId) const
    {
        return utility::findScope(scopeId, m_Project->database(), m_ApplicationModel->globalDatabase());
    }

    /**
     * @brief SignatureMaker::type
     * @param typeId
     * @return
     */
    entity::SharedType SignatureMaker::findType(const QString &typeId) const
    {
        return utility::findType(typeId, m_Project->database(), m_ApplicationModel->globalDatabase());
    }

} // namespace gui

