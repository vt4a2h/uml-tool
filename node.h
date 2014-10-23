#pragma once

#include <QString>

class QJsonObject;

namespace relationship {

    enum Multiplicity : int;

    class Node
    {
    public:
        Node();
        Node(const QString &typeId, Multiplicity multiplicity = static_cast<Multiplicity>(0));

        friend bool operator ==(const Node &lhs, const Node &rhs);

        Multiplicity multiplicity() const;
        void setMultiplicity(const Multiplicity &multiplicity);

        QString description() const;
        void setDescription(const QString &description);

        QString typeId() const;
        void setTypeId(const QString &typeId);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

    private:
        QString m_TypeId;
        QString m_Description;
        Multiplicity m_Multiplicity;
    };

} // namespace relationship
