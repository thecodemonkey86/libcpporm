#pragma once
#include <memory>
#include <QString>
#include <QList>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include "orm_global.h"
#include "sqlcon.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif
namespace ORM2 {
template <class DbConnectionPool>
class ORMSHARED_EXPORT BaseRepository
{
public:
    BaseRepository()=delete;
    bool beginTransaction() const;
    bool commitTransaction() const;
    bool rollbackTransaction() const;


    template<class T>
   static QSqlQuery prepareInsert()
    {
        QSqlQuery q(DbConnectionPool::getDatabase());
        if(q.prepare( QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders())))
        {
            return q;
        } else {
            throwSqlExceptionWithLine(DbConnectionPool::getDatabase().lastError().nativeErrorCode(), q.lastError().text(),q.lastQuery());
        }

    }

    template<class T>
   static  QSqlQuery prepareInsertOrIgnorePg()
    {
        QSqlQuery q(DbConnectionPool::getDatabase());
        if(q.prepare( QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders())))
        {
            return q;
        } else {
            throwSqlExceptionWithLine(DbConnectionPool::getDatabase().lastError().nativeErrorCode(), q.lastError().text(),q.lastQuery());
        }

    }

    template<class T>
   static void execInsert(QSqlQuery & preparedStmt,const std::shared_ptr<T> & entity)
    {
         QList<QVariant> params=entity->getInsertParams();
         for(int i = 0; i < params.size(); i++) {
             preparedStmt.addBindValue(params.at(i));

         }

         if(!preparedStmt.exec())
         {
            throwSqlExceptionWithLine(DbConnectionPool::getDatabase().lastError().nativeErrorCode(), preparedStmt.lastError().text(),preparedStmt.lastQuery());
         }
         if (entity->isAutoIncrement()) {
            entity->setAutoIncrementId(preparedStmt.lastInsertId().toLongLong());
         }
         entity->setInsertNew(false);
    }


    template <class T>
   static void bulkInsert(const QVector<std::shared_ptr<T>> & entities ) {
        if(!entities.isEmpty()) {
            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QSqlQuery q(DbConnectionPool::getDatabase());
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
    template <class T>
   static void bulkSave(const QVector<std::shared_ptr<T>> & entities ) {
        if(!entities.isEmpty()) {

            for(const auto & b : entities) {
                if (b->isInsertNew()){
                    for(const auto & bean : entities) {
                        if (bean->isInsertNew()){
                            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(b->getTableName(), b->getInsertFields(), b->getInsertValuePlaceholders());
                            QSqlQuery q(DbConnectionPool::getDatabase());
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
                                throw SqlUtil3::SqlException(DbConnectionPool::getDatabase().lastError().nativeErrorCode(), DbConnectionPool::getDatabase().driver()->lastError().text(),sql);
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
                                QSqlQuery q(DbConnectionPool::getDatabase());
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

    template <class T>
   static void save(const std::shared_ptr<T> & entity ) {
        if (entity->isInsertNew()){
            QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QList<QVariant> params=entity->getInsertParams();
            if (entity->isAutoIncrement()) {
#ifdef QT_DEBUG
                qDebug() << SqlUtil3::Sql::getDebugString(query,params);
#endif
                entity->setAutoIncrementId(SqlUtil3::Sql::insert(DbConnectionPool::getDatabase(), query,params));

            } else {
                SqlUtil3::Sql::execute(DbConnectionPool::getDatabase(), query,params);
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
                SqlUtil3::Sql::execute(DbConnectionPool::getDatabase(),query,params);
            }
        }
    }

    template<class T>
   static void insertOrIgnorePg(const std::shared_ptr<T> & entity) {
        QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
        QList<QVariant> params=entity->getInsertParams();
        SqlUtil3::Sql::execute(DbConnectionPool::getDatabase(), query,params);
        entity->setInsertNew(false);
    }

};
}

