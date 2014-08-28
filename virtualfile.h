#include <QString>
#include <QFileInfo>
#include "types.h"

namespace generator {

    class VirtualFile
    {
    public:
        VirtualFile();
        VirtualFile(VirtualFile &&src);
        VirtualFile(const VirtualFile &src);
        VirtualFile(const QString &path);
        virtual ~VirtualFile();

        VirtualFile &operator =(VirtualFile &&rhs);
        VirtualFile &operator =(VirtualFile rhs);

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        QString baseName() const;

        QString data() const;
        void setData(const QString &data);

        virtual void write() const;
        virtual bool valid() const;

        SharedErrorList errorList() const;
        void setErrorList(const SharedErrorList &errorList);

    protected:
        virtual void copyFrom(const VirtualFile &src);
        virtual void moveFrom(VirtualFile &src);
        void toNativeSeparators(); // just for better displaying

        QFileInfo m_FileInfo;
        QString   m_Data;

        SharedErrorList m_ErrorList;
    };

} // namespace generator
