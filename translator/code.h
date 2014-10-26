#pragma once

#include <QString>

namespace translator {

    struct Code {
       Code();
       Code(const QString &h, const QString &cpp);
       Code(const Code &src);

       void join(const Code &code, const QString &sep = "\n");

       bool isEmpty() const;

       QString toHeader;
       QString toSource;
    };

} // namespace code
