#include "image2gd.hpp"

void enableUpdateHook()
{
	MH_EnableHook(reinterpret_cast<void*>(gd::base + LEVEL_EDITOR_LAYER__UPDATE));
}

void disableUpdateHook()
{
	MH_DisableHook(reinterpret_cast<void*>(gd::base + LEVEL_EDITOR_LAYER__UPDATE));
}

bool isValidBitmap(const geometrize::Bitmap& bitmap)
{
	return bitmap.getWidth() != 0 && bitmap.getHeight() != 0;
}

void RGBtoHSV(int r, int g, int b, float& h, float& s, float& v)
{
	auto _max = [](auto a, auto b) -> auto {
		return a > b ? a : b;
	};

	auto _min = [](auto a, auto b) -> auto {
		return a < b ? a : b;
	};

	// Normalize RGB values to the range of 0 to 1
	float r_normalized = r / 255.0;
	float g_normalized = g / 255.0;
	float b_normalized = b / 255.0;

	// Calculate value (maximum of RGB values)
	v = _max(_max(r_normalized, g_normalized), b_normalized);

	// Calculate saturation
	if (v == 0)
		s = 0;
	else
		s = (v - _min(_min(r_normalized, g_normalized), b_normalized)) / v;

	// Calculate hue
	if (s == 0)
		h = 0;
	else
	{
		float delta = v - _min(_min(r_normalized, g_normalized), b_normalized);
		if (v == r_normalized)
			h = 60 * (g_normalized - b_normalized) / delta;
		else if (v == g_normalized)
			h = 60 * (2 + (b_normalized - r_normalized) / delta);
		else if (v == b_normalized)
			h = 60 * (4 + (r_normalized - g_normalized) / delta);

		if (h < 0)
			h += 360;
	}
}

geometrize::Bitmap readImage(const std::string& filePath)
{
	const char* path{filePath.c_str()};
	int w = 0;
	int h = 0;
	stbi_set_flip_vertically_on_load(true);
	std::uint8_t* dataPtr{stbi_load(path, &w, &h, nullptr, 4)};
	if(dataPtr == nullptr) {
		return geometrize::Bitmap(0, 0, geometrize::rgba{0, 0, 0, 0});
	}
	const std::vector<std::uint8_t> data{dataPtr, dataPtr + (w * h * 4)};
	delete dataPtr;

	const geometrize::Bitmap bitmap(static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h), data);
	return bitmap;
}

geometrize::ShapeTypes shapeTypesForNames(const std::string& str)
{
	// Split string into words based on whitespace
	std::istringstream iss(str);
	const std::vector<std::string> shapeNames(std::istream_iterator<std::string>{iss},
									std::istream_iterator<std::string>());
	
	std::vector<geometrize::ShapeTypes> shapeTypes;
	
	// Convert the shape names into ShapeTypes
	for(const std::string& shapeName : shapeNames) {
		for(const std::pair<geometrize::ShapeTypes, std::string>& p : geometrize::shapeTypeNames) {
			if(p.second == shapeName) {
				shapeTypes.push_back(p.first);
			}
		}
	}
	
	if(shapeTypes.empty()) {
		std::cout << "Bad shape names provided, defaulting to ellipses \n";
		return geometrize::ELLIPSE;
	}
	
	// Combine shape types together
	std::underlying_type<geometrize::ShapeTypes>::type combinedShapeTypes = 0;
	for (const auto& shapeType : shapeTypes) {
		combinedShapeTypes |= shapeType;
	}
	return geometrize::ShapeTypes(combinedShapeTypes);
}

int _stoi(const std::string_view s) {
	int ret = -1;
	std::from_chars(s.data(),s.data() + s.size(), ret);
	return ret;
}

float _stof(const std::string_view s) {
	float ret = -1.0f;
	std::from_chars(s.data(),s.data() + s.size(), ret);
	return ret;
}

void updateLabel(bool enabled)
{
	gd::EditorUI* ui = gd::EditorUI::get();
	if(!ui) return;
	
	cocos2d::CCLabelBMFont* label = reinterpret_cast<cocos2d::CCLabelBMFont*>(ui->getChildByTag(33));

	if(!label)
	{
		auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize() / 2;
		label = cocos2d::CCLabelBMFont::create("", "bigFont.fnt");
		label->setPosition({winSize.width, winSize.height - 50.0f});
		label->setTag(33);
		ui->addChild(label);
	}
	
	if(enabled)
	{
		float percentage;
		if(STEP >= TOTAL_SHAPES) {
			percentage = 100.0f;
		}
		else {
			percentage = ((static_cast<float>(STEP) / static_cast<float>(TOTAL_SHAPES)) * 100);
		}

		std::string labelstr = fmt::format("{}/{} | {:.2}%", STEP, TOTAL_SHAPES, percentage);
		label->setString(labelstr.c_str());
		label->setVisible(true);
	}
	else
	{
		label->setVisible(false);
	}
}

void addImage()
{
	SHAPE_LOCK.lock();
	SHAPE_DATA.clear();
	SHAPE_LOCK.unlock();
	
	//const char* inputImagePath = getImagePathDialog();
	const char* inputImagePath = filePath.c_str();
	const geometrize::Bitmap bitmap = readImage(inputImagePath);

	TOTAL_SHAPES = 5000;
	
	int stepShapeCount = 0;
	stepShapeCount = 300;
	
	DRAW_SCALE = 1;
	
	Z_LAYER = 0;
	
	// the options for geometrizing the image
	geometrize::ImageRunnerOptions options;
	options.shapeCount = stepShapeCount;
	options.alpha = 255;
	options.shapeTypes = shapeTypesForNames("circle");
	
	geometrize::ImageRunner runner(bitmap);
	STEP = 0;
	updateLabel(true);
	
	PROCESSING_IMAGE = true;
	
	int total = TOTAL_SHAPES + 2;

	for(STEP = 1; STEP < total; STEP++)
	{
		if(!PROCESSING_IMAGE || !gd::LevelEditorLayer::get()) {
			if(BTN_SPR) {
				BTN_SPR->setString("Import Image");
			}
			break;
		}
		
		const std::vector<geometrize::ShapeResult> shapes{runner.step(options)};
		for(const auto& result : shapes)
		{
			geometrize::rgba color = result.color;
			
			//switch(result.shape->getType())
			//{
			//	case geometrize::ShapeTypes::CIRCLE:
			//	{
			//		logShapeResult(STEP, result);
			//	}
			//}
			SHAPE_LOCK.lock();
			std::copy(shapes.begin(), shapes.end(), std::back_inserter(SHAPE_DATA));
			SHAPE_LOCK.unlock();
		}
	}
	updateLabel(false);
	PROCESSING_IMAGE = false;
}