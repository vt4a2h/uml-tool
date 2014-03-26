#pragma once

#include <QHash>
#include <QMultiHash>
#include <memory>

namespace entity {
    class Field;
    using SharedField = std::shared_ptr<Field>;
    using Fields      = QHash<QString, SharedField>;
    using FieldsList  = QList<SharedField>;

    class ClassMethod;
    using SharedMethod = std::shared_ptr<ClassMethod>;
    using Methods      = QMultiHash<QString, SharedMethod>;
    using MethodsList  = QList<SharedMethod>;

    class Class;
    using SharedClass = std::shared_ptr<Class>;
    using Classes     = QHash<QString, SharedClass>;
    using ClassesList = QList<SharedClass>;

    enum Section : int;
    using Parent      = std::pair<SharedClass, Section>;
    using Parents     = QMultiHash<QString, Parent>;
    using ParentsList = QList<Parent>;
}
