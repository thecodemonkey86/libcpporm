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
    bool beginTransaction() const;
    bool commitTransaction() const;
    bool rollbackTransaction() const;
    Sql *getSqlCon() const;

protected :
    template <class T> void bulkInsert(const QVector<shared_ptr<T>> & beans ) {
        if(!beans.isEmpty()) {
            auto bean0 = beans[0];
            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(bean0->getTableName(), bean0->getInsertFields(), bean0->getInsertValuePlaceholders());
            QSqlQuery q(sqlCon->getCon());
            bool res = q.prepare(sql);
            for(const auto & bean : beans) {
                QList<QVariant> params=bean->getInsertParams();
                for(int i = 0; i < params.size(); i++) {
                    q.addBindValue(params.at(i));

                }
                if (bean->isAutoIncrement()) {

                    res &= q.exec();
                    int id = q.lastInsertId().toInt();
                    bean->setAutoIncrementId(id);
                    bean->setInsertNew(false);

                } else {
                    res &= q.exec();
                    bean->setInsertNew(false);
                }
            }

            if(!res) {
                throw SqlException(q.lastError().number(),q.lastError().text());
            }

        }
    }
	
	  /**
     * bulk insert or update of many entity instances using one prepared statement
     */
    template <class T> void bulkSave(const QVector<shared_ptr<T>> & beans ) {
        if(!beans.isEmpty()) {

            for(const auto & b : beans) {
                if (b->isInsertNew()){
                    for(const auto & bean : beans) {
                        if (bean->isInsertNew()){
                            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(b->getTableName(), b->getInsertFields(), b->getInsertValuePlaceholders());
                            QSqlQuery q(sqlCon->getCon());
                            bool res = q.prepare(sql);
                            for(const auto & bean : beans) {
                                QList<QVariant> params=bean->getInsertParams();
                                for(int i = 0; i < params.size(); i++) {
                                    q.addBindValue(params.at(i));

                                }
                                res &= q.exec();
                                if (bean->isAutoIncrement()) {


                                    int id = q.lastInsertId().toInt();
                                    bean->setAutoIncrementId(id);
                                }
                                bean->setInsertNew(false);
                            }

                            if(!res) {
                                throw SqlException(q.lastError().number(),q.lastError().text());
                            }
                        }
                    }
                    break;
                }
            }
            for(const auto & b : beans) {
                if (!b->isInsertNew()){
                    for(const auto & bean : beans) {
                        if (!bean->isInsertNew()){
                            QList<QVariant> params;
                            QString updateFields = bean->getUpdateFields(&params).join(QChar(','));

                            if (params.size() > 0) {

                                QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(bean->getTableName(),updateFields,bean->getUpdateCondition());
                                QList<QVariant> conditionParams=  bean->getUpdateConditionParams();
                                params.append(conditionParams);
#ifdef QT_DEBUG
                                qDebug() << query;
#endif
                                QSqlQuery q(sqlCon->getCon());
                                bool res = q.prepare(query);
                                for(int i = 0; i < params.size(); i++) {
                                    q.addBindValue(params.at(i));

                                }
                                res &= q.exec();
                            }
                        }
                    }
                    break;
                }
            }
        }


    }

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
#ifdef QT_DEBUG
                qDebug() << query;
#endif
                sqlCon->execute(query,params);
            }
        }
    }
};

#endif // BASEREPOSITORY_H
