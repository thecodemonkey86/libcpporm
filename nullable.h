#ifndef NULLABLE_H
#define NULLABLE_H

namespace ORM2 {
template<class T>
class Nullable
{
public:
    Nullable(T val) :value(val),valIsNull(false) {
    }

    Nullable() : valIsNull(true) {
    }

    ~Nullable(){

    }


protected:
    bool valIsNull;

 T value;
 public:

 bool isNull() const {
     return valIsNull;
 }

    void setNull() {
        valIsNull = true;
    }

    void setVal(T val) {
        valIsNull = false;
        this->value = val;
    }

    const T & val() const {
        return value;
    }
};


}


#endif // NULLABLE_H
