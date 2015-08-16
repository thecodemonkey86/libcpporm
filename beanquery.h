#ifndef BEANQUERY_H
#define BEANQUERY_H
#include <QList>
#include "sqlquery.h"

template<class T>
class BeanQuery
{
protected:
   QString mainBeanAlias;
SqlQuery* qu;
public:
    BeanQuery(SqlQuery* qu) {
       this->qu = qu;
    }

    ~BeanQuery() {

    }

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

    //BeanQuery* where(const QString &  whereCond, const QList<QVariant>& params);

    T* queryOne() {
        QSqlQuery * res= qu->execQuery();
        return T::fetchOne(res);
    }

    QList<T*>* query() {
        QSqlQuery * res= qu->execQuery();
        QList<T*>* beans = T::fetchList(res);
        return beans;
    }
};

#endif // BEANQUERY_H
