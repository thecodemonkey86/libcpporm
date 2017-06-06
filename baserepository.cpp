#include "baserepository.h"

BaseRepository::BaseRepository(Sql *sqlCon)
{
    this->sqlCon = sqlCon;
}

