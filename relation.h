#pragma once

#include "types.h"
#include <QString>

namespace relationship {

    class Relation
    {
    public:
        Relation();
        Relation(const QString &tailTypeId, const QString &headTypeId);
        virtual ~Relation();

        // TODO: add firs and second nodes maker

        QString description() const;
        void setDescription(const QString &description);

        virtual make() = 0;
        virtual clear() = 0;

    private:
        SharedNode m_TailNode;
        SharedNode m_HeadNode;
        QString m_Description;
    };

} // namespace relationship
