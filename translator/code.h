#pragma once

#include <QString>

/**
 * @brief translator
 */
namespace translator {

    /**
     * @brief The Code struct
     */
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
