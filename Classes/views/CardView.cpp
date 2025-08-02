#include "CardView.h"
#include <iostream>

CardView* CardView::create(const CardModel& model, const Vec2& offset) {
    auto view = new (std::nothrow) CardView();
    if (view && view->init(model, offset)) {
        view->autorelease(); // 启用内存自动释放机制
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

void CardView::setClickCallback(const ClickCallback& callback) {
    _clickCallback = callback;
    if (_cardManager) {
        // 绑定卡牌点击回调，将视图自身作为参数传递
        _cardManager->setCardClickedCallback([this, callback](CardModel& model) {
            if (callback) {
                callback(this);
            }
        });
    }
}

bool CardView::isTouchInside(const Vec2& touchPos) {
    if (!_background) return false;
    // 检查触摸点是否在背景图范围内
    return _background->getBoundingBox().containsPoint(touchPos);
}

bool CardView::init(const CardModel& model, const Vec2& offset) {
    if (!Node::init()) {
        return false;
    }

    // 1. 加载卡牌背景并设置尺寸
    loadBackground();
    if (!_background) {
        CCLOG("CardView: 背景图加载失败");
        return false;
    }

    // 2. 创建并初始化卡牌管理器
    _cardManager = new (std::nothrow) CardManager(model);
    if (!_cardManager) {
        CCLOG("CardView: 创建CardManager失败");
        return false;
    }

    // 3. 加载卡牌元素（数字、花色图标）
    loadSmallNumber(model);
    loadBigNumber(model);
    loadSuitIcon(model);

    // 4. 设置卡牌在场景中的位置（模型位置+偏移量）
    this->setPosition(model.getPosition() + offset);

    // 5. 将模型与视图关联到管理器
    _cardManager->setCard(model, this);

    return true;
}

void CardView::loadBackground() {
    // 从配置获取背景图资源并创建精灵
    _background = Sprite::create(CardResConfig::getBackGround());
    if (_background) {
        _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE); // 锚点设为中心
        this->addChild(_background);
        // 使CardView尺寸与背景图保持一致
        this->setContentSize(_background->getContentSize());
    }
    else {
        CCLOG("CardView: 背景资源获取失败 - %s", CardResConfig::getBackGround().c_str());
    }
}

void CardView::loadSmallNumber(const CardModel& model) {
    // 获取角落小数字资源路径
    const auto suit = model.getSuit();
    const auto face = model.getFace();
    const std::string res = CardResConfig::getSmallNumberRes(suit, face);

    // 创建小数字精灵并设置位置
    _smallNumber = Sprite::create(res);
    if (_smallNumber) {
        _smallNumber->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT); // 左上角锚点
        _smallNumber->setPosition(_smallNumberPos);          // 使用预设位置
        this->addChild(_smallNumber);
    }
    else {
        CCLOG("CardView: 小数字资源缺失: %s", res.c_str());
    }
}

void CardView::loadBigNumber(const CardModel& model) {
    // 获取中间大数字资源路径
    const auto suit = model.getSuit();
    const auto face = model.getFace();
    const std::string res = CardResConfig::getBigNumberRes(suit, face);

    // 创建大数字精灵并设置位置
    _bigNumber = Sprite::create(res);
    if (_bigNumber) {
        _bigNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE); // 中心锚点
        _bigNumber->setPosition(_bigNumberPos);          // 使用预设位置
        this->addChild(_bigNumber);
    }
    else {
        CCLOG("CardView: 大数字资源缺失: %s", res.c_str());
    }
}

void CardView::loadSuitIcon(const CardModel& model) {
    // 获取花色图标资源路径
    const auto suit = model.getSuit();
    const std::string res = CardResConfig::getSuitRes(suit);

    // 创建花色图标精灵并设置位置
    _suitIcon = Sprite::create(res);
    if (_suitIcon) {
        _suitIcon->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT); // 右上角锚点
        _suitIcon->setPosition(_suitIconPos);              // 使用预设位置
        this->addChild(_suitIcon);
    }
    else {
        CCLOG("CardView: 花色图标资源缺失: %s", res.c_str());
    }
}

CardView::~CardView() {
    CC_SAFE_DELETE(_cardManager); // 释放卡牌管理器内存
}