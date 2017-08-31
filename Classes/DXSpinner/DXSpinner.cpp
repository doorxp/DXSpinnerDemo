//
//  DXSpinner.cpp
//  ShojiPlusPlus
//
//  Created by doorxp on 2017/8/29.
//

#include "DXSpinner.hpp"
#include "../Utls/MyMath.hpp"

#define SCROLL_DEACCEL_RATE  0.5f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f
#define MOVE_INCH            7.0f/160.0f
#define BOUNCE_BACK_FACTOR   0.35f

using namespace cocos2d;

DXSpinner *DXSpinner::spinner() {
    DXSpinner *pRet = new DXSpinner();
    if (!pRet || !pRet->init()) {
        if (pRet) {
            pRet->release();
        }
        return nullptr;
    }
    
    pRet->autorelease();
    
    return pRet;
}

bool DXSpinner::init() {
    if (!super::init()) {
        return false;
    }
    
    setTouchEnabled(true);
    
    dragging = false;
    
    pMask = nullptr;
    pIndicator = nullptr;
    
    iItemHeight = 50;
    iItemWidth = 0;
    
    setClippingEnabled(true);
    setClippingType(cocos2d::ui::Layout::ClippingType::STENCIL);
    
    cocos2d::DrawNode *stencil = cocos2d::DrawNode::create();
    
    cocos2d::ClippingNode *clippingNode = cocos2d::ClippingNode::create(stencil);

    setMask(clippingNode);
    
    this->addChild(clippingNode);
    
    opComponent = nullptr;
    
    setIndicator(cocos2d::DrawNode::create());
    
    if (pIndicator) {
        this->addChild(pIndicator);
    }
    
    return true;
}

void DXSpinner::setContentSize(const cocos2d::Size & size) {
    super::setContentSize(size);
    
    if (pMask) {
        cocos2d::DrawNode *stencil = dynamic_cast<cocos2d::DrawNode *>(pMask->getStencil());
        if (stencil) {
            cocos2d::Vec2 start;
            start.x = 10;
            start.y = 10;
            
            cocos2d::Vec2 end = this->getContentSize();
            end.x -= 10;
            end.y -= 10;
            
            stencil->drawSolidRect(start, end, cocos2d::Color4F(1,1,1,0.6));
        }
    }
    
    if (pIndicator) {
        pIndicator->setContentSize(this->getContentSize());
        float x, y;
        x = 0;
        y = this->getContentSize().height/2 - iItemHeight/2;
        Vec2 start = Vec2(x+10, y);
        x = this->getContentSize().width;
        Vec2 end = Vec2(x-10, y);
        pIndicator->drawLine(start, end, cocos2d::Color4F::GRAY);
        
        x = 0;
        y = this->getContentSize().height/2 + iItemHeight/2;
        start = Vec2(x+10, y);
        x = this->getContentSize().width;
        end = Vec2(x-10, y);
        pIndicator->drawLine(start, end, cocos2d::Color4F::GRAY);
    }
}

void DXSpinner::onEnter() {
    super::onEnter();
}

bool DXSpinner::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    
    if (!this->isVisible())
    {
        return false;
    }
    
    Vec2 pnt = touch->getLocation();
    
    Rect r;
    r.origin = Vec2::ZERO;
    r.size = this->getContentSize();
    
    if (!r.containsPoint(this->convertTouchToNodeSpace(touch))) {
        return false;
    }
    
    if (!!opComponent) {
        this->relocateContainer(false);
    }
    
    
    
    for (int i = 0; i<components.size(); i++) {
        DXSpinnerComponent *component = components.at(i);
        if (component->pointIn(pnt)) {
            opComponent = component;
            break;
        }
    }
    
    if (opComponent) {
        dragging = true;
        ySpeed = 0;
        touchStartPnt = pnt;
        opStartPnt = opComponent->getPosition();
        opMinY = this->getContentSize().height/2 + iItemHeight/2;
        opMaxY = opMinY + opComponent->getContentSize().height - iItemHeight/2;
    }
   
    
    return !!opComponent;
}

void DXSpinner::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    Vec2 pnt = touch->getLocation();
    ySpeed = pnt.y - touchStartPnt.y;
    float y = ySpeed;
    y = opStartPnt.y + y;
    float newMinY  = opMinY - iItemHeight;
    float newMaxY = opMaxY + iItemHeight;
    y = My::clip(y, newMinY, newMaxY);
    opComponent->setPositionY(y);
}

void DXSpinner::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    Vec2 pnt = touch->getLocation();
    float y = pnt.y - touchStartPnt.y;
    y = opStartPnt.y + y;
    float newMinY  = opMinY - iItemHeight;
    float newMaxY = opMaxY + iItemHeight;
    y = My::clip(y, newMinY, newMaxY);
    opComponent->setPositionY(y);
    dragging = false;
    
    ySpeed = (touch->getLocation() - touch->getPreviousLocation()).y;
    
    if (fabsf(ySpeed) > FLT_EPSILON) {
        this->schedule(CC_SCHEDULE_SELECTOR(THIS::deaccelerateScrolling));
    }
    else {
        this->relocateContainer(true);
    }
}

void DXSpinner::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    Vec2 pnt = touch->getLocation();
    float y = pnt.y - touchStartPnt.y;
    y = opStartPnt.y + y;
    
    float newMinY  = opMinY - iItemHeight;
    float newMaxY = opMaxY + iItemHeight;
    
    y = My::clip(y, newMinY, newMaxY);
    opComponent->setPositionY(y);
    dragging = false;

    ySpeed = (touch->getLocation() - touch->getPreviousLocation()).y;
    if (fabsf(ySpeed) > FLT_EPSILON) {
        this->schedule(CC_SCHEDULE_SELECTOR(THIS::deaccelerateScrolling));
    }
    else {
        this->relocateContainer(true);
    }
    
}

void DXSpinner::deaccelerateScrolling(float dt) {
    if(dragging || !opComponent) {
        this->unschedule(CC_SCHEDULE_SELECTOR(THIS::deaccelerateScrolling));
        return ;
    }
    
    float y = opComponent->getPositionY() + ySpeed;
    
    float newMaxY = opMaxY + iItemHeight;
    float newMinY = opMinY - iItemHeight;
    
    y = My::clip(y, newMinY, newMaxY);
    
    opComponent->setPositionY(y);
    
    ySpeed = ySpeed * SCROLL_DEACCEL_RATE;
    
    if (fabsf(ySpeed) <= SCROLL_DEACCEL_DIST) {
        this->unschedule(CC_SCHEDULE_SELECTOR(THIS::deaccelerateScrolling));
        this->relocateContainer(true);
    }
}

void DXSpinner::relocateContainer(bool animated) {
    
    if (!opComponent) {
        return;
    }
    
    float y = opComponent->getPositionY();
    y = My::clip(y, opMinY, opMaxY);
    
    y -= this->getContentSize().height/2;
    
    y = (ssize_t)(ceilf(y / iItemHeight)) * iItemHeight - iItemHeight/2;
    
    y += this->getContentSize().height/2;
    
    if (animated) {
        auto move = MoveTo::create(BOUNCE_DURATION, Vec2(opComponent->getPositionX(), y));
        auto action = EaseBackOut::create(move);
        opComponent->runAction(action);
    }
    else {
        opComponent->setPositionY(y);
    }
    
    ssize_t inComponent = components.getIndex(opComponent);
    
    if (vSelectedRows[inComponent] != opComponent->getSelectedRow()) {
        vSelectedRows[inComponent] = opComponent->getSelectedRow();
        if (pDelegate) {
            pDelegate->didSelectRowInComponent(this, vSelectedRows[inComponent], inComponent);
        }
    }
    
    opComponent = nullptr;
}

ssize_t DXSpinner::numberOfComponents() {
    return iComponents;
}

ssize_t DXSpinner::numberOfRowsInComponents(ssize_t inComponent) {
    if (inComponent < components.size()) {
        DXSpinnerComponent *component = components.at(inComponent);
        if (component) {
            return component->getNumOfRows();
        }
    }
    
    return 0;
}

void DXSpinner::reloadAllComponents() {
    if (pDataSource) {
       iComponents = pDataSource->numberOfComponents(this);
        if (iComponents < 1) {
            iComponents = 1;
        }
    }
    else {
        iComponents = 1;
    }
    
    iItemWidth = getContentSize().width/iComponents;
    
    for (ssize_t i = 0; i<iComponents; i++) {
        vSelectedRows.push_back(0);
        this->reloadComponent(i);
    }
}

void DXSpinner::reloadComponent(ssize_t inComponent) {
    
    vSelectedRows[inComponent] = 0;
    
    DXSpinnerComponent *componet = nullptr;
    if (inComponent < this->components.size()) {
        componet = this->components.at(inComponent);
    }
    
    ssize_t x = iItemWidth * inComponent;
    
    if (!componet) {
        componet = DXSpinnerComponent::component();
        componet->setItemHeight(iItemHeight);
        this->components.insert(inComponent, componet);
        if (pMask) {
            pMask->addChild(componet);
        }
        else {
            this->addChild(componet);
        }
        
    }
    
    componet->clear();
    
    if (pDelegate && pDataSource) {
        
        componet->setPosition(x, this->getContentSize().height/2 + iItemHeight/2);
        
        componet->setItemWidth(iItemWidth);
        
        ssize_t numRowsInComponent = pDataSource->numberOfRowsInComponent(this, inComponent);
        componet->setNumOfRows(numRowsInComponent);
        
        for (ssize_t i = 0; i<numRowsInComponent; i++) {
            const std::string &item = pDelegate->titleForRowInComponent(this, i, inComponent);
            componet->appendItem(item, i);
        }
        
        componet->updateUI();
    }
}

ssize_t DXSpinner::selectedRow(ssize_t inComponent) {
    if (vSelectedRows.size()>inComponent) {
        return vSelectedRows[inComponent];
    }
    else {
        return -1;
    }
}

void DXSpinner::setParent(Node * parent) {
    super::setParent(parent);
    this->reloadAllComponents();
}

void DXSpinner::selectRow(ssize_t row, ssize_t inComponent, bool animated) {
    if (inComponent < iComponents && inComponent<components.size()) {
        DXSpinnerComponent *component = this->components.at(inComponent);
        if (!!component) {
            ssize_t drow = My::clip(row, (ssize_t)0, component->getNumOfRows());
            float y = this->getContentSize().height/2;
            y += (iItemHeight * drow);
            y += + iItemHeight/2;
            
            component->setPositionY(y);
            component->updateUI();
            
            if (vSelectedRows[inComponent] != component->getSelectedRow()) {
                vSelectedRows[inComponent] = component->getSelectedRow();
                if (pDelegate) {
                    pDelegate->didSelectRowInComponent(this, vSelectedRows[inComponent], inComponent);
                }
            }
        }
    }
}
