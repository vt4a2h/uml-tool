#include "globaldatabase.h"

int main()
{
    db::GlobalDatabase::instance();
    db::GlobalDatabase::instance();
    db::GlobalDatabase::instance();
    db::GlobalDatabase::instance();
    db::GlobalDatabase::instance();

    return 0;
}
