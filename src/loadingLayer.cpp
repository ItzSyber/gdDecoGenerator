#include "loadingLayer.hpp"

loadLayer* loadLayer::create() {
    auto ret = new loadLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool loadLayer::init() {
    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    setZOrder(998);
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    
    this->registerWithTouchDispatcher();
    setTouchEnabled(0);
    setKeypadEnabled(0);

    auto l = CCLabelBMFont::create("Loading...", "bigFont.fnt");
    l->setPosition(winSize / 2);
    l->setZOrder(999);
    addChild(l);

    auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize(winSize);
    bg->setOpacity(0);

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    addChild(menu);

    menu->addChild(bg);
    addChild(bg);
    return true;
}

void loadLayer::keyBackClicked() {
    setTouchEnabled(1);
    setKeypadEnabled(1);
    gd::FLAlertLayer::keyBackClicked();
}