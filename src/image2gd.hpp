#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <string_view>

#include <gd.h>
#include <cocos2d.h>
#include "Minhook.h"

#include "geometrize/shaperesult.h"
#include "geometrize/bitmap/bitmap.h"
#include "geometrize/shape/shapetypes.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shape/circle.h"

#include "portable-file-dialogs.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include <fmt/format.h>
#include <charconv>

using namespace cocos2d;
using namespace gd;

inline std::vector<geometrize::ShapeResult> SHAPE_DATA;
inline std::mutex SHAPE_LOCK;

inline bool PROCESSING_IMAGE = false;
inline int SHAPES_DONE = 0;
inline int DRAW_SCALE = 1;
inline int Z_LAYER = 0;
inline int TOTAL_SHAPES = 0;
inline int STEP = 0;

inline gd::ButtonSprite* BTN_SPR = nullptr;

#define LEVEL_EDITOR_LAYER__UPDATE 0x1632b0
constexpr int CIRCLE_ID = 1764;

inline std::string filePath;

void enableUpdateHook();
void disableUpdateHook();

void addImage();
void RGBtoHSV(int r, int g, int b, float& h, float& s, float& v);
void updateLabel(bool enabled = true);
bool isValidBitmap(const geometrize::Bitmap& bitmap);

int _stoi(const std::string_view s);
float _stof(const std::string_view s);

geometrize::Bitmap readImage(const std::string& filePath);

geometrize::ShapeTypes shapeTypesForNames(const std::string& str);

