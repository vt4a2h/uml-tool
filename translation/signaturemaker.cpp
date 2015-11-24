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
#include <entity/property.h>

#include <models/applicationmodel.h>

#include <utility/helpfunctions.h>

#include "constants.h"
#include "enums.h"

namespace translation {

    namespace {
        const QString noSignature = SignatureMaker::tr("Not available.");
        const QSet<QString> globalIds = {GLOBAL_SCOPE_ID, PROJECT_GLOBAL_SCOPE_ID, GLOBAL, STUB_ID};

        const QString memberMark      = "MEMBER";
        const QString readMark        = "READ";
        const QString writeMark       = "WRITE";
        const QString resetMark       = "RESET";
        const QString notifyMark      = "NOTIFY";
        const QString revisionMark    = "REVISION";
        const QString designableMark  = "DESIGNABLE";
        const QString scriptableMark  = "SCRIPTABLE";
        const QString storedMark      = "STORED";
        const QString userMark        = "USER";
        const QString finalMark       = "FINAL";
        const QString constantMark    = "CONSTANT";

        template<class Check, class CheckDefault, class Get>
        inline void addAdditionalMember(const entity::SharedProperty &p, Check check, CheckDefault checkDefault, Get get,
                                 const QString &mark, QString &out)
        {
            Q_ASSERT(p);
            if (const auto &func = (p.get()->*get)())
                out.append(QChar::Space + mark + QChar::Space + func->name());
            else if (!(p.get()->*checkDefault)())
                out.append(QChar::Space + mark + QChar::Space + ((p.get()->*check)() ? "true" : "false"));
        }

        template<class F>
        inline void addCommonMember(const entity::SharedProperty &p, F f, const QString &mark, QString &out)
        {
            Q_ASSERT(p);
            if (const auto &name = (p.get()->*f)())
                out.append(QChar::Space + mark + QChar::Space + name->name());
        }
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
    SignatureMaker::SignatureMaker(const db::SharedDatabase &globalDb,
                                   const db::SharedProjectDatabase &projectDb,
                                   const entity::SharedScope &scope, const entity::SharedType &type)
        : m_Type(type)
        , m_Scope(scope)
        , m_GlobalDatabase(globalDb)
        , m_ProjectDatabase(projectDb)
    {
        m_MakersMap[entity::Field::staticHashType()] =
            [&](const entity::SharedBasicEntity &component) {
                return makeField(std::static_pointer_cast<entity::Field>(component));
            };
        m_MakersMap[entity::ClassMethod::staticHashType()] =
                [&](const entity::SharedBasicEntity &component) {
                    return makeMethod(std::static_pointer_cast<entity::ClassMethod>(component));
                };
        m_MakersMap[entity::Property::staticHashType()] =
                [&](const entity::SharedBasicEntity &component) {
                    return makeProperty(std::static_pointer_cast<entity::Property>(component));
                };
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
        return m_MakersMap.value(component->hashType(), [](auto){ return tr("Wrong component"); })(component);
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
        QSet<QString> forbidden = {m_ProjectDatabase->defaultScopeID(), GLOBAL_SCOPE_ID,
                                   LOCALE_TEMPLATE_SCOPE_ID, PROJECT_GLOBAL_SCOPE_ID};
        while (!globalIds.contains(scopeId)) {
            if (auto scope = findScope(scopeId)) {
                if (!scope->name().isEmpty() && !forbidden.contains(scopeId))
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
                }
                if (!parameters.isEmpty())
                    result.append("<").append(parameters.join(", ")).append(">");
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
     * @brief SignatureMaker::typeSignatureById
     * @param id
     * @return
     */
    QString SignatureMaker::typeSignatureById(const QString &id) const
    {
        const auto &type = findType(id);
        return type ? makeTypeOrExtType(type) : "";
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
     * @brief SignatureMaker::makeMethod
     * @param method
     * @return
     */
    QString SignatureMaker::makeMethod(const entity::SharedMethod &method) const
    {
        if (!method)
            return "";

        QString result;

        // Add return type id
        auto returnType = typeSignatureById(method->returnTypeId());
        if (returnType.isEmpty())
            return "";
        result.append(returnType);

        // Add name
        if (method->name().isEmpty())
            return "";
        result.append(QChar::Space).append(method->name());

        // Add parameters
        auto parameters = method->parameters();
        QStringList parametersString;
        parametersString.reserve(parameters.size());
        for (auto &&p : parameters) {
            const auto &s = makeField(p);
            if (s.isEmpty())
                return "";
            parametersString << s;
        }
        result.append("(").append(parametersString.join(", ")).append(")");

        // Add const
        if (method->isConst())
            result.append(QChar::Space).append("const");

        // Add rhs, e.g. default
        if (entity::RhsIdentificator::None != method->rhsIdentificator())
            result.append(QChar::Space).append(utility::methodRhsIdToString(method->rhsIdentificator()));

        // Add lhs, e.g. static
        auto lhsIds = method->lhsIdentificators();
        if (!lhsIds.isEmpty()) {
            QStringList lhsStrings;
            lhsStrings.reserve(lhsIds.count());
            for (auto &&id : lhsIds)
                lhsStrings << utility::methodLhsIdToString(id);
            result.prepend(QChar::Space).prepend(lhsStrings.join(QChar::Space));
        }

        return result;
    }

    /**
     * @brief SignatureMaker::makeProperty
     *
     *        Q_PROPERTY(type name
     *                   (READ getFunction [WRITE setFunction] |
     *                   MEMBER memberName [(READ getFunction | WRITE setFunction)])
     *                   [RESET resetFunction]
     *                   [NOTIFY notifySignal]
     *                   [REVISION int]
     *                   [DESIGNABLE bool]
     *                   [SCRIPTABLE bool]
     *                   [STORED bool]
     *                   [USER bool]
     *                   [CONSTANT]
     *                   [FINAL])
     *
     * @param property
     * @return
     */
    QString SignatureMaker::makeProperty(const entity::SharedProperty &property) const
    {
        if (!property)
            return "";

        // Add type
        QString result = typeSignatureById(property->typeId());
        if (result.isEmpty())
            return "";

        // Add name
        const auto &name = property->name();
        if (name.isEmpty() || name == DEFAULT_NAME)
            return "";
        result.append(QChar::Space).append(name);

        // Add member
        if (const auto &member = property->member())
            result.append(QChar::Space + memberMark + QChar::Space + member->prefix + member->name + member->suffix);

        // Add common members
        addCommonMember(property, &entity::Property::getter,   readMark,   result);
        addCommonMember(property, &entity::Property::setter,   writeMark,  result);
        addCommonMember(property, &entity::Property::resetter, resetMark,  result);
        addCommonMember(property, &entity::Property::notifier, notifyMark, result);

        // Add revision
        if (!property->isRevisionDefault())
            result.append(QChar::Space +  revisionMark + QChar::Space + QString::number(property->revision()));

        // Add designible
        addAdditionalMember(property, &entity::Property::isDesignable, &entity::Property::isDesignableDefault,
                            &entity::Property::designableGetter, designableMark, result);

        // Add scriptable
        addAdditionalMember(property, &entity::Property::isScriptable, &entity::Property::isScriptableDefault,
                            &entity::Property::scriptableGetter, scriptableMark, result);

        // Add stored
        if (!property->isStoredDefault())
            result.append(QChar::Space + storedMark + QChar::Space + (property->isStored() ? "true" : "false"));

        // Add user
        if (!property->isUserDefault())
            result.append(QChar::Space + userMark + QChar::Space + (property->isUser() ? "true" : "false"));

        // Add const
        if (property->isConstant())
            result.append(QChar::Space + constantMark);

        // Add final
        if (property->isFinal())
            result.append(QChar::Space + finalMark);

        return result;
    }

    /**
     * @brief SignatureMaker::findScope
     * @param scope
     * @return
     */
    entity::SharedScope SignatureMaker::findScope(const QString &scopeId) const
    {
        return utility::findScope(scopeId, m_ProjectDatabase, m_GlobalDatabase);
    }

    /**
     * @brief SignatureMaker::type
     * @param typeId
     * @return
     */
    entity::SharedType SignatureMaker::findType(const QString &typeId) const
    {
        return utility::findType(typeId, m_ProjectDatabase, m_GlobalDatabase);
    }

} // namespace translation

