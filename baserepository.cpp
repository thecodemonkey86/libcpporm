#include "baserepository.h"
#include <QSqlDriver>

QSqlDatabase BaseRepository::getSqlCon() const
{
    return sqlCon;
}

BaseRepository::BaseRepository(const QSqlDatabase & sqlCon)
{
    this->sqlCon = sqlCon;
}

bool BaseRepository::beginTransaction() const
{
    return sqlCon.driver()->beginTransaction();
}

bool BaseRepository::commitTransaction() const
{
    return sqlCon.driver()->commitTransaction();
}

bool BaseRepository::rollbackTransaction() const
{
    return sqlCon.driver()->rollbackTransaction();
}

