#pragma once
#include "orm_global.h"

namespace ORM2 {

class ORMSHARED_EXPORT BaseEntity
{
protected:
    bool insert;
    bool loaded;
    bool autoIncrement, primaryKeyModified;

public:
    BaseEntity(bool autoIncrement);  
public:
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
}
