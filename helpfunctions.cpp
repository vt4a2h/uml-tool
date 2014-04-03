#include "helpfunctions.h"

#include <functional>

#include <QUuid>
#include <QString>
#include <QJsonObject>
#include <QStringList>

namespace utility {

    QString genId()
    {
        return QUuid::createUuid().toString();
    }

    void checkAndSet(const QJsonObject &object, const QString &key, QStringList &lst, std::function<void ()> func)
    {
        object.contains(key) ? func() : lst.append(QObject::tr("Key \"%1\" not found!").arg(key));
    }

}
