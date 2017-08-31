//
//  MyMath.hpp
//  ShojiPlusPlus
//
//  Created by doorxp on 2017/8/30.
//

#ifndef MyMath_hpp
#define MyMath_hpp

namespace My {
    template <typename T>
    T clip(T value, T minValue, T maxValue) {
        T r = value;
        if (r < minValue) {
            r = minValue;
        }
        
        if (r>maxValue) {
            r = maxValue;
        }
        
        return r;
    }
}



#endif /* MyMath_hpp */
