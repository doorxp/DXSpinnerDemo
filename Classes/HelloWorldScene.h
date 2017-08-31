#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DXSpinner/DXSpinner.hpp"

class HelloWorld : public cocos2d::Scene, public Protocol<DXSpinnerDelegate, DXSpinnerDataSource>
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CC_SYNTHESIZE_RETAIN(DXSpinner *, pSpinner, Spinner);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    const std::string titleForRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent);
    ssize_t numberOfRowsInComponent(DXSpinner *spinner, ssize_t inComponent);
    ssize_t numberOfComponents(DXSpinner *spinner);
    
    
    void didSelectRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent);
};

#endif // __HELLOWORLD_SCENE_H__
