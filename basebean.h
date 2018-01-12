#ifndef BASEBEAN_H
#define BASEBEAN_H
#include <QString>
#include "orm_global.h"
#include "sqlcon.h"
#include <QSet>
#include <memory>

using namespace std;

class ORMSHARED_EXPORT BaseBean
{
protected:
    bool insert;
    bool loaded;
    bool autoIncrement, primaryKeyModified;

public:
    BaseBean();
    BaseBean(bool insertNew);
    virtual ~BaseBean();
public:
    virtual void setAutoIncrementId(int id);

    bool isLoaded();

    void setLoaded(bool value);
    bool isInsertNew() const;
    void setInsertNew();
    void setInsertNew(bool value);
    bool isAutoIncrement() const;
    void setAutoIncrement(bool value);
    bool isPrimaryKeyModified() const;
    void setPrimaryKeyModified(bool value);
};

#endif // BASEBEAN_H
