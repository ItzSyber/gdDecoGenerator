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
inline std::string imgID = "";
inline bool GENERATING_DONE;

inline int sleep_limit = 0;

class sd2gdClient : public CCObject, public gd::FLAlertLayerProtocol {
protected:
    loadLayer* load = loadLayer::create();
    virtual bool init();
    void handleResponse(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response);
    void LoadImageFromResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
public:
    void request();
    void saveImageFromURL(const char* url);
    static sd2gdClient* create();
    void createReq(CCObject*);
};