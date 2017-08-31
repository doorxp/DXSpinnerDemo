//
//  DXSpinnerComponent.hpp
//  Shoji++
//
//  Created by doorxp on 2017/8/30.
//

#ifndef DXSpinnerComponent_hpp
#define DXSpinnerComponent_hpp
#include "../MyKit/Inherit.h"

#include <cocos2d.h>
#include <cocos/ui/UIWidget.h>
#include <cocos/ui/UIScale9Sprite.h>
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class DXSpinnerComponent : public Super<cocos2d::ui::Layout> {
    CC_SYNTHESIZE(ssize_t, iItemHeight, ItemHeight);
    CC_SYNTHESIZE(ssize_t, iItemWidth, ItemWidth);
    CC_SYNTHESIZE(ssize_t, iSelectedRow, SelectedRow);
    
    CC_PROPERTY(ssize_t, iNumOfRows, NumOfRows);
public:
    static DXSpinnerComponent *component();
    virtual bool init();
    
    void clear();
    
    void appendItem(const std::string & item, ssize_t idx);
    
    bool pointIn(const cocos2d::Vec2 & point);
    
    virtual void setPosition(float x, float y);
    
    void updateUI();
};

#endif /* DXSpinnerComponent_hpp */
