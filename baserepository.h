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
protected :
    template <class T> void saveBean(const shared_ptr<T> & bean ) {
        if (bean->isInsertNew()){
            QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(bean->getTableName(), bean->getInsertFields(), bean->getInsertValuePlaceholders());
             QList<QVariant> params=bean->getInsertParams();
            if (bean->isAutoIncrement()) {

                int id=sqlCon->insert(query,params);
                if (id==-1) {
                    throw new SqlException(sqlCon->getErrorNr(),sqlCon->getCon().lastError().text());
                } else {
                    bean->setAutoIncrementId(id);
                    bean->setInsertNew(false);
                }
            } else {
                if (!sqlCon->execute(query,params)) {
                    throw new SqlException(sqlCon->getErrorNr(),sqlCon->getCon().lastError().text());
                } else {
                    bean->setInsertNew(false);
                }
            }
        } else  {
            QList<QVariant> params;
            QString updateFields = bean->getUpdateFields(&params).join(QChar(','));

            if (params.size() > 0) {

                QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(bean->getTableName(),updateFields,bean->getUpdateCondition());
                 QList<QVariant> conditionParams=  bean->getUpdateConditionParams();
                params.append(conditionParams);
                if (!sqlCon->execute(query,params)) {
                    throw new SqlException(sqlCon->getErrorNr(),sqlCon->getCon().lastError().text());
                }
            }
        }
    }
};

#endif // BASEREPOSITORY_H
