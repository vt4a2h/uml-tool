#pragma once

#include <QHash>
#include <memory>

namespace entity {
    class Field;

    using SharedField = std::shared_ptr<Field>;
    using Fields = QHash<QString, SharedField>;
    using FieldsList = QList<SharedField>;
}
