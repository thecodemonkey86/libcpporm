#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H
#include <cstdint>
#include <memory>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "exception/sqlexception.h"
#include "sqlcon.h"
#include <QList>
#include "orm_global.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif


class ORMSHARED_EXPORT BaseRepository
{
protected: QSqlDatabase sqlCon;
public:
    BaseRepository(const QSqlDatabase & sqlCon);
    bool beginTransaction() const;
//    bool beginTransactionIsolationLevelSerializable() const;
    bool commitTransaction() const;
    bool rollbackTransaction() const;

    QSqlDatabase getSqlCon() const;

    template<class T>
    QSqlQuery prepareInsert() const
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
    QSqlQuery prepareInsertOrIgnorePg() const
    {
        QSqlQuery q(sqlCon);
        if(q.prepare( QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders())))
        {
            return q;
        } else {
            throwSqlExceptionWithLine(sqlCon.lastError().nativeErrorCode(), q.lastError().text(),q.lastQuery());
        }

    }

    template<class T> void execInsert(QSqlQuery & preparedStmt,const std::shared_ptr<T> & entity)
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


    template <class T> void bulkInsert(const QVector<std::shared_ptr<T>> & entities ) {
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
                    e->setAutoIncrementId(q.lastInsertId().toLongLong());
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
     * bulk insert or update of many entity instances using one prepared statement (insert only)
     */
    template <class T> void bulkSave(const QVector<std::shared_ptr<T>> & entities ) {
        if(!entities.isEmpty()) {


            QString sqlInsert = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
            QSqlQuery qInsert(sqlCon);
            if(!qInsert.prepare(sqlInsert))
            {
                throwSqlExceptionWithLine(qInsert.lastError().nativeErrorCode(),qInsert.lastError().text(),qInsert.lastQuery());
            }

            for(const auto & entity : entities) {
                if (!entity->isInsertNew()){
                    QList<QVariant> params;
                    QString updateFields = entity->getUpdateFields(&params).join(QChar(','));

                    if (params.size() > 0) {

                        QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(T::getTableName(),updateFields,entity->getUpdateCondition());
                        QList<QVariant> conditionParams=  entity->getUpdateConditionParams();
                        params.append(conditionParams);
#ifdef QT_DEBUG
                        qDebug() << query;
#endif
                        QSqlQuery q(sqlCon);
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
                    }
                } else {
                    QList<QVariant> params=entity->getInsertParams();
                    for(int i = 0; i < params.size(); i++) {
                        qInsert.addBindValue(params.at(i));

                    }
                    if(!qInsert.exec())
                    {
                        throwSqlExceptionWithLine(qInsert.lastError().nativeErrorCode(),qInsert.lastError().text(),qInsert.lastQuery());
                    }
                    if (entity->isAutoIncrement()) {
                        entity->setAutoIncrementId(qInsert.lastInsertId().toLongLong());
                    }
                    entity->setInsertNew(false);
                }

            }


        }


    }

    template <class T> void save(std::shared_ptr<T> entity ) {
      save(*entity);
    }

    template<class T> void insertOrIgnorePg(std::shared_ptr<T> entity) {
      insertOrIgnorePg(*entity);
    }



    template <class T> void save(T & entity ) {
      if (entity.isInsertNew()){
        QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
        QList<QVariant> params=entity.getInsertParams();
        if (entity.isAutoIncrement()) {
#ifdef QT_DEBUG
          qDebug() << SqlUtil3::Sql::getDebugString(query,params);
#endif
          entity.setAutoIncrementId(SqlUtil3::Sql::insert(sqlCon, query,params));

        } else {
          SqlUtil3::Sql::execute(sqlCon, query,params);
        }
        entity.setInsertNew(false);
      } else  {
        QList<QVariant> params;
        QString updateFields = entity.getUpdateFields(&params).join(QChar(','));

        if (params.size() > 0) {
          #ifdef QT_DEBUG
          qDebug()<<sqlCon.databaseName();
            #endif

          QString query = QStringLiteral("UPDATE %1 SET %2 WHERE %3").arg(T::getTableName(),updateFields,entity.getUpdateCondition());
          QList<QVariant> conditionParams=  entity.getUpdateConditionParams();
          params.append(conditionParams);
//#ifdef QT_DEBUG
          qDebug().noquote() << SqlUtil3::Sql::getDebugString(query,params);
          qDebug().noquote() << sqlCon.databaseName();

//#endif
          SqlUtil3::Sql::execute(sqlCon,query,params);
        }
      }
    }

    template<class T> void insertOrIgnorePg(T & entity) {
      QString query = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT DO NOTHING").arg(T::getTableName(), T::getInsertFields(), T::getInsertValuePlaceholders());
      QList<QVariant> params=entity.getInsertParams();
      SqlUtil3::Sql::execute(sqlCon, query,params);
      entity.setInsertNew(false);
    }

};

#endif // BASEREPOSITORY_H
