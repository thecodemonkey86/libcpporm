#include "basebean.h"
#include <QString>

BaseBean::BaseBean()
{
    insert = false;
    idModified = false;
}

BaseBean::~BaseBean()
{

}

void BaseBean::save()
{
    if (insert){
        QString query("INSERT INTO " + getTableName()+ " ("+getInsertFields()+ ") VALUES ("+getInsertValuePlaceholders()+")");
         QList<QVariant>* params=getInsertParams();
        if (autoIncrement) {

            int id=sqlCon->insert(query,params);
            if (id==-1) {
                delete params;
                throw new SqlException(sqlCon);
            } else {
                setAutoIncrementId(id);
                insert = false;
                delete params;
            }
        } else {
            if (!sqlCon->execute(query,params)) {
                delete params;
                throw new SqlException(sqlCon);
            } else {
                insert = false;
                delete params;
            }
        }
    } else  {
        QList<QVariant>* params=new QList<QVariant>();
        QString updateFields = getUpdateFields(params);

        if (params->size() > 0) {

            QString query("UPDATE " + getTableName()+ " SET "+updateFields+ " WHERE "+getUpdateCondition());
             QList<QVariant>* conditionParams=  getUpdateConditionParams();
            params->append(*conditionParams);
            if (!sqlCon->execute(query,params)) {
                delete params;
                throw new SqlException(sqlCon);
            }
        }
        delete params;
    }
}

void BaseBean::setAutoIncrementId(int id)
{

}



void BaseBean::setConnection(Sql *sqlCon)
{
    BaseBean::sqlCon = sqlCon;
}

void BaseBean::setInsertNew()
{
    this->insert = true;
}

 Sql * BaseBean::sqlCon;
