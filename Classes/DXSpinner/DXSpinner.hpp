//
//  DXSpinner.hpp
//  ShojiPlusPlus
//
//  Created by doorxp on 2017/8/29.
//

#ifndef DXSpinner_hpp
#define DXSpinner_hpp

#include "DXSpinnerComponent.hpp"

class DXSpinnerDelegate;
class DXSpinnerDataSource;

class DXSpinner: public Super<cocos2d::ui::Layout> {
public:
    using THIS = DXSpinner;
    
    virtual bool init();
    
public:
    static DXSpinner *spinner();
    
    ssize_t numberOfComponents();
    ssize_t numberOfRowsInComponents(ssize_t inComponent);
    
    void reloadAllComponents();
    void reloadComponent(ssize_t inComponent);
    
    ssize_t selectedRow(ssize_t inComponent);
    
    void selectRow(ssize_t row, ssize_t inComponent, bool animated);
    
public:
    
    void onEnter();
    
    void setContentSize(const cocos2d::Size & size);
    void deaccelerateScrolling(float dt);
    void relocateContainer(bool animated);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    
    CC_SYNTHESIZE_RETAIN(cocos2d::DrawNode *, pIndicator, Indicator);
    CC_SYNTHESIZE_RETAIN(cocos2d::ClippingNode *, pMask, Mask);
    CC_SYNTHESIZE(DXSpinnerDelegate *, pDelegate, Delegate);
    CC_SYNTHESIZE(DXSpinnerDataSource *, pDataSource, DataSource);
    
public:
    void setParent(Node * parent);
protected:
    cocos2d::Vector<DXSpinnerComponent *> components;
    
    DXSpinnerComponent *opComponent;
    cocos2d::Vec2 opStartPnt;
    cocos2d::Vec2 touchStartPnt;
    
    float ySpeed;
    
    float opMinY;
    float opMaxY;
    bool dragging;
    
    ssize_t iItemWidth;
    ssize_t iItemHeight;
    ssize_t iComponents;
    
    std::vector<ssize_t> vSelectedRows;
};

struct DXSpinnerDelegate : ProtocolBase {
    virtual const std::string titleForRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent) = 0;
    
    virtual void didSelectRowInComponent(DXSpinner *spinner, ssize_t row, ssize_t inComponent) {}
};

struct DXSpinnerDataSource : ProtocolBase {
    virtual ssize_t numberOfComponents(DXSpinner *spinner) {return 1;};
    virtual ssize_t numberOfRowsInComponent(DXSpinner *spinner, ssize_t inComponent) = 0;
};

#endif /* DXSpinner_hpp */
