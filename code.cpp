#include "code.h"

namespace translator {

    Code::Code()
        : Code("", "")
    {}

    Code::Code(const QString &h, const QString &cpp)
        : toHeader(h)
        , toSource(cpp)
    {}

    Code::Code(const Code &src)
        : Code(src.toHeader, src.toSource)
    {}

    void Code::join(const Code &code, const QString &sep)
    {
        if (!code.toHeader.isEmpty())
            toHeader.append(sep).append(code.toHeader);
        if (!code.toSource.isEmpty())
            toSource.append(sep).append(code.toSource);
    }

    bool Code::isEmpty() const
    {
        return toHeader.isEmpty() && toSource.isEmpty();
    }

} // namespace translator
