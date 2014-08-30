#include <QString>
#include <QFileInfo>
#include "virtualfilesystemabstractitem.h"

namespace generator {

    class VirtualFile : public VirtualFileSystemAbstractItem
    {
    public:
        VirtualFile();
        VirtualFile(const QString &path);

        QString data() const;
        void setData(const QString &data);

        void write() const override;
        bool remove() const override;

    private:
        QString   m_Data;
    };

} // namespace generator
