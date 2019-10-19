//
// Created by Brian on 26/09/2019.
//

#include "ORMContainer.hpp"

b12software::babel::ORM *b12software::babel::ORMContainer::currentORM = nullptr;

void b12software::babel::ORMContainer::SetDefaultORM(b12software::babel::ORM *orm)
{
    currentORM = orm;
}

void b12software::babel::ORMContainer::SetDefaultORM(const std::string &dbName)
{
    currentORM = new ORM(dbName);
}

b12software::babel::ORM *b12software::babel::ORMContainer::GetCurrentORM()
{
    return currentORM;
}

void b12software::babel::ORMContainer::CleanORM()
{
    delete currentORM;
}
