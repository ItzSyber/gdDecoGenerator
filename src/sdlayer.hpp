#pragma once

#include <string>
#include <iostream>

#include <cocos2d.h>
#include <gd.h>

#include "sdreq.hpp"
#include "nodes.hpp"
#include "portable-file-dialogs.h"

using namespace cocos2d;

class sd2gdLayer : public gd::FLAlertLayer, public gd::FLAlertLayerProtocol {
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void backButtonCallback(CCObject*);

    virtual void createBtnClicked();
    void createBtnCallback(CCObject*);

    void infoBtnCallback(CCObject* sender);

    virtual void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2);
public:
    static sd2gdLayer* create();
    void switchToCustomLayerButton(CCObject*);
};

