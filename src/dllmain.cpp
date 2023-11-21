
#include <Windows.h>
#include <cocos2d.h>
#include <MinHook.h>
#include <gd.h>

#define WIN32_LEAN_AND_MEAN

using namespace cocos2d;

#include <fmt/format.h>

#include "geometrize/shaperesult.h"
#include "geometrize/bitmap/bitmap.h"
#include "geometrize/shape/shapetypes.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shape/circle.h"

#include "sdreq.hpp"
#include "sdlayer.hpp"

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET



void addCircle(LevelEditorLayer* self, const geometrize::ShapeResult& result)
{
	
	geometrize::Circle* circle = dynamic_cast<geometrize::Circle*>(result.shape.get());
	
	geometrize::rgba color = result.color;
	
	float h,s,v;
	RGBtoHSV(color.r, color.g, color.b, h, s, v);

	std::string hsv_string = fmt::format("{:.2}a{:.2}a{:.2}a1a1", h, s, v);
	
	const float x = circle->m_x * DRAW_SCALE;
	const float y = circle->m_y * DRAW_SCALE;
	const float scale = circle->m_r * DRAW_SCALE / 4;
	
	std::string str = fmt::format(
		"1,{},2,{},3,{},32,{},41,1,42,1,43,{},44,{},25,{},21,{},20,{}",
		CIRCLE_ID, x, y, scale, hsv_string, hsv_string, STEP, 1010, Z_LAYER
	);

	self->addObjectFromString(str);
	updateLabel(STEP != (TOTAL_SHAPES + 1));

}

void addShape(LevelEditorLayer* self, const geometrize::ShapeResult& result)
{
	switch(result.shape->getType())
	{
		case geometrize::ShapeTypes::CIRCLE: return addCircle(self, result);
	}
}

void (__thiscall* LevelEditorLayer_updateO)(void* self, float dt);
void __fastcall LevelEditorLayer_updateH(LevelEditorLayer* self, void* edx, float dt)
{
	if(PROCESSING_IMAGE)
	{
		SHAPE_LOCK.lock();
		if(SHAPE_DATA.size() != 0)
		{
			for(geometrize::ShapeResult result : SHAPE_DATA)
			{
				addShape(self, result);
			}
			SHAPE_DATA.clear();
		}
		SHAPE_LOCK.unlock();
	}
	LevelEditorLayer_updateO(self, dt);
}

bool (__thiscall* LevelSettingsLayer_init)(CCLayer* self, void* settings, void* editor);
bool __fastcall LevelSettingsLayer_init_H(CCLayer* self, void* edx, void* settings, void* editor) {
   if (!LevelSettingsLayer_init(self, settings, editor)) return false;

	bool startPosSettings = MBO(bool, settings, 272);
	if(startPosSettings) return true;

    auto win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();

    auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
    auto menu = (CCMenu*)layer->getChildren()->objectAtIndex(1);
    
    auto btnSpr = gd::ButtonSprite::create(PROCESSING_IMAGE ? "Interrupt" : "Generate", 0, false, "bigFont.fnt", "GJ_button_01.png", 0, 1);
    btnSpr->setScale(0.65f);
    auto btn = gd::CCMenuItemSpriteExtra::create(btnSpr, self, menu_selector(sd2gdLayer::switchToCustomLayerButton));
    btn->setPosition(menu->convertToNodeSpace({380.0f, 30.0f}));

    menu->addChild(btn);
    return true;
}

DWORD WINAPI thread_func(void* hModule) {
    MH_Initialize();
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x170E50),
        reinterpret_cast<void*>(&LevelSettingsLayer_init_H),
        reinterpret_cast<void**>(&LevelSettingsLayer_init)
    );

	MH_EnableHook(MH_ALL_HOOKS);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1632b0),
		reinterpret_cast<void*>(&LevelEditorLayer_updateH),
		reinterpret_cast<void**>(&LevelEditorLayer_updateO)
	);
	
    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}

