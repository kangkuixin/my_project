// GameController.h
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "managers/CardManager.h"
#include <vector>

class CardManager;
/*
用于衔接GameModel与GameView/CardView的控制器类
核心职责：
1. 处理卡牌选择与匹配逻辑（通过selectCardFromPlayefieldAndMatch验证匹配规则）
2. 管理Stack区域卡牌点击事件并通过UndoManager记录操作状态
3. 实现撤销功能：基于UndoModel恢复卡牌位置和状态
4. 作为视图层与数据层的桥梁：接收CardView点击事件并执行移动操作
 */
class GameController {
public:
    /**
     * 构造函数
     * @param gameModel 游戏数据模型，用于初始化控制器状态
     */
    GameController(GameModel gameModel);
    
    /**
     * 析构函数
     * 释放控制器相关资源
     */
    ~GameController();

    /**
     * 从游戏区选择卡牌并验证匹配规则
     * 检查选中卡牌与栈底卡牌是否符合相邻数值规则
     * @param selectedCard 选中的游戏区卡牌模型
     * @return 匹配成功返回true，否则返回false
     */
    bool selectCardFromPlayefieldAndMatch(CardModel& selectedCard);

    /**
     * 处理牌堆区(Stack)卡牌点击事件
     * 记录当前操作状态到撤销管理器，用于后续撤销操作
     * @param card 被点击的牌堆区卡牌模型
     */
    void clickStackCard(CardModel& card);

    /**
     * 处理卡牌点击后的核心逻辑
     * 包括移动卡牌到目标区域、更新状态及执行动画
     * @param card 被点击的卡牌模型
     */
    void handleCardClicked(CardModel& card);

    /**
     * 执行撤销操作
     * 从撤销管理器获取最近操作状态，恢复卡牌到原始位置和区域
     * @return 撤销成功返回true，无撤销记录时返回false
     */
    bool undo();

    /**
     * 处理标签点击事件（通常用于触发撤销操作）
     */
    void handleLabelClick();

private:
    GameModel _gameModel;       // 游戏数据模型，存储卡牌集合及状态
    UndoManager _undoManager;   // 撤销管理器，负责记录和恢复操作状态

    /**
     * 获取撤销栈中最底部的卡牌模型
     * 用于匹配验证时作为基准卡牌
     * @return 栈底卡牌模型，栈为空时返回默认卡牌
     */
    CardModel getBottomCard();

    /**
     * 验证两张卡牌是否符合匹配规则（数值相邻）
     * @param card1 待匹配的第一张卡牌
     * @param card2 待匹配的第二张卡牌
     * @return 数值相邻返回true，否则返回false
     */
    bool isCardMatch(const CardModel& card1, const CardModel& card2);

    /**
     * 将卡牌恢复到指定的原始位置和区域
     * @param state 包含原始位置和区域信息的撤销状态
     */
    void moveCardToOriginalPosition(const UndoCardState& state);

    /**
     * 通过卡牌模型获取对应的管理器实例
     * @param card 目标卡牌模型
     * @return 卡牌管理器指针，未找到时返回nullptr
     */
    CardManager* getCardManager(const CardModel& card);
};
#endif