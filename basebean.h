#ifndef BASEBEAN_H
#define BASEBEAN_H
#include <QString>
#include "orm_global.h"
#include "sqlcon.h"
#include <QSet>

class ORMSHARED_EXPORT BaseBean
{
public:
    BaseBean();
    virtual ~BaseBean();
    virtual void save();

protected:
    bool insert;
    static QString tableName;
    virtual QString getInsertFields()=0;
    virtual QString getUpdateFields(QList<QVariant>* params)=0;
    virtual QString getInsertValuePlaceholders()=0;
    virtual QString getUpdateCondition()=0;
    virtual QList<QVariant>* getInsertParams()=0;
    virtual QList<QVariant>* getUpdateConditionParams()=0;
    virtual void setAutoIncrementId(int id);
    static Sql * sqlCon;
    bool autoIncrement, idModified;
public:
    virtual QString getTableName()=0;
    static void setConnection(Sql*sqlCon);
    void setInsertNew();
};

#endif // BASEBEAN_H
