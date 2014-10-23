#pragma once

#include <QFileInfo>
#include "types.h"

namespace generator {

    class VirtualFileSystemAbstractItem
    {
    public:
        VirtualFileSystemAbstractItem();
        VirtualFileSystemAbstractItem(VirtualFileSystemAbstractItem &&src);
        VirtualFileSystemAbstractItem(const VirtualFileSystemAbstractItem &src);
        VirtualFileSystemAbstractItem(const QString &path);
        virtual ~VirtualFileSystemAbstractItem();

        VirtualFileSystemAbstractItem &operator =(VirtualFileSystemAbstractItem &&rhs);
        VirtualFileSystemAbstractItem &operator =(VirtualFileSystemAbstractItem rhs);

        virtual void write() const;
        virtual bool remove() const;
        virtual bool valid() const;

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        QString baseName() const;

        SharedErrorList errorList() const;
        void setErrorList(const SharedErrorList &errorList);

    protected:
        virtual void copyFrom(const VirtualFileSystemAbstractItem &src);
        virtual void moveFrom(VirtualFileSystemAbstractItem &src);
        void toNativeSeparators(); // just for better displaying

        QFileInfo m_FileInfo;
        SharedErrorList m_ErrorList;
    };

} // namespace generator

