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

    template<class T>
    QSqlQuery prepareInsert()
    {
        QSqlQuery q(sqlCon);
        if(q.prepare( QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders())))
        {
            return q;
        } else {
            throwSqlExceptionWithLine(sqlCon.lastError().nativeErrorCode(), q.lastError().text(),q.lastQuery());
        }

    }

    template<class T>
    QSqlQuery prepareInsertOrIgnorePg()
    {
        QSqlQuery q(sqlCon);
        if(q.prepare( QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders())))
        {
            return q;
        } else {
            throwSqlExceptionWithLine(sqlCon.lastError().nativeErrorCode(), q.lastError().text(),q.lastQuery());
        }

    }

    template<class T> void execInsert(QSqlQuery & preparedStmt,const shared_ptr<T> & entity)
    {
         QList<QVariant> params=entity->getInsertParams();
         for(int i = 0; i < params.size(); i++) {
             preparedStmt.addBindValue(params.at(i));

         }

         if(!preparedStmt.exec())
         {
            throwSqlExceptionWithLine(sqlCon.lastError().nativeErrorCode(), preparedStmt.lastError().text(),preparedStmt.lastQuery());
         }
         if (entity->isAutoIncrement()) {
            entity->setAutoIncrementId(preparedStmt.lastInsertId().toLongLong());
         }
         entity->setInsertNew(false);
    }


    template <class T> void bulkInsert(const QVector<shared_ptr<T>> & entities ) {
        if(!entities.isEmpty()) {
            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QSqlQuery q(sqlCon);
            if(!q.prepare(sql))
            {
                throwSqlExceptionWithLine(q.lastError().nativeErrorCode(),q.lastError().text(),q.lastQuery());
            }
            for(const auto & e : entities) {
                QList<QVariant> params=e->getInsertParams();
                for(const auto & p : params) {
                    q.addBindValue(p);

                }
                if (e->isAutoIncrement()) {

                    if(!q.exec())
                    {
                        throwSqlExceptionWithLine(q.lastError().nativeErrorCode(),q.lastError().text(),q.lastQuery());
                    }
                    int id = q.lastInsertId().toInt();
                    e->setAutoIncrementId(id);
                    e->setInsertNew(false);

                } else {
                    if(!q.exec())
                    {
                        throwSqlExceptionWithLine(q.lastError().nativeErrorCode(),q.lastError().text(),q.lastQuery());
                    }
                    e->setInsertNew(false);
                }
            }



        }
    }

    /**
     * bulk insert or update of many entity instances using one prepared statement
     */
    template <class T> void bulkSave(const QVector<shared_ptr<T>> & entities ) {
        if(!entities.isEmpty()) {

            for(const auto & b : entities) {
                if (b->isInsertNew()){
                    for(const auto & bean : entities) {
                        if (bean->isInsertNew()){
                            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(b->getTableName(), b->getInsertFields(), b->getInsertValuePlaceholders());
                            QSqlQuery q(sqlCon);
                            bool res = q.prepare(sql);
                            for(const auto & bean : entities) {
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
            for(const auto & b : entities) {
                if (!b->isInsertNew()){
                    for(const auto & bean : entities) {
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

    template <class T> void save(const shared_ptr<T> & entity ) {
        if (entity->isInsertNew()){
            QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QList<QVariant> params=entity->getInsertParams();
            if (entity->isAutoIncrement()) {
#ifdef QT_DEBUG
                qDebug() << SqlUtil3::Sql::getDebugString(query,params);
#endif
                entity->setAutoIncrementId(SqlUtil3::Sql::insert(sqlCon, query,params));

            } else {
                SqlUtil3::Sql::execute(sqlCon, query,params);
            }
             entity->setInsertNew(false);
        } else  {
            QList<QVariant> params;
            QString updateFields = entity->getUpdateFields(&params).join(QChar(','));

            if (params.size() > 0) {

                QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(T::getTableName(),updateFields,entity->getUpdateCondition());
                QList<QVariant> conditionParams=  entity->getUpdateConditionParams();
                params.append(conditionParams);
#ifdef QT_DEBUG
                qDebug() << query;
#endif
                SqlUtil3::Sql::execute(sqlCon,query,params);
            }
        }
    }

    template<class T> void insertOrIgnorePg(const shared_ptr<T> & entity) {
        QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
        QList<QVariant> params=entity->getInsertParams();
        SqlUtil3::Sql::execute(sqlCon, query,params);
        entity->setInsertNew(false);
    }

};

#endif // BASEREPOSITORY_H
