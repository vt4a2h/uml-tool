#pragma once

#include <QString>

namespace {

    const QString INDENT("    ");
    const QString DOUBLE_INDENT(INDENT + INDENT);
    const QString ENUM_TEMPLATE("enum %class%%name%%type% {%values%};");
    const QString EXT_TYPE_TEMPLATE("%const%%name%%pl%%template_params%");
    const QString FIELD_TEMPLATE("%keywords%%type%%name%");
    const QString METHOD_TEMPLATE("%lhs_k%%r_type%%name%(%parameters%)%rhs_k%%const%");
    const QString UNION_TEMPLATE("union %name% {%variables%};");
    const QString CLASS_TEMPLATE("%kind%%name%%parents%{%section%};");
    const QString TEMPLATE_TEMPLATE("template <%template_parameters%>");
    const QString TEMPLATE("%template%");
    const QString SCOPE_TEMPLATE_HEADER("namespace %name%\n{\n\n%code%\n\n}");
    const QString SCOPE_TEMPLATE_SOURCE("namespace %name%\n{\n%code%\n}");

}
