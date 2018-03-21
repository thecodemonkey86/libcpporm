#include "baserepository.h"

BaseRepository::BaseRepository(Sql *sqlCon)
{
    this->sqlCon = sqlCon;
}

void BaseRepository::beginTransaction() const
{
    this->sqlCon->beginTransaction();
}

void BaseRepository::commitTransaction() const
{
    this->sqlCon->commitTransaction();
}

void BaseRepository::rollbackTransaction() const
{
    this->sqlCon->rollbackTransaction();
}

