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

void BaseEntity::setPrimaryKeyModified(bool value)
{
    primaryKeyModified = value;
}

BaseEntity::BaseEntity() : BaseEntity(true)
{
}

BaseEntity::BaseEntity(bool insertNew)
{
    this->insert = insertNew;
    this->primaryKeyModified = false;
}

//BaseEntity::~BaseEntity()
//{

//}

//void BaseEntity::setAutoIncrementId(int id)
//{

//}

bool BaseEntity::isLoaded()
{
    return loaded;
}
