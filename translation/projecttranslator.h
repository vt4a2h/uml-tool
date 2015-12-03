/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include <functional>

#include <QHash>

#include <db/db_types.hpp>
#include <entity/entity_types.hpp>

namespace translation {

    struct Code;

    /**
     * @brief The ProjectTranslator class
     */
    class ProjectTranslator
    {
    public:
        /**
         * @brief The TranslatorOption enum
         */
        enum TranslatorOption {
            NoOptions       = 0x00,
            WithNamespace   = 0x01,
            WithAlias       = 0x02,
            GenerateNumbers = 0x04,
            NoLhs           = 0x08,
            NoDefaultName   = 0x10,
        };
        Q_DECLARE_FLAGS(TranslatorOptions, TranslatorOption)

        ProjectTranslator();
        ProjectTranslator(const db::SharedDatabase &globalDb,
                          const db::SharedProjectDatabase &projectDb);

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const db::SharedDatabase &globalDatabase);

        db::SharedProjectDatabase projectDatabase() const;
        void setProjectDatabase(const db::SharedProjectDatabase &projectDatabase);

        Code translate(const entity::SharedBasicEntity &e,
                       const TranslatorOptions &options = WithNamespace,
                       const db::SharedDatabase &localeDatabase = nullptr,
                       const db::SharedDatabase &classDatabase = nullptr) const;

        Code generateClassMethodsImpl(const entity::SharedClass &_class,
                                      const db::SharedDatabase &localeDatabase = nullptr) const;
        Code generateClassMethodsImpl(const entity::SharedTemplateClass &_class) const;

        void addNamespace(const entity::SharedType &type, Code &code, uint indentCount = 1);

    private: // Translators
        Code translateType(const entity::SharedType &type,
                           const TranslatorOptions &options = WithNamespace,
                           const db::SharedDatabase &localeDatabase = nullptr,
                           const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateExtType(const entity::SharedExtendedType &extType,
                              const TranslatorOptions &options = WithNamespace,
                              const db::SharedDatabase &localeDatabase = nullptr,
                              const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateField(const entity::SharedField &field,
                            const TranslatorOptions &options = WithNamespace,
                            const db::SharedDatabase &localeDatabase = nullptr,
                            const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateEnum(const entity::SharedEnum &_enum,
                           const TranslatorOptions &options = NoOptions,
                           const db::SharedDatabase &localeDatabase = nullptr,
                           const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateMethod(const entity::SharedMethod &method,
                             const TranslatorOptions &options = NoOptions,
                             const db::SharedDatabase &localeDatabase = nullptr,
                             const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateUnion(const entity::SharedUnion &_union,
                            const TranslatorOptions &options = NoOptions,
                            const db::SharedDatabase &localeDatabase = nullptr,
                            const db::SharedDatabase &classDatabase = nullptr) const;
        Code translateClass(const entity::SharedClass &_class,
                            const TranslatorOptions &options = NoOptions,
                            const db::SharedDatabase &localeDatabase = nullptr,
                            const db::SharedDatabase &classDatabase = nullptr) const;

    private:
        void checkDb() const;
        void makeCallbacks();
        QString generateCodeForExtTypeOrType(const QString &id, const TranslatorOptions &options,
                                             const db::SharedDatabase &localeDatabase = nullptr,
                                             const db::SharedDatabase &classDatabase = nullptr) const;
        void generateClassSection(const entity::SharedClass &_class,
                                  const db::SharedDatabase &localeDatabase,
                                  entity::Section section, QString &out) const;
        void generateMethods(const entity::MethodsList &methods, const db::SharedDatabase &localeDatabase,
                             const QString &indent, QString &out) const;
        void generateFileds(const entity::FieldsList &fields, const entity::SharedClass &_class,
                            const db::SharedDatabase &localeDatabase, const QString &indent,
                            QString &out) const;
        void generateTemplatePart(QString &result, const entity::SharedTemplate &t,
                                  bool withDefaultTypes = true) const;
        bool toHeader(const entity::SharedMethod &m,
                      const db::SharedDatabase &classDatabase = nullptr) const;
        void generateSectionMethods(const entity::MethodsList &methods,
                                    const db::SharedDatabase &localeDatabase,
                                    bool needSection, entity::Section section,
                                    const QString &marker, QString &out) const;

        db::SharedDatabase m_GlobalDatabase;
        db::SharedProjectDatabase m_ProjectDatabase;

        using TranslatorsMap = QHash<size_t, std::function<Code(const entity::SharedBasicEntity &,
                                                                const ProjectTranslator::TranslatorOptions &,
                                                                const db::SharedDatabase &,
                                                                const db::SharedDatabase &)>>;

        TranslatorsMap m_translators;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(ProjectTranslator::TranslatorOptions)

} // namespace translation
