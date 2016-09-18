#ifndef BEANQUERY_H
#define BEANQUERY_H

class SqlQuery;

#include <QVector>
#include "sqlquery.h"
#include <memory>
#include <QDebug>
#include "orm_global.h"
#include <QSqlResult>

using namespace std;

template<class T>
class BeanQuery
{
protected:
    QString mainBeanAlias;
    shared_ptr<SqlQuery> qu;
    Sql* sqlCon;
public:
    BeanQuery(Sql* sqlCon) {
        this->qu = sqlCon->buildQuery();
        this->sqlCon = sqlCon;
    }

//public: BeanQuery(const BeanQuery&&other) {
//    }

   virtual ~BeanQuery() {

    }

//    static BeanQuery<T> createQuery(Sql* sqlCon, unique_ptr<SqlQuery> qu) {
//        return std::move(unique_ptr<T>(new BeanQuery<T>(sqlCon,std::move(qu))));
//    }

    BeanQuery & select() {
        this->mainBeanAlias = QString("b1");
        qu->select(T::getAllSelectFields(mainBeanAlias))
        ->from(T::getTableName(mainBeanAlias));
        T::addRelatedTableJoins(*this);
       // qu = + QString(" FROM ") + ;
        return *this;
    }

    BeanQuery & select(const QString&mainBeanAlias ) {
        this->mainBeanAlias = mainBeanAlias;
        qu->select(T::getAllSelectFields(mainBeanAlias))
        ->from(T::getTableName(mainBeanAlias));
        return *this;
    }

    BeanQuery & select(const QString&mainBeanAlias, const QString &selectFields ) {
        this->mainBeanAlias = mainBeanAlias;
        qu->select(selectFields)
        ->from(T::getTableName(mainBeanAlias));
        return *this;
    }

    BeanQuery & join(const QString &  table,const QString &  alias,const QString &  on) {
        qu->join(table,alias,on);
        return *this;
    }

    BeanQuery & join(const QString &  tableAlias,const QString &  on) {
        qu->join(tableAlias,on);
        return *this;
    }

    BeanQuery & join(const QString &  table,const QString &  alias,const QString &  on, const QVariant&param) {
        qu->join(table,alias,on,param);
        return *this;
    }

    BeanQuery & join(const QString &  tableAlias,const QString &  on, const QVariant&param) {
        qu->join(tableAlias,on,param);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  table,const QString &  alias,const QString &  on) {
        qu->leftJoin(table,alias,on);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  table,const QString &  alias,const QString &  on, const QVariant&param) {
        qu->leftJoin(table,alias,on,param);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  table,const QString &  alias,const QString &  on, const QList<QVariant>&params) {
        qu->leftJoin(table,alias,on,params);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  tableAlias,const QString &  on) {
        qu->leftJoin(tableAlias,on);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  tableAlias,const QString &  on, const QVariant&param) {
        qu->leftJoin(tableAlias,on,param);
        return *this;
    }

    BeanQuery & leftJoin(const QString &  tableAlias,const QString &  on, const QList<QVariant>&params) {
        qu->leftJoin(tableAlias,on,params);
        return *this;
    }


    BeanQuery & where(const QString &  whereCond) {
        qu->where(whereCond);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond, const QString&param) {
        qu->where(whereCond,param);
        return *this;
    }

    BeanQuery & where(const QString &  whereCond, const QVariant&param) {
        qu->where(whereCond,param);
        return *this;
    }

    BeanQuery & andWhere(const QString &  whereCond, const QVariant&param) {
        qu->andWhere(whereCond,param);
        return *this;
    }

    BeanQuery & andWhere(const QString &  whereCond) {
        qu->andWhere(whereCond);
        return *this;
    }

    BeanQuery & orderBy(const QString & order, const SqlQuery::OrderDirection direction){
        qu->orderBy(order, direction);
         return *this;
    }

    BeanQuery & orderBy(const QString & orderByExpression){
        qu->orderBy(orderByExpression);
         return *this;
    }

    QString toString() {
        return qu->toString();
    }

    void printDebug() {
       qu->debug();
    }

    unique_ptr<QSqlQuery>  execQuery() {
        return std::move(qu->execQuery());
    }

//    virtual std::shared_ptr<T> queryOne()=0;
    virtual  QVector<std::shared_ptr<T>> query() =0;

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
