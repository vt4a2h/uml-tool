#pragma once

#include <QString>
#include "types.h"

class QJsonObject;
class QStringList;

namespace entity {

    enum Section : int;
    enum FieldKeyword : int;

    class Field
    {
    public:
        Field();
        Field(const QString &name, const QString &typeId);
        Field(const QString &name, const QString &typeId, const QString &prefix, Section section);

        friend bool operator== (const Field &lhs, const Field &rhs);

        QString name() const;
        QString fullName() const;
        void setName(const QString &name);

        Section section() const;
        void setSection(Section section);

        QString prefix() const;
        void removePrefix();
        void setPrefix(const QString &prefix);

        FieldKeywordsList keywords() const;
        void addKeyword(FieldKeyword keyword);
        bool containsKeyword(FieldKeyword keyword) const;
        bool hasKeywords() const;
        void removeKeyword(FieldKeyword keyword);

        QString typeId() const;
        void setTypeId(const QString &typeId);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        QString suffix() const;
        void removeSuffix();
        void setSuffix(const QString &suffix);

        virtual Field *clone() const;

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

    private:
        QString m_TypeId;
        Section m_Section;
        QString m_Name;
        QString m_Prefix;
        QString m_Suffix;

        FieldKeywords m_Keywords;
    };

} // namespace entity
