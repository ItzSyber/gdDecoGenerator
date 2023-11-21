#include "sdreq.hpp"

sd2gdClient* sd2gdClient::create() {
    auto ret = new sd2gdClient();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool sd2gdClient::init() {
    GENERATING_DONE = false;
    load->setZOrder(999);
    CCDirector::sharedDirector()->getRunningScene()->addChild(load);
    sd2gdClient::request();
    return true;
}

void sd2gdClient::FLAlert_Clicked(gd::FLAlertLayer*, bool btn2) {
    if(!btn2) {
        FLAlertLayer::create(nullptr, "Test", "OK", nullptr, "sd2gdClient reazlization")->show();
    }
}

void sd2gdClient::request() {
    char const* data = "{\"key\":\"KEY\",\"model_id\":\"dreamshaper-v7\",\"prompt\":\"a screenshot of a ((varied level)) from a (((2d))) platformer game, ((front view)), geometry dash, (buildings), (obstacles), matching colors, hyperdetailed\",\"negative_prompt\":\"((3d)), ((nature)), ((pixel style)), ugly, tiling, poorly drawn hands, poorly drawn feet, poorly drawn face, out of frame, extra limbs, disfigured, deformed, body out of frame, bad anatomy, watermark, signature, cut off, low contrast, underexposed, overexposed, bad art, beginner, amateur, distorted face\",\"width\":\"768\",\"height\":\"512\",\"samples\":\"1\",\"num_inference_steps\":\"30\",\"safety_checker\":\"yes\",\"enhance_prompt\":\"no\",\"seed\":null,\"guidance_scale\":7.5,\"multi_lingual\":\"no\",\"panorama\":\"no\",\"self_attention\":\"no\",\"upscale\":\"no\",\"embeddings_model\":null,\"lora_model\":null,\"tomesd\":\"yes\",\"clip_skip\":\"2\",\"use_karras_sigmas\":\"yes\",\"vae\":null,\"lora_strength\":null,\"scheduler\":\"DPMSolverSinglestepScheduler\",\"webhook\":null,\"track_id\":null}";
    
    rapidjson::Document docData;
    docData.Parse(data);

    docData["key"] = Value(inputKey.c_str(), docData.GetAllocator());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    docData.Accept(writer);

    char const* newData = buffer.GetString();
    CCString* dataToSend =  CCString::create(newData);

    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");

    cocos2d::extension::CCHttpRequest * request = new cocos2d::extension::CCHttpRequest();
    request->setUrl("https://stablediffusionapi.com/api/v4/dreambooth");

    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(sd2gdClient::handleResponseGen));
    request->setRequestData(dataToSend->getCString(), dataToSend->length());
    request->setHeaders(headers);
    request->setTag("POST Request");

    cocos2d::extension::CCHttpClient::getInstance()->send(request);

    request->release();
}

void sd2gdClient::handleResponseGen(cocos2d::extension::CCHttpClient* sender, cocos2d::extension::CCHttpResponse* response) {
    if (!response)
    {
        return;
    }

    if (!response->isSucceed())
    {
        auto flalert = gd::FLAlertLayer::create(nullptr, "Error", "OK", nullptr, "Response is not succeed.\nCheck your internet connection.");
        flalert->show();
        load->keyBackClicked();
        return;
    }

    std::vector<char>* buffer = response->getResponseData();
    std::string str_response(buffer->begin(), buffer->end());

    rapidjson::Document jsonResponse;
    jsonResponse.Parse(str_response.c_str());

    const Value& status = jsonResponse["status"];
    const Value& message = jsonResponse["message"];
    bool errEnabled = false;

    if (status.GetString() == "error" || status.GetString() == "failed") {
        const char* msg = message.GetString();
        
        auto flalert = gd::FLAlertLayer::create(nullptr, "SD API Error", "OK", nullptr, msg);
        flalert->show();
        errEnabled = true;
        load->keyBackClicked();
    }

    if (!errEnabled) {
        const Value& outp = jsonResponse["output"];
        if (outp.IsArray()) {
            const char* url = outp[0].GetString();
            sd2gdClient::saveImageFromURL(url);
        }
    }

}

void sd2gdClient::saveImageFromURL(const char* url) {
    cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
    request->setUrl(url);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(sd2gdClient::handleResponseLoad));

    cocos2d::extension::CCHttpClient::getInstance()->send(request);
    request->release();
}

void sd2gdClient::handleResponseLoad(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response) {
    if (!response) {
        return;
    }

    if (!response->isSucceed()) {
        std::string err = fmt::format("Something went wrong...\nDetails: {}", response->getErrorBuffer()); 
        auto flalert = gd::FLAlertLayer::create(nullptr, "HTTP Error", "OK", nullptr, err.c_str());
        flalert->show();
        load->keyBackClicked();
        return;
    }

    std::vector<char>* buffer = response->getResponseData();
    std::string responseData(buffer->begin(), buffer->end());

    filePath = filePath + "/GeneratedDeco.jpg";
    std::ofstream file(filePath.c_str(), std::ios::binary);
    file.write(responseData.c_str(), responseData.size());
    file.close();
    std::string warn = "Your image is saved as:\n" + filePath + "\nStarting to import the image to the editor...";
    FLAlertLayer::create(nullptr, "Saving Done", "OK", nullptr, warn.c_str())->show();
    load->keyBackClicked();

updateLabel(false);
    if(!PROCESSING_IMAGE) {
        enableUpdateHook();
        std::thread(addImage).detach();
    }
    else {
        PROCESSING_IMAGE = false;
        disableUpdateHook();
    }
}

void sd2gdClient::createReq(CCObject*) {
    if(!PROCESSING_IMAGE){
        auto node = sd2gdClient::create();
    }
    else {
        PROCESSING_IMAGE = false;
    }
}