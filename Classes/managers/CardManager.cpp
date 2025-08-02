#include "managers/CardManager.h"
#include <iostream>
#include "views/CardView.h"  
#include "services/CardIdManagerMap.h"
#include "cocos2d.h"

CardManager::CardManager(const CardModel& model)
    : _model(model), _view(nullptr), _isSelected(false) {
    CardIdManagerMap::getInstance().addCardManager(model._id, this);
    CCLOG(u8"创建CardManager - 卡牌ID：%d", model._id);
}

CardManager::~CardManager() {
    // 从全局映射中移除当前管理器
    CardIdManagerMap::getInstance().removeCardManager(_model._id);
    CCLOG(u8"销毁CardManager - 卡牌ID：%d", _model._id);
}

void CardManager::setCard(const CardModel& model, CardView* view) {
    // 移除旧ID的映射
    if (_model._id != model._id) {
        CardIdManagerMap::getInstance().removeCardManager(_model._id);
    }

    _model = model;
    _view = view;
    CardIdManagerMap::getInstance().addCardManager(model._id, this);
    setupTouchEvents();
    CCLOG(u8"更新卡牌信息 - ID：%d，区域：%d", model._id, static_cast<int>(model.getZone()));
}

void CardManager::setupTouchEvents() {
    if (!_view) {
        CCLOG(u8"警告：CardView为空，无法设置触摸事件");
        return;
    }

    // 移除已存在的触摸监听，防止重复注册
    auto dispatcher = _view->getEventDispatcher();
    if (dispatcher->getEventListenerForTarget(_view, -1)) {
        dispatcher->removeEventListenersForTarget(_view);
    }

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(CardManager::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(CardManager::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(CardManager::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(CardManager::onTouchCancelled, this);

    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, _view);
    CCLOG(u8"为卡牌ID：%d 设置触摸事件", _model._id);
}

bool CardManager::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_view) return false;

    cocos2d::Vec2 touchPos = _view->convertToNodeSpace(touch->getLocation());
    if (!_view->isTouchInside(touchPos)) {
        CCLOG(u8"触摸不在卡牌ID：%d 范围内", _model._id);
        return false;
    }

    CCLOG(u8"开始触摸卡牌放大 - ID：%d", _model._id);
    _view->setScale(1.1f);
    _isSelected = true;
    return true;
}

void CardManager::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_view || !_isSelected) return;

    // 实现拖拽逻辑：根据触摸偏移移动
    cocos2d::Vec2 delta = touch->getDelta();
    _view->setPosition(_view->getPosition() + delta);

    // 同步更新模型位置
    _model.setPosition(_view->getPosition() - _view->getParent()->getPosition());
    CCLOG(u8"卡牌ID：%d 移动中，新位置：(%.0f, %.0f)",
        _model._id, _model.getPosition().x, _model.getPosition().y);
}

void CardManager::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_view) return;

    _view->setScale(1.0f);
    CCLOG(u8"触摸结束，卡牌恢复原大小 - ID：%d", _model._id);

    cocos2d::Vec2 touchPos = _view->convertToNodeSpace(touch->getLocation());
    if (_view->isTouchInside(touchPos) && _cardClickedCallback) {
        CCLOG(u8"触发点击回调 - 卡牌ID：%d", _model._id);
        _cardClickedCallback(_model);
    }

    _isSelected = false;
}

void CardManager::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_view) return;

    _view->setScale(1.0f);
    _isSelected = false;
    CCLOG(u8"触摸取消 - 卡牌ID：%d 恢复原大小", _model._id);
}

void CardManager::setCardClickedCallback(const std::function<void(CardModel&)>& callback) {
    _cardClickedCallback = callback;
    CCLOG(u8"设置卡牌ID：%d 的点击回调", _model._id);
}