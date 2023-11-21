#include "nodes.hpp"

bool TextInputNode::init(float x, float y, float scale, char const* font) {
    if (!CCNode::init()) return false;

    input_node = gd::CCTextInputNode::create("", this, font, x, y);
    input_node->setDelegate(this);
    input_node->setAllowedChars(R"(1234567890qwertyuiop()asdfghjkl:zxcvbnm,. QWERTYUIOPASDFGHJKLZXCVBNM)");
    addChild(input_node);

    background = extension::CCScale9Sprite::create("square02_small.png");
    background->setContentSize(CCSize(x, y));
    background->setScale(1.f / scale);
    background->setOpacity(100);
    background->setZOrder(-1);
    addChild(background);

    return true;
}

void TextInputNode::textChanged(gd::CCTextInputNode*) {
    callback(*this);
}

void TextInputNode::set_value(char const* value) {
    input_node->setString(value);
}

std::string TextInputNode::get_value() {
    return input_node->getString();
}