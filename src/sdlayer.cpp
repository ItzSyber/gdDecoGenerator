#include "sdlayer.hpp"

sd2gdLayer* sd2gdLayer::create() {
    auto ret = new sd2gdLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool sd2gdLayer::init() {

    if (!initWithColor({ 0, 0, 0, 105 })) return false;

    setZOrder(700);

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    addChild(menu);

    this->registerWithTouchDispatcher();
    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(1);

    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    auto bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
    const CCSize windowSize(winSize.width - 200, winSize.height - 100);
    bg->setContentSize(windowSize);
    bg->setPosition(winSize / 2);
    bg->setZOrder(710);
    addChild(bg);

    auto label = CCLabelBMFont::create("DecoGenerator", "goldFont.fnt");
    label->setPosition(winSize.width / 2, winSize.height - 68);
    label->setZOrder(720);
    addChild(label);

    auto btnSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    auto btn = gd::CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(sd2gdLayer::backButtonCallback));
    btn->setZOrder(720);
    btn->setPosition({101, winSize.height - 55});
    addChild(btn);
    menu->addChild(btn);

    btnSpr = gd::ButtonSprite::create("Generate", 0, false, "bigFont.fnt", "GJ_button_01.png", 0, 1);
    btn = gd::CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(sd2gdLayer::createBtnCallback));
    btn->setPosition(winSize.width / 2, 90);
    btn->setZOrder(720);
    label->setScale(0.95f);
    addChild(btn);
    menu->addChild(btn);

    //API Key 
    int yOffset = 65;
    auto txt = TextInputNode::create(300.0f, 40.0f, 1.0f, "chatFont.fnt");
    txt->setPosition(winSize.width / 2, winSize.height / 2);
    txt->setZOrder(720);
    txt->callback = [this](auto& txt) {
        inputKey = txt.get_value();
    };
    txt->input_node->setString(inputKey.c_str());
    addChild(txt);

    label = CCLabelBMFont::create("Enter your API Key:", "bigFont.fnt");
    label->setScale(0.75f);
    label->setPosition(winSize.width / 2 + 13, winSize.height / 2 + 40);
    label->setZOrder(720);
    addChild(label);

    btnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    btnSpr->setScale(0.80f);
    btn = gd::CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(sd2gdLayer::infoBtnCallback));
    btn->setPosition(winSize.width / 2 - 135 , winSize.height / 2 + 40);
    btn->setZOrder(720);
    addChild(btn);
    menu->addChild(btn);

    setTouchEnabled(true);
    setKeypadEnabled(true);

    return true;
}

void sd2gdLayer::keyBackClicked() {
    gd::FLAlertLayer::keyBackClicked();
}
void sd2gdLayer::backButtonCallback(CCObject*) {
    keyBackClicked();
}

void sd2gdLayer::createBtnCallback(CCObject*) {
    createBtnClicked();
}


void sd2gdLayer::createBtnClicked() {
    filePath = pfd::select_folder("Select a folder").result();
    if (!filePath.empty()) {
        sd2gdClient* img2file = sd2gdClient::create();
    }
}


void sd2gdLayer::switchToCustomLayerButton(CCObject*) {
    if(!PROCESSING_IMAGE) {
        auto node = create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(node);
    } else {
        PROCESSING_IMAGE = false;
    }
}

void sd2gdLayer::infoBtnCallback(CCObject* sender) {
    const char* msg = "1. Log in to <cy>stablediffusionapi.com</c>\n2. Go to your profile and copy the key in the <cg>API Settings</c> section";
    gd::FLAlertLayer::create(this, "How to get API Key", "Open URL", "Close", msg)->show();
}

void sd2gdLayer::FLAlert_Clicked(gd::FLAlertLayer*, bool btn2) {
    if(!btn2) {
        CCApplication::sharedApplication()->openURL("http://stablediffusionapi.com");
    }
}