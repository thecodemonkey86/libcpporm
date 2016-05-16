#include "basebean.h"
#include <QString>

BaseBean::BaseBean(Sql * sqlCon)
{
    insert = false;
    idModified = false;
    this->sqlCon = sqlCon;
}

BaseBean::~BaseBean()
{

}

void BaseBean::save()
{
    if (insert){
        if (!sqlInsert()) {
            if (!sqlUpdate()) {
                throw SqlException(sqlCon);
            }
        }
    } else  {
        if (!sqlUpdate()) {
            throw SqlException(sqlCon);
        }
    }
}

void BaseBean::setAutoIncrementId(int id)
{

}

void BaseBean::beginTransaction()
{
    sqlCon->beginTransaction();
}

void BaseBean::commitTransaction()
{
    sqlCon->commitTransaction();
}

void BaseBean::rollbackTransaction()
{
     sqlCon->rollbackTransaction();
}
bool BaseBean::isLoaded() const
{
    return loaded;
}

void BaseBean::setLoaded(bool value)
{
    loaded = value;
}





void BaseBean::setConnection(Sql *sqlCon)
{
    this->sqlCon = sqlCon;
}

void BaseBean::setInsertNew()
{
    this->insert = true;
}

bool BaseBean::sqlInsert()
{
    QString query("INSERT INTO %1 (%2) VALUES (%3)");
    QList<QVariant> params=getInsertParams();
   if (autoIncrement) {

       int id=sqlCon->insert(query.arg( getTableNameInternal(),getInsertFields(),getInsertValuePlaceholders()),params);
       if (id==-1) {
           return false;
       } else {
           setAutoIncrementId(id);
           insert = false;
           return true;
       }
   } else {
       if (!sqlCon->execute(query,params)) {
           qDebug()<<sqlCon->printDebug(query,params);
           return false;
       } else {
           insert = false;
           return true;
       }
   }
}

bool BaseBean::sqlUpdate()
{
    QList<QVariant> params;
    QStringList updateFieldsList=getUpdateFields(&params);

    if (params.size() > 0) {
         QString updateFields = updateFieldsList.join(QChar(','));
        QString query("UPDATE %1 SET %2 WHERE %3");
         QList<QVariant> conditionParams=  getUpdateConditionParams();
        params.append(conditionParams);
        return sqlCon->execute(query.arg(getTableNameInternal(),updateFields,getUpdateCondition()),params);
    }
    return true;
}

 //Sql * BaseBean::sqlCon;
