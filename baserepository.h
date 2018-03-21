#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H
#include <memory>
#include <QString>
#include <QList>
#include <QVariant>
#include "sqlcon.h"
#include "orm_global.h"
using namespace std;

class ORMSHARED_EXPORT BaseRepository
{
protected:
    protected: Sql* sqlCon;
public:
    BaseRepository(Sql* sqlCon);
    void beginTransaction() const;
    void commitTransaction() const;
    void rollbackTransaction() const;
protected :
    template <class T> void saveBean(const shared_ptr<T> & bean ) {
        if (bean->isInsertNew()){
            QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(bean->getTableName(), bean->getInsertFields(), bean->getInsertValuePlaceholders());
             QList<QVariant> params=bean->getInsertParams();
            if (bean->isAutoIncrement()) {

                int id=sqlCon->insert(query,params);

                bean->setAutoIncrementId(id);
                bean->setInsertNew(false);

            } else {
               sqlCon->execute(query,params);
                bean->setInsertNew(false);
            }
        } else  {
            QList<QVariant> params;
            QString updateFields = bean->getUpdateFields(&params).join(QChar(','));

            if (params.size() > 0) {

                QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(bean->getTableName(),updateFields,bean->getUpdateCondition());
                 QList<QVariant> conditionParams=  bean->getUpdateConditionParams();
                params.append(conditionParams);
                sqlCon->execute(query,params);
            }
        }
    }
};

#endif // BASEREPOSITORY_H
