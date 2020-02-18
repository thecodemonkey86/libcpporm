#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H
#include <memory>
#include <QString>
#include <QList>
#include <QVariant>
#include "sqlcon.h"
#include <QSqlQuery>
#include "orm_global.h"
#ifdef QT_DEBUG
  #include <QDebug>
#endif

using namespace std;
using namespace SqlUtil3;

class ORMSHARED_EXPORT BaseRepository
{
protected:
protected: QSqlDatabase sqlCon;
public:
    BaseRepository(const QSqlDatabase & sqlCon);
    bool beginTransaction() const;
    bool commitTransaction() const;
    bool rollbackTransaction() const;

    QSqlDatabase getSqlCon() const;

protected :
    template <class T> void bulkInsert(const QVector<shared_ptr<T>> & beans ) {
        if(!beans.isEmpty()) {
            auto bean0 = beans[0];
            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(bean0->getTableName(), bean0->getInsertFields(), bean0->getInsertValuePlaceholders());
            QSqlQuery q(sqlCon);
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
                throw SqlUtil3::SqlException(sqlCon.lastError().nativeErrorCode(), sqlCon.driver()->lastError().text(),sql);
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
                            QSqlQuery q(sqlCon);
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
                                throw SqlUtil3::SqlException(sqlCon.lastError().nativeErrorCode(), sqlCon.driver()->lastError().text(),sql);
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
                                QSqlQuery q(sqlCon);
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
                #ifdef QT_DEBUG
                qDebug() << SqlUtil3::Sql::getDebugString(query,params);
                #endif
                int id=SqlUtil3::Sql::insert(sqlCon, query,params);

                bean->setAutoIncrementId(id);
                bean->setInsertNew(false);

            } else {
                SqlUtil3::Sql::execute(sqlCon, query,params);
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
                SqlUtil3::Sql::execute(sqlCon,query,params);
            }
        }
    }

    template<class T> void upsertPg(const shared_ptr<T> & entity) {
      QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(entity->getTableName(), entity->getInsertFields(), entity->getInsertValuePlaceholders());
      QList<QVariant> params=entity->getInsertParams();
      SqlUtil3::Sql::execute(sqlCon, query,params);
      entity->setInsertNew(false);
    }
};

#endif // BASEREPOSITORY_H
