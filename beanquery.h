#ifndef BEANQUERY_H
#define BEANQUERY_H
#include <QVector>
#include "sqlquery.h"
#include <memory>
#include <QDebug>

using namespace std;

template<class T>
class BeanQuery
{
protected:
    QString mainBeanAlias;
    unique_ptr<SqlQuery> qu;
    Sql* sqlCon;
public:
    BeanQuery(Sql* sqlCon) {
        this->qu = move(sqlCon->buildQuery());
        this->sqlCon = sqlCon;
    }

   virtual ~BeanQuery() {

    }

//    static BeanQuery<T> createQuery(Sql* sqlCon, unique_ptr<SqlQuery> qu) {
//        return std::move(unique_ptr<T>(new BeanQuery<T>(sqlCon,std::move(qu))));
//    }

    BeanQuery * select() {
        return this->select(QString("b1"));
    }

    BeanQuery * select(const QString&mainBeanAlias ) {
        this->mainBeanAlias = mainBeanAlias;
        qu->select(T::getAllSelectFields(mainBeanAlias))
        ->from(QString(T::TABLENAME),mainBeanAlias);
        T::addRelatedTableJoins(this);
       // qu = + QString(" FROM ") + ;
        return this;
    }

    BeanQuery* join(const QString &  table,const QString &  alias,const QString &  on) {
        qu->join(table,alias,on);
        return this;
    }

    BeanQuery* join(const QString &  tableAlias,const QString &  on) {
        qu->join(tableAlias,on);
        return this;
    }

    BeanQuery* leftJoin(const QString &  table,const QString &  alias,const QString &  on) {
        qu->leftJoin(table,alias,on);
        return this;
    }

    BeanQuery* where(const QString &  whereCond) {
        qu->where(whereCond);
        return this;
    }

    BeanQuery* where(const QString &  whereCond, const QVariant&param) {
        qu->where(whereCond,param);
        return this;
    }

    BeanQuery* andWhere(const QString &  whereCond, const QVariant&param) {
        qu->andWhere(whereCond,param);
        return this;
    }

    BeanQuery* andWhere(const QString &  whereCond) {
        qu->andWhere(whereCond);
        return this;
    }

//    virtual std::shared_ptr<T> queryOne()=0;
    virtual  QVector<std::shared_ptr<T>> query() =0;

    //BeanQuery* where(const QString &  whereCond, const QList<QVariant>& params);

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

    void printDebug() {
        qDebug()<<qu->toString();
    }
};

#endif // BEANQUERY_H
