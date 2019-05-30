#ifndef BASEENTITY_H
#define BASEENTITY_H
#include <QString>
#include "orm_global.h"
#include "sqlcon.h"
#include <QSet>
#include <memory>

using namespace std;

class ORMSHARED_EXPORT BaseEntity
{
protected:
    bool insert;
    bool loaded;
    bool autoIncrement, primaryKeyModified;

public:
    BaseEntity();
    BaseEntity(bool insertNew);
    virtual ~BaseEntity();
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

#endif // BASEENTITY_H
