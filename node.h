#pragma once

#include <QString>

namespace relationship {

    enum Multiplicity : int;

    class Node
    {
    public:
        Node();
        Node(const QString &typeId, Multiplicity multiplicity = static_cast<Multiplicity>(0));

        QString typeId() const;
        void setTypeId(const QString &typeId);

        Multiplicity multiplicity() const;
        void setMultiplicity(const Multiplicity &multiplicity);

    private:
        QString m_TypeId;
        Multiplicity m_Multiplicity;
    };

} // namespace relationship
