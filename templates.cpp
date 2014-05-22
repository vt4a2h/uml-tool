#include <QString>

namespace {

    const QString ENUM_TEMPLATE = "enum %class%%name%%type% {%values%};";
    const QString EXT_TYPE_TEMPLATE = "%const%%name%%pl%%template_params%";
    const QString FIELD_TEMPLATE = "%keywords%%type%%name%";

}
