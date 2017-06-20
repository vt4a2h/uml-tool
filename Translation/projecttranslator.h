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

#include <DB/DBTypes.hpp>
#include <Entity/entity_types.hpp>
#include <Common/common_types.h>

namespace Translation {

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
        ProjectTranslator(const DB::SharedDatabase &globalDb,
                          const DB::SharedProjectDatabase &projectDb);

        DB::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const DB::SharedDatabase &globalDatabase);

        DB::SharedProjectDatabase projectDatabase() const;
        void setProjectDatabase(const DB::SharedProjectDatabase &projectDatabase);

        Code translate(const Common::SharedBasicEntity &e,
                       const TranslatorOptions &options = WithNamespace,
                       const DB::SharedDatabase &localeDatabase = nullptr,
                       const DB::SharedDatabase &classDatabase = nullptr) const;

        Code generateClassMethodsImpl(const Entity::SharedClass &_class,
                                      const DB::SharedDatabase &localeDatabase = nullptr) const;
        Code generateClassMethodsImpl(const Entity::SharedTemplateClass &_class) const;

        void addNamespace(const Entity::SharedType &type, Code &code, uint indentCount = 1);

    private: // Translators
        Code translateType(const Entity::SharedType &type,
                           const TranslatorOptions &options = WithNamespace,
                           const DB::SharedDatabase &localeDatabase = nullptr,
                           const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateExtType(const Entity::SharedExtendedType &extType,
                              const TranslatorOptions &options = WithNamespace,
                              const DB::SharedDatabase &localeDatabase = nullptr,
                              const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateField(const Entity::SharedField &field,
                            const TranslatorOptions &options = WithNamespace,
                            const DB::SharedDatabase &localeDatabase = nullptr,
                            const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateEnum(const Entity::SharedEnum &_enum,
                           const TranslatorOptions &options = NoOptions,
                           const DB::SharedDatabase &localeDatabase = nullptr,
                           const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateMethod(const Entity::SharedMethod &method,
                             const TranslatorOptions &options = NoOptions,
                             const DB::SharedDatabase &localeDatabase = nullptr,
                             const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateUnion(const Entity::SharedUnion &_union,
                            const TranslatorOptions &options = NoOptions,
                            const DB::SharedDatabase &localeDatabase = nullptr,
                            const DB::SharedDatabase &classDatabase = nullptr) const;
        Code translateClass(const Entity::SharedClass &_class,
                            const TranslatorOptions &options = NoOptions,
                            const DB::SharedDatabase &localeDatabase = nullptr,
                            const DB::SharedDatabase &classDatabase = nullptr) const;

    private:
        void checkDb() const;
        void makeCallbacks();
        QString generateCodeForExtTypeOrType(const Common::ID &id, const TranslatorOptions &options,
                                             const DB::SharedDatabase &localeDatabase = nullptr,
                                             const DB::SharedDatabase &classDatabase = nullptr) const;
        void generateClassSection(const Entity::SharedClass &_class,
                                  const DB::SharedDatabase &localeDatabase,
                                  Entity::Section section, QString &out) const;
        void generateMethods(const Entity::MethodsList &methods, const DB::SharedDatabase &localeDatabase,
                             const QString &indent, QString &out) const;
        void generateFileds(const Entity::FieldsList &fields, const Entity::SharedClass &_class,
                            const DB::SharedDatabase &localeDatabase, const QString &indent,
                            QString &out) const;
        void generateTemplatePart(QString &result, const Entity::SharedTemplate &t,
                                  bool withDefaultTypes = true) const;
        bool toHeader(const Entity::SharedMethod &m,
                      const DB::SharedDatabase &classDatabase = nullptr) const;
        void generateSectionMethods(const Entity::MethodsList &methods,
                                    const DB::SharedDatabase &localeDatabase,
                                    bool needSection, Entity::Section section,
                                    const QString &marker, QString &out) const;

        DB::SharedDatabase m_GlobalDatabase;
        DB::SharedProjectDatabase m_ProjectDatabase;

        using TranslatorsMap = QHash<size_t, std::function<Code(const Common::SharedBasicEntity &,
                                                                const ProjectTranslator::TranslatorOptions &,
                                                                const DB::SharedDatabase &,
                                                                const DB::SharedDatabase &)>>;

        TranslatorsMap m_translators;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(ProjectTranslator::TranslatorOptions)

} // namespace translation
