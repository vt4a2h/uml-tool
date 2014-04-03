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

        QString name() const;
        void setName(const QString &name);

        Section section() const;
        void setSection(Section section);

        QString prefix() const;
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

    private:
        QString m_TypeId;
        Section m_Section;
        QString m_Name;
        QString m_Prefix;

        FieldKeywords m_Keywords;
    };

} // namespace entity
