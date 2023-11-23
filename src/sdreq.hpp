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

//char const* data = R"({\"key\":\"KEY\",\"model_id\":\"dreamshaper-v7\",\"prompt\":\"a screenshot of a ((varied level)) from a (((2d))) platformer game, ((front view)), geometry dash, (buildings), (obstacles), matching colors, hyperdetailed\",\"negative_prompt\":\"((3d)), ((nature)), ((pixel style)), ugly, tiling, poorly drawn hands, poorly drawn feet, poorly drawn face, out of frame, extra limbs, disfigured, deformed, body out of frame, bad anatomy, watermark, signature, cut off, low contrast, underexposed, overexposed, bad art, beginner, amateur, distorted face\",\"width\":\"768\",\"height\":\"512\",\"samples\":\"1\",\"num_inference_steps\":\"30\",\"safety_checker\":\"yes\",\"enhance_prompt\":\"no\",\"seed\":null,\"guidance_scale\":7.5,\"multi_lingual\":\"no\",\"panorama\":\"no\",\"self_attention\":\"no\",\"upscale\":\"no\",\"embeddings_model\":null,\"lora_model\":null,\"tomesd\":\"yes\",\"clip_skip\":\"2\",\"use_karras_sigmas\":\"yes\",\"vae\":null,\"lora_strength\":null,\"scheduler\":\"DPMSolverMultistepScheduler\",\"webhook\":null,\"track_id\":null})";



class sd2gdClient : public CCObject, public gd::FLAlertLayerProtocol {
protected:
    loadLayer* load = loadLayer::create();
    virtual bool init();
    void handleResponse(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response);
    void LoadImageFromResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    virtual void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2);
public:
    void request();
    void saveImageFromURL(const char* url);
    static sd2gdClient* create();
    void createReq(CCObject*);
};