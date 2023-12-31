#include "baseentity.h"
#include <QString>


void BaseEntity::setLoaded(bool value)
{
    loaded = value;
}

bool BaseEntity::isInsertNew() const
{
    return insert;
}

void BaseEntity::setInsertNew()
{
    this->insert = true;
}

void BaseEntity::setInsertNew(bool value)
{
    insert = value;
}

bool BaseEntity::isAutoIncrement() const
{
    return autoIncrement;
}

void BaseEntity::setAutoIncrement(bool value)
{
    autoIncrement = value;
}

bool BaseEntity::isPrimaryKeyModified() const
{
    return primaryKeyModified;
}



BaseEntity::BaseEntity(bool autoIncrement) : insert(false),loaded(false),autoIncrement(autoIncrement),primaryKeyModified(false)
{
}


bool BaseEntity::isLoaded()
{
    return loaded;
}
