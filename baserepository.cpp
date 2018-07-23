#include "baserepository.h"

Sql *BaseRepository::getSqlCon() const
{
    return sqlCon;
}

BaseRepository::BaseRepository(Sql *sqlCon)
{
    this->sqlCon = sqlCon;
}

bool BaseRepository::beginTransaction() const
{
    return this->sqlCon->beginTransaction();
}

bool BaseRepository::commitTransaction() const
{
    return this->sqlCon->commitTransaction();
}

bool BaseRepository::rollbackTransaction() const
{
    return  this->sqlCon->rollbackTransaction();
}

