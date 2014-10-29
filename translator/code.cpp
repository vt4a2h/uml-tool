#include "code.h"

namespace translator {

    /**
     * @brief Code::Code
     */
    Code::Code()
        : Code("", "")
    {}

    /**
     * @brief Code::Code
     * @param h
     * @param cpp
     */
    Code::Code(const QString &h, const QString &cpp)
        : toHeader(h)
        , toSource(cpp)
    {}

    /**
     * @brief Code::Code
     * @param src
     */
    Code::Code(const Code &src)
        : Code(src.toHeader, src.toSource)
    {}

    /**
     * @brief Code::join
     * @param code
     * @param sep
     */
    void Code::join(const Code &code, const QString &sep)
    {
        if (!code.toHeader.isEmpty())
            toHeader.append(sep).append(code.toHeader);
        if (!code.toSource.isEmpty())
            toSource.append(sep).append(code.toSource);
    }

    /**
     * @brief Code::isEmpty
     * @return
     */
    bool Code::isEmpty() const
    {
        return toHeader.isEmpty() && toSource.isEmpty();
    }

} // namespace translator
