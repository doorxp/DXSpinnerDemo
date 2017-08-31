//
//  DXSpinnerComponent.cpp
//  Shoji++
//
//  Created by doorxp on 2017/8/30.
//

#include "DXSpinnerComponent.hpp"
#include "DXSpinner.hpp"
#include "../Utls/MyMath.hpp"

using namespace cocos2d;
DXSpinnerComponent *DXSpinnerComponent::component() {
    auto pRet = new DXSpinnerComponent();
    if (!pRet || !pRet->init()) {
        if (pRet) {
            delete pRet;
            pRet = nullptr;
        }
    }
    else {
        pRet->autorelease();
    }
    
    return pRet;
}

bool DXSpinnerComponent::init() {
    if(!super::init()) {
        return false;
    }
    
//    this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    this->setBackGroundColor(cocos2d::Color3B::BLUE);
    
    iItemHeight = 40;
    iSelectedRow = 0;
    this->setAnchorPoint(cocos2d::Vec2(0.0, 1.0));
    
    return true;
}

void DXSpinnerComponent::clear() {
    this->removeAllChildrenWithCleanup(true);
}

void DXSpinnerComponent::appendItem(const std::string & item, ssize_t idx) {
    cocos2d::ui::Text *tx = cocos2d::ui::Text::create(item, "", 40);
    tx->setTextColor(cocos2d::Color4B::BLACK);
    this->addChild(tx);
    tx->setPosition(cocos2d::Vec2(iItemWidth/2, this->getContentSize().height - (idx + 0.5) * iItemHeight));
    
}

void DXSpinnerComponent::setNumOfRows(ssize_t var) {
    if (iNumOfRows == var) {
        return;
    }
    
    iNumOfRows = var;
    
    setContentSize(cocos2d::Size(iItemWidth, iNumOfRows * iItemHeight));
}

ssize_t DXSpinnerComponent::getNumOfRows() const {
    return iNumOfRows;
}

bool DXSpinnerComponent::pointIn(const cocos2d::Vec2 & point) {
    Vec2 pnt = this->convertToNodeSpace(point);
    Rect r;
    r.origin = Vec2::ZERO;
    r.size = this->getContentSize();
    return pnt.x >= r.getMinX() && pnt.x <r.getMaxX();
}

void DXSpinnerComponent::updateUI() {
    
    float y = this->getPositionY();
    
    Node *parent = this->getParent();
    DXSpinner *spinner = dynamic_cast<DXSpinner *>(parent);
    while (!spinner && !!parent) {
        parent = parent->getParent();
        spinner = dynamic_cast<DXSpinner *>(parent);
    }
    
    if (spinner) {
        
        float h = spinner->getContentSize().height;
        
        y = y - h/2;
        
        iSelectedRow = y / iItemHeight;
        iSelectedRow = My::clip(iSelectedRow, (ssize_t)0, getNumOfRows()-1);
        
        
        for (int i = 0; i<getChildren().size(); i++) {
            ui::Text *tx =
            dynamic_cast<ui::Text *>(this->getChildren().at(i));
            float iy = this->getContentSize().height - tx->getPositionY();
            float r = (iy-y)/h;
            
            float scale = My::clip(cos(r * 2), 0.0f, 1.0f);

            Color4B color = tx->getTextColor();
            
            color.a = 255 * scale * 0.5;
            
            tx->setTextColor(color);
            
            tx->setScale(scale);
        }
        
        if (this->getChildren().size() > iSelectedRow && iSelectedRow != -1) {
            ui::Text *tx =
            dynamic_cast<ui::Text *>(this->getChildren().at(iSelectedRow));
            if (tx) {
                Color4B color = tx->getTextColor();
                color.a = 255;
                tx->setTextColor(color);
                tx->setScale(1.0);
            }
        }
    }
}

void DXSpinnerComponent::setPosition(float x, float y) {
    super::Node::setPosition(x, y);
    this->updateUI();
    
}
