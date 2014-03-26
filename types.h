#pragma once

#include <QHash>
#include <memory>

namespace entity {
    class Field;
    using SharedField = std::shared_ptr<Field>;
    using Fields      = QHash<QString, SharedField>;
    using FieldsList  = QList<SharedField>;

    class ClassMethod;
    using SharedMethod = std::shared_ptr<ClassMethod>;
    using Methods      = QHash<QString, SharedMethod>;
    using MethodsList  = QList<SharedMethod>;

    class Class;
    using SharedClass = std::shared_ptr<Class>;
    using Classes     = QHash<QString, SharedClass>;
    using ClassesList = QList<SharedClass>;

    enum Section : int;
    using ClassParent  = std::pair<SharedClass, Section>;
    using ClassParents = QList<ClassParent>;
}
