#ifndef UNDO_MODEL_H_
#define UNDO_MODEL_H_

#include "cocos2d.h"
#include "CardModel.h"  

USING_NS_CC;

/**
 * 撤销状态结构体，存储单次卡牌操作的关键状态
 */
struct UndoCardState {
    int id;                 // 卡牌唯一标识符
    Vec2 position;          // 操作前的位置坐标
    CardZone zone;          // 操作前所在的区域
};

/*
撤销数据模型类，负责存储和管理操作历史记录
核心功能：
1. 记录每次卡牌操作前的状态（位置、区域等）
2. 支持按"后进先出"顺序获取历史状态，实现撤销功能
3. 提供历史记录管理接口（清空、查询等）
 */
class UndoModel {
public:
    /**
     * 默认构造函数
     */
    UndoModel() = default;

    /**
     * 记录单次操作的卡牌状态
     * @param state 包含卡牌ID、位置和区域的状态数据
     */
    void record(const UndoCardState& state) {
        _history.push_back(state);  // 将新状态追加到历史记录
    }

    /**
     * 获取最近一次操作的状态并从历史中移除
     * @param outState 输出参数，用于接收历史状态
     * @return 成功获取返回true，无历史记录返回false
     */
    bool undo(UndoCardState& outState) {
        if (_history.empty()) {
            return false;  // 无历史记录可撤销
        }
        // 获取最后一次操作的状态
        outState = _history.back();
        // 移除最后一次操作记录（实现撤销）
        _history.pop_back();
        return true;
    }

    /**
     * 清空所有历史记录
     */
    void clearHistory() {
        _history.clear();
    }

    /**
     * 检查是否存在可撤销的操作
     * @return 有历史记录返回true，否则返回false
     */
    bool canUndo() const { 
        return !_history.empty(); 
    }

    /**
     * 获取历史记录的数量
     * @return 记录总数
     */
    int getSize() const { 
        return _history.size(); 
    }

private:
    std::vector<UndoCardState> _history;  // 按操作顺序存储的历史状态列表
};

#endif // UNDO_MODEL_H_