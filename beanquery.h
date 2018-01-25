#ifndef BEANQUERY_H
#define BEANQUERY_H

class SqlQuery;

#include <QVector>
#include "sqlquery.h"
#include <memory>
#include <QDebug>
#include "orm_global.h"
#include <QSqlResult>
#include <QSqlDriver>

using namespace std;

template<class T>
class BeanQuery
{
protected:
    QString mainBeanAlias;
    QString selectFields;
    QString fromTable;
    QStringList orderByExpressions;
    QStringList joinTables;
    QStringList conditions;
    QStringList group;
    int64_t limitResults, resultOffset;
    QString limitOffsetCondition;
    QString limitOffsetOrderBy;
    QString deleteFromTable;
    QList<QVariant> params;
    Sql*sqlCon;


public:
    BeanQuery(Sql* sqlCon) {
        this->sqlCon = sqlCon;
        this->limitResults = 0;
        this->resultOffset = -1;
    }

//public: BeanQuery(const BeanQuery&&other) {
//    }

   virtual ~BeanQuery() {

    }



//    static BeanQuery<T> createQuery(Sql* sqlCon, unique_ptr<SqlQuery> qu) {
//        return std::move(unique_ptr<T>(new BeanQuery<T>(sqlCon,std::move(qu))));
//    }

    BeanQuery & select() {
        this->mainBeanAlias = QStringLiteral("b1");
        this->selectFields = T::getAllSelectFields(mainBeanAlias);
        this->fromTable = T::getTableName(mainBeanAlias);
        T::addRelatedTableJoins(*this);
       // qu = + QString(" FROM ") + ;
        return *this;
    }

    BeanQuery & select(const QString&mainBeanAlias ) {
        this->mainBeanAlias = mainBeanAlias;
        this->selectFields = T::getAllSelectFields(mainBeanAlias);
        this->fromTable = T::getTableName(mainBeanAlias);
        return *this;
    }

    BeanQuery & select(const QString&mainBeanAlias, const QString &selectFields ) {
        this->mainBeanAlias = mainBeanAlias;
        this->selectFields = selectFields;
        this->fromTable = T::getTableName(mainBeanAlias);
        return *this;
    }


    BeanQuery & join(const QString &joinTable, const QString &alias, const QString & on)
    {
        this->joinTables.append(QStringLiteral(" JOIN %1 %2 ON %3").arg( joinTable, alias, on));
        return *this;
    }

    BeanQuery & join(const QString &joinTableAlias, const QString &on)
    {
        this->joinTables.append(QStringLiteral(" JOIN %1 ON %2").arg( joinTableAlias,on));
        return *this;
    }

    BeanQuery & join(const QString &joinTable, const QString &alias, const QString &on, const QVariant &param)
    {
        this->params.append(param);
        return join(joinTable,alias,on);
    }

    BeanQuery & join(const QString &joinTableAlias, const QString &on, const QVariant &param)
    {
        this->params.append(param);
        return join(joinTableAlias,on);
    }

    BeanQuery & join(const QString &joinTable, const QString &alias, const QString &on, const QList<QVariant> &params)
    {
        this->params.append(params);
        return join(joinTable,alias,on);
    }

    BeanQuery & join(const QString &joinTableAlias, const QString &on, const QList<QVariant> &params)
    {
        this->params.append(params);
        return join(joinTableAlias,on);
    }

    BeanQuery & leftJoin(const QString &joinTable, const QString &alias, const QString & on)
    {
        this->joinTables.append(QStringLiteral(" LEFT JOIN %1 %2 ON %3").arg(joinTable, alias,on));
        return *this;
    }

    BeanQuery & leftJoin(const QString &joinTable, const QString &alias, const QString &on, const QVariant &param)
    {
        this->params.append(param);
        return leftJoin(joinTable,alias,on);
    }

    BeanQuery & leftJoin(const QString &joinTable, const QString &alias, const QString &on, const QList<QVariant> &params)
    {
        this->params.append(params);
        return leftJoin(joinTable,alias,on);
    }

    BeanQuery & leftJoin(const QString &joinTableAlias, const QString &on)
    {
        this->joinTables.append(QStringLiteral(" LEFT JOIN %1 ON %2").arg(joinTableAlias,on));
        return *this;
    }

    BeanQuery & leftJoin(const QString &joinTableAlias, const QString &on, const QVariant &param)
    {
        this->params.append(param);
        return leftJoin(joinTableAlias,on);
    }

    BeanQuery & leftJoin(const QString &joinTableAlias, const QString &on, const QList<QVariant> &params)
    {
        this->params.append(params);
        return leftJoin(joinTableAlias,on);
    }

    BeanQuery & where(const QString &  whereCond) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->conditions.append(whereCond);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond,int param) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->conditions.append(whereCond);
        this->params.append(param);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond,int param1, int param2) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->params.append(QVariant(param1));
        this->params.append(QVariant(param2));
        this->conditions.append(whereCond);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond,int param1, int param2, int param3) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->params.append(QVariant(param1));
        this->params.append(QVariant(param2));
        this->params.append(QVariant(param3));
        this->conditions.append(whereCond);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond,const QList<QVariant>&params) {
        this->params.append(params);
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->conditions.append(whereCond);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond, const QString&param) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->conditions.append(whereCond);
        this->params.append(param);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond, const QVariant&param) {
        if (!conditions.empty()) {
            this->conditions.append(SqlQuery::AND);
        }
        this->conditions.append(whereCond);
        this->params.append(param);
        return *this;
    }

    BeanQuery & andWhere(const QString &  whereCond, const QVariant&param) {
        this->conditions.append(SqlQuery::AND);
        this->conditions.append(whereCond);
        this->params.append(param);
        return *this;
    }

    BeanQuery & andWhere(const QString &  whereCond) {
        this->conditions.append(SqlQuery::AND);
        this->conditions.append(whereCond);
        return *this;
    }

    BeanQuery & orderBy(const QString & order, const SqlQuery::OrderDirection direction = SqlQuery::ORDER_ASC){
        this->orderByExpressions.append(QStringLiteral("%1 %2 ").arg(order, (direction == SqlQuery::ORDER_ASC ? QStringLiteral("asc") : QStringLiteral("desc")) ));
        return *this;
    }



    QString toString() {
        QString query;
        if (!selectFields.isEmpty()) {
            query += QStringLiteral("SELECT %1 FROM %2").arg(selectFields, fromTable);
        } else if (!deleteFromTable.isEmpty()) {
             query+= QStringLiteral("DELETE FROM %1").arg(deleteFromTable);
        }

        for(int i=0;i<joinTables.size();i++) {
            query+=joinTables.at(i);
        }

        if (!conditions.empty()) {

            if (limitResults > 0 || resultOffset > -1) {
                query += QStringLiteral(" WHERE (");
            } else {
                query += QStringLiteral(" WHERE ");
            }

            for(const QString &cond: conditions) {
                query += cond;
            }
        }

        if (group.size()>0) {
            query += QStringLiteral(" GROUP BY %1").arg(group.at(0));
            for(int i=1;i<group.size();i++) {
                query += QStringLiteral(", %1").arg(group.at(i));
            }
        }
        if (limitResults > 0 || resultOffset > -1) {
            if (!conditions.empty()) {
                query += QStringLiteral(") AND %1").arg(T::getLimitQueryString(limitResults,resultOffset,limitOffsetCondition));
            } else {
                query += QStringLiteral(" WHERE %1").arg(T::getLimitQueryString(limitResults,resultOffset,limitOffsetCondition));
            }
            if (!limitOffsetOrderBy.isEmpty()) {
                query += QStringLiteral(" ORDER BY %1").arg(limitOffsetOrderBy);
            }
        }

        query += QStringLiteral(" ORDER BY ");
        for(auto order : this->orderByExpressions) {
            query += QStringLiteral("%1,").arg(order);
        }
        query += this->orderByPrimaryKey();

        return query;
    }

    QString getDebugString() {
        QString result(toString());
       for(int i=0;i<params.size();i++) {
    //       qDebug()<<params.at(i).typeName();
           QString v= QString(params.at(i).typeName())!= QString( "QByteArray") ? params.at(i).toString() :QString(params.at(i).toByteArray().toHex());
           QRegExp e("^[0-9][0-9]*$");
            result.replace(result.indexOf(QChar('?')),1,
                          v.isNull()?QString("NULL"): e.exactMatch(v)?v:QString("'")+ v+ QString("'"));
       }
       return result;
    }

    void printQDebug() {
        qDebug() << getDebugString();
    }

    void printDebug() {

       std::cout<<getDebugString().toUtf8().data();
    }

    BeanQuery & deleteFrom(){
        this->deleteFromTable = T::getTableName();
        return *this;
    }

    bool execute() {
         return *this->sqlCon->execute(toString(),params);
    }

    QSqlQuery  execQuery() {
        QSqlQuery q(sqlCon->getCon());
        q.setForwardOnly(true);
        if (q.prepare(toString())) {


            for(int i=0;i<params.size();i++) {
                q.addBindValue(params.at(i));

            }
            if (!q.exec()) {
                QString msg=q.lastError().text();
                qDebug()<<msg;
                qDebug()<<q.driver()->lastError().text();
                throw SqlException(sqlCon->getErrorNr(),q.driver()->lastError().text(), toString());
            }
            return q;

        } else {
            QString msg=q.lastError().text();
            qDebug()<<msg;
            throw SqlException(sqlCon->getErrorNr(), q.driver()->lastError().text(),toString());
        }
    }

    virtual  QVector<std::shared_ptr<T>> query() =0;
    virtual std::shared_ptr<T> queryOne()=0;

    BeanQuery & limit(int64_t limit, const  QString & condition, const  QString & orderBy = QString()) {
        this->limitResults = limit;
        this->resultOffset = -1;
        this->limitOffsetCondition = condition;
        this->limitOffsetOrderBy = orderBy;
        return *this;
    }

    BeanQuery & limitAndOffset(int64_t limit,int64_t offset, const  QString & condition, const  QString & orderBy = QString()) {
        this->limitResults = limit;
        this->resultOffset = offset;
        this->limitOffsetCondition = condition;
        this->limitOffsetOrderBy = orderBy;
        return *this;
    }

    BeanQuery & offset(int64_t offset, const  QString & condition, const  QString & orderBy = QString()) {
        this->limitResults = -1;
        this->resultOffset = offset;
        this->limitOffsetCondition = condition;
        this->limitOffsetOrderBy = orderBy;
        return *this;
    }

protected:
    virtual QString orderByPrimaryKey() const = 0;

    //BeanQuery & where(const QString &  whereCond, const QList<QVariant>& params);

    /* std::shared_ptr<T> queryOne() {
        std::unique_ptr<QSqlQuery> res= qu->execQuery();
        return T::fetchOne(std::move(res));
    }

    QVector<std::shared_ptr<T>> query() {
        std::unique_ptr<QSqlQuery> res= qu->execQuery();
        return T::fetchList(std::move(res));
    }

   T* queryOne() {
            std::unique_ptr<QSqlQuery> res= qu->execQuery();
            return T::fetchOne(std::move(res));
        }

        QVector<T*> query() {
            std::unique_ptr<QSqlQuery> res= qu->execQuery();
            return T::fetchList(std::move(res));
        }*/

   // void printDebug() {
      //  qDebug()<<qu->toString();
   // }
};



#endif // BEANQUERY_H
