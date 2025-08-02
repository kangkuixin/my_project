#include "controllers/GameController.h"
#include <iostream>
#include "services/CardIdManagerMap.h"
#include "cocos2d.h"

GameController::GameController(GameModel gameModel)
    : _gameModel(gameModel), _undoManager(gameModel.getUndoModel()) {
    CCLOG(u8"初始化时undoManager大小为：%d", _undoManager.getUndoSize());
}

GameController::~GameController() {}

bool GameController::selectCardFromPlayefieldAndMatch(CardModel& selectedCard) {
    if (_undoManager.getUndoSize() == 0) {
        CCLOG(u8"撤销栈为空，无法进行匹配操作");
        return false;
    }

    CardModel bottomCard = getBottomCard();
    CCLOG(u8"Playfield区选中 - 选择卡牌ID：%d，匹配底部卡牌ID：%d",
        selectedCard._id, bottomCard._id);

    if (isCardMatch(selectedCard, bottomCard)) {
        // 记录当前状态用于撤销
        UndoCardState state;
        state.id = selectedCard._id;
        state.position = selectedCard.getPosition();
        state.zone = selectedCard.getZone();
        _undoManager.recordUndoState(state);
        CCLOG(u8"卡牌匹配成功，已记录撤销状态 - ID：%d", selectedCard._id);

        handleCardClicked(selectedCard);
        return true;
    }

    CCLOG(u8"卡牌匹配失败 - 数值不相邻");
    return false;
}

void GameController::clickStackCard(CardModel& card) {
    // 记录当前状态用于撤销
    UndoCardState state;
    state.id = card._id;
    state.position = card.getPosition();
    state.zone = card.getZone();
    _undoManager.recordUndoState(state);
    CCLOG(u8"Stack区选中 - 已记录撤销状态 - ID：%d", card._id);

    handleCardClicked(card);
}

bool GameController::undo() {
    UndoCardState state;
    if (_undoManager.undo(state)) {
        CCLOG(u8"执行撤销操作 - 卡牌ID：%d，目标区域：%d",
            state.id, static_cast<int>(state.zone));
        moveCardToOriginalPosition(state);
        return true;
    }

    CCLOG(u8"撤销栈为空，无法执行撤销操作");
    return false;
}

// 专门处理Stack区域的卡牌撤销
bool GameController::undoStack() {
    UndoCardState state;
    if (_undoManager.undoStack(state)) {
        CCLOG(u8"执行Stack区撤销 - 卡牌ID：%d", state.id);
        moveCardToOriginalPosition(state);
        return true;
    }

    CCLOG(u8"没有可撤销的Stack区操作");
    return false;
}

CardModel GameController::getBottomCard() {
    UndoCardState state;
    int currentSize = _undoManager.getUndoSize();
    CCLOG(u8"获取栈底卡牌 - 当前撤销栈大小：%d", currentSize);

    if (currentSize == 0) {
        CCLOG(u8"撤销栈为空，返回默认卡牌");
        return CardModel(CardFaceType::CFT_ACE, CardSuitType::CST_SPADES, cocos2d::Vec2::ZERO);
    }

    if (_undoManager.getLastState(state)) {
        // 查找对应的卡牌模型
        const auto& playfield = _gameModel.getPlayfield();
        for (const auto& card : playfield) {
            if (card._id == state.id) {
                return card;
            }
        }

        const auto& stackfield = _gameModel.getStackfield();
        for (const auto& card : stackfield) {
            if (card._id == state.id) {
                return card;
            }
        }
    }

    CCLOG(u8"未找到栈底卡牌，返回默认卡牌");
    return CardModel(CardFaceType::CFT_ACE, CardSuitType::CST_SPADES, cocos2d::Vec2::ZERO);
}

bool GameController::isCardMatch(const CardModel& card1, const CardModel& card2) {
    int face1 = static_cast<int>(card1.getFace());
    int face2 = static_cast<int>(card2.getFace());
    bool result = (face1 == face2 + 1) || (face1 == face2 - 1);
    CCLOG(u8"卡牌匹配检查 - 卡牌1数值：%d，卡牌2数值：%d，结果：%s",
        face1, face2, result ? u8"匹配" : u8"不匹配");
    return result;
}

void GameController::moveCardToOriginalPosition(const UndoCardState& state) {
    // 先在Playfield中查找并移动卡牌
    auto& playfield = _gameModel.getPlayfield();
    for (auto& card : playfield) {  // 使用引用修改实际卡牌
        if (card._id == state.id) {
            CardManager* cardManager = getCardManager(card);
            if (cardManager) {
                card.setPosition(state.position);
                card.setZone(state.zone);
                cardManager->getView()->runAction(cocos2d::MoveTo::create(0.5f, state.position));
                cardManager->getView()->setLocalZOrder(0);
                CCLOG(u8"卡牌移回原位置 - ID：%d，区域：Playfield", state.id);
            }
            return;
        }
    }

    // 再在Stackfield中查找并移动卡牌
    auto& stackfield = _gameModel.getStackfield();
    for (auto& card : stackfield) {  // 使用引用修改实际卡牌
        if (card._id == state.id) {
            CardManager* cardManager = getCardManager(card);
            if (cardManager) {
                card.setPosition(state.position);
                card.setZone(state.zone);
                cardManager->getView()->runAction(cocos2d::MoveTo::create(0.5f, state.position));
                cardManager->getView()->setLocalZOrder(0);
                CCLOG(u8"卡牌移回原位置 - ID：%d，区域：Stackfield", state.id);
            }
            return;
        }
    }

    CCLOG(u8"未找到卡牌 - ID：%d，无法执行移动操作", state.id);
}

CardManager* GameController::getCardManager(const CardModel& card) {
    CardManager* manager = CardIdManagerMap::getInstance().getCardManager(card._id);
    if (!manager) {
        CCLOG(u8"警告：未找到卡牌管理器 - ID：%d", card._id);
    }
    return manager;
}

void GameController::handleCardClicked(CardModel& card) {
    CCLOG(u8"卡牌被点击 - ID：%d，当前区域：%d",
        card._id, static_cast<int>(card.getZone()));

    if (card.getZone() != CardZone::Hand) {
        cocos2d::Vec2 newPos(700, 400);
        CardManager* cardManager = getCardManager(card);

        if (cardManager) {
            // 更新卡牌状态
            card.setZone(CardZone::Hand);
            card.setPosition(newPos);

            // 执行移动动画
            auto moveTo = cocos2d::MoveTo::create(0.5f, newPos);
            cardManager->getView()->runAction(moveTo);

            // 设置Z轴顺序确保正确显示层级
            if (_undoManager.getUndoSize() > 1) {  // 大于1是因为包含了当前操作的记录
                CardModel lastCard = getBottomCard();
                CardManager* lastCardManager = getCardManager(lastCard);
                if (lastCardManager) {
                    int newZOrder = lastCardManager->getView()->getLocalZOrder() + 1;
                    cardManager->getView()->setLocalZOrder(newZOrder);
                    CCLOG(u8"更新卡牌Z轴顺序 - ID：%d，ZOrder：%d", card._id, newZOrder);
                }
            }

            CCLOG(u8"卡牌移动到Hand区域 - ID：%d，新位置：(%.0f, %.0f)",
                card._id, newPos.x, newPos.y);
        }
    }
}

void GameController::handleLabelClick() {
    CCLOG(u8"标签被点击事件 - 执行撤销操作");
    undo();
}