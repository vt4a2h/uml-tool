#include "helpfunctions.h"

namespace utility {

    QString genId()
    {
        return QUuid::createUuid().toString();
    }

}
