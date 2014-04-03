#pragma once

class QString;
class QJsonObject;
class QStringList;

namespace std {
    template<class T>
    class function;
}

namespace utility {
    QString genId();
    void checkAndSet(const QJsonObject& object, const QString& key, QStringList& lst, std::function<void()> func);
}

