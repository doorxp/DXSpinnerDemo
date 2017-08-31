//
//  Inherit.h
//  ShojiPlusPlus
//
//  Created by doorxp on 2017/8/10.
//

#ifndef Inherit_h
#define Inherit_h

template <typename T>
class Super : public T {
protected:
    using super = T;
public:
    template<typename ...ArgTypes>
    Super<T>(ArgTypes... args) : T(args...){}
};

struct ProtocolBase {
public:
    virtual void Imp() = 0;
};

template<typename ... TS >
struct Protocol: virtual public TS ... {
protected:
    using protocal = Protocol;
    virtual void Imp()  {

    };
};

#endif /* Inherit_h */
