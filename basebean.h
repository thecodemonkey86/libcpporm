#ifndef BASEBEAN_H
#define BASEBEAN_H
#include <QString>
#include "orm_global.h"
#include "sqlcon.h"
#include <QSet>
#include <memory>

using namespace std;

class ORMSHARED_EXPORT BaseBean
{
public:
    BaseBean(Sql * sqlCon);
    virtual ~BaseBean();
    virtual void save();
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
protected:
    bool insert;
    bool loaded;
//    static QString tableName;
    virtual QString getInsertFields()=0;
    virtual QStringList getUpdateFields(QList<QVariant>* params)=0;
    virtual QString getInsertValuePlaceholders()=0;
    virtual QString getUpdateCondition()=0;
    virtual QList<QVariant> getInsertParams()=0;
    virtual QList<QVariant> getUpdateConditionParams()=0;
    virtual void setAutoIncrementId(int id);
    Sql * sqlCon;
    virtual QString getTableNameInternal()=0;
    bool autoIncrement, idModified;
public:

    void setConnection(Sql* sqlCon);
    void setInsertNew();
    bool isLoaded() const;
    void setLoaded(bool value);

private:
    inline bool sqlInsert();
    inline bool sqlUpdate();
};

#endif // BASEBEAN_H
