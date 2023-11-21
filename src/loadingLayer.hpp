#pragma once

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;

class loadLayer : public gd::FLAlertLayer {
private:
    virtual bool init();
public:
    static loadLayer* create();
    void keyBackClicked();
};