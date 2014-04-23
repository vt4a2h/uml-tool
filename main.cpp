#include <iostream>
#include "scope.h"
#include "database.h"
#include "type.h"

int main()
{
    db::Database *d = new db::Database();

    auto firstScope = d->addScope("first scope");
    auto foo = firstScope->addType("Foo");

    auto fourthScope = d->addScope("fourth scope");
    auto foobar = fourthScope->addType("FooBar");

    auto secondScope = fourthScope->addChildScope("second scope");
    auto bar = secondScope->addType("Bar");

    auto thridScope = secondScope->addChildScope("thrid scope");
    auto baz = thridScope->addType("Baz");

    std::cout << foobar->id().toStdString() << std::endl;

    auto result = d->depthTypeSearch("Baz");
    std::cout << std::boolalpha << (result != nullptr ? "object founded: " + result->id().toStdString() : "object not founded") << std::endl;

    delete d;

    return 0;
}
