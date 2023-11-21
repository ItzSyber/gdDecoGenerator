#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <cocos2d.h>
#include <gd.h>

#include <fmt/format.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "image2gd.hpp"
#include "loadingLayer.hpp"

using namespace rapidjson;
using namespace cocos2d;
using namespace gd;

inline std::string inputKey = "";

inline bool GENERATING_DONE;

class sd2gdClient : public CCObject, public gd::FLAlertLayerProtocol {
protected:
    loadLayer* load = loadLayer::create();
    virtual bool init();
    void handleResponseGen(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response);
    void handleResponseLoad(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    virtual void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2);
public:
    void request();
    void saveImageFromURL(const char* url);
    static sd2gdClient* create();
    void createReq(CCObject*);
};