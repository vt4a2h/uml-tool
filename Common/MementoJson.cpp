#include "MementoJson.hpp"

#include "BasicElement.h"

namespace Common {

    MementoJson::MementoJson(const BasicElement &elem)
        : m_JsonObj(elem.toJson())
    {
        Q_ASSERT(!m_JsonObj.empty());
    }

    IMemento::Type MementoJson::type() const
    {
        return Json;
    }

    QJsonObject MementoJson::json() const
    {
        return m_JsonObj;
    }

} // namespace Common
