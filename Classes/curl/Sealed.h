//
//  Seaked.h
//  WebGame
//
//  Created by 何遵祖 on 13-11-21.

/*
    Sealed.h
    
    此类用于将继承此类的类无法被其它类继承,用于加强类的封装.
 
    使用的示例如下:
 
    class test : virtual public internalSealed::Sealed<test>{
        public:
            test(){
 
        };
        int print(){
            return 1;
            }
    };
 
    class t1 : public test{
        public :
        int print(){
            return 2;
        }
    };
 
    实例化:
    
    test t;
    t1 t2;//会报错
 */

#ifndef __WebGame__Seaked__
#define __WebGame__Seaked__

#include <iostream>

#define Class_Is_Sealed(__CLASS__) public virtual internalSealed::Sealed<__CLASS__>

namespace internalSealed {
    template<typename T>
    class Sealed{
        friend T;
        private:
            Sealed(){};
            ~Sealed(){};
    };
};

#endif /* defined(__WebGame__Seaked__) */

