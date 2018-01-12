#include "basebean.h"
#include <QString>


void BaseBean::setLoaded(bool value)
{
    loaded = value;
}

bool BaseBean::isInsertNew() const
{
    return insert;
}

void BaseBean::setInsertNew()
{
    this->insert = true;
}

void BaseBean::setInsertNew(bool value)
{
    insert = value;
}

bool BaseBean::isAutoIncrement() const
{
    return autoIncrement;
}

void BaseBean::setAutoIncrement(bool value)
{
    autoIncrement = value;
}

bool BaseBean::isPrimaryKeyModified() const
{
    return primaryKeyModified;
}

void BaseBean::setPrimaryKeyModified(bool value)
{
    primaryKeyModified = value;
}

BaseBean::BaseBean() : BaseBean(true)
{
}

BaseBean::BaseBean(bool insertNew)
{
    this->insert = insertNew;
    this->primaryKeyModified = false;
}

BaseBean::~BaseBean()
{

}

void BaseBean::setAutoIncrementId(int id)
{

}

bool BaseBean::isLoaded()
{
    return loaded;
}
