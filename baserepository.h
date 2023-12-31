#pragma once
#include <memory>
#include <QString>
#include <QList>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <exception/sqlexception.h>
#include <QSqlDriver>
#include "sqlcon.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif
namespace ORM2 {
template <class DbConnectionPool>
class  BaseRepository
{
BaseRepository()=delete ;
public:

   static void beginTransaction()
   {
     auto con = DbConnectionPool::getDatabase();
     if(!con.driver()->beginTransaction())
     {
         throwSqlExceptionWithLine(con.driver()->lastError().nativeErrorCode(),con.driver()->lastError().text(),QString());
     }
   }
   static void commitTransaction()
   {
     auto con = DbConnectionPool::getDatabase();
     if(!con.driver()->commitTransaction())
     {
       throwSqlExceptionWithLine(con.driver()->lastError().nativeErrorCode(),con.driver()->lastError().text(),QString());
     }
   }
   static void rollbackTransaction()
   {
     auto con = DbConnectionPool::getDatabase();
     if(!con.driver()->rollbackTransaction())
     {
       throwSqlExceptionWithLine(con.driver()->lastError().nativeErrorCode(),con.driver()->lastError().text(),QString());
     }
   }

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
         entity->resetModifiedFlags();
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
                e->resetModifiedFlags();
            }



        }
    }

    /**
     * bulk insert or update of many entity instances using one prepared statement
     */
    template <class T> static void bulkSave(const QVector<std::shared_ptr<T>> & entities ) {
      if(!entities.isEmpty()) {

        for(const auto & entity : entities) {
          if (entity->isInsertNew()){
            QString sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(entity->getTableName(), entity->getInsertFields(), entity->getInsertValuePlaceholders());
            QSqlQuery q(DbConnectionPool::getDatabase());
            bool res = q.prepare(sql);
            for(const auto & entity : entities) {
              QList<QVariant> params=entity->getInsertParams();
              for(int i = 0; i < params.size(); i++) {
                q.addBindValue(params.at(i));

              }
              res &= q.exec();
              if (entity->isAutoIncrement()) {


                int id = q.lastInsertId().toInt();
                entity->setAutoIncrementId(id);
              }
              entity->setInsertNew(false);
              entity->resetModifiedFlags();
            }

            if(!res) {
              throw SqlUtil4::SqlException(q.lastError().nativeErrorCode(), q.driver()->lastError().text(),sql);
            }

            break;
          }

        }


        for(const auto & entity : entities) {
          if (!entity->isInsertNew()){
            QList<QVariant> params;
            QString updateFields = entity->getUpdateFields(&params).join(QChar(','));

            if (params.size() > 0) {

              QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(entity->getTableName(),updateFields,entity->getUpdateCondition());
              QList<QVariant> conditionParams=  entity->getUpdateConditionParams();
              params.append(conditionParams);
#ifdef QT_DEBUG
              qDebug() << query;
#endif
              QSqlQuery q(DbConnectionPool::getDatabase());
              if(!q.prepare(query))
              {
                  throwSqlExceptionWithLine(q.lastError().nativeErrorCode(),q.lastError().text(),q.lastQuery());
              }
              for(int i = 0; i < params.size(); i++) {
                q.addBindValue(params.at(i));

              }
              if(!q.exec())
              {
                   throwSqlExceptionWithLine(q.lastError().nativeErrorCode(),q.lastError().text(),q.lastQuery());
              }
               entity->resetModifiedFlags();
            }
          }
        }

        }



    }

    template <class T> static void save(std::shared_ptr<T> entity ) {
      save(*entity);
    }


   template <class T> static void save(T & entity ) {
        if (entity.isInsertNew()){
            QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QList<QVariant> params=entity.getInsertParams();
            if (entity.isAutoIncrement()) {
#ifdef QT_DEBUG
                qDebug() << SqlUtil4::Sql::getDebugString(query,params);
#endif
                entity.setAutoIncrementId(SqlUtil4::Sql::insert(DbConnectionPool::getDatabase(), query,params));

            } else {
                SqlUtil4::Sql::execute(DbConnectionPool::getDatabase(), query,params);
            }
             entity.setInsertNew(false);
             entity.resetModifiedFlags();
        } else  {
            QList<QVariant> params;
            QString updateFields = entity.getUpdateFields(&params).join(QChar(','));

            if (params.size() > 0) {

                QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(T::getTableName(),updateFields,entity.getUpdateCondition());
                QList<QVariant> conditionParams=  entity.getUpdateConditionParams();
                params.append(conditionParams);
#ifdef QT_DEBUG
                qDebug() << SqlUtil4::Sql::getDebugString(query,params);
#endif
                SqlUtil4::Sql::execute(DbConnectionPool::getDatabase(),query,params);
                entity.resetModifiedFlags();
            }
        }
    }


};


}

