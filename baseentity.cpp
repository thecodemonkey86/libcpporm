#include "baseentity.h"
#include <QString>


void ORM2::BaseEntity::setLoaded(bool value)
{
    loaded = value;
}

bool ORM2::BaseEntity::isInsertNew() const
{
    return insert;
}

void ORM2::BaseEntity::setInsertNew()
{
    this->insert = true;
}

void ORM2::BaseEntity::setInsertNew(bool value)
{
    insert = value;
}

bool ORM2::BaseEntity::isAutoIncrement() const
{
    return autoIncrement;
}

void ORM2::BaseEntity::setAutoIncrement(bool value)
{
    autoIncrement = value;
}

bool ORM2::BaseEntity::isPrimaryKeyModified() const
{
    return primaryKeyModified;
}

void ORM2::BaseEntity::setPrimaryKeyModified(bool value)
{
    primaryKeyModified = value;
}

ORM2::BaseEntity::BaseEntity(bool autoIncrement) : insert(false),loaded(false),autoIncrement(autoIncrement),primaryKeyModified(false)
{
}

bool ORM2::BaseEntity::isLoaded()
{
    return loaded;
}
