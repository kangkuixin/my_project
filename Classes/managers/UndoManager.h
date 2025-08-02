#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H
#include "models/UndoModel.h"

/*
撤销管理器类，负责统一处理游戏中的撤销操作逻辑
通过封装UndoModel实现操作记录与恢复功能，采用栈结构存储操作状态
核心职责：
- 记录卡牌操作状态（位置、区域等）
- 执行撤销操作，恢复卡牌到上一状态
- 提供撤销状态查询与历史记录管理接口
 */
class UndoManager {
public:
    /**
     * 构造函数
     * @param undoModel 撤销数据模型，用于存储操作历史
     */
    UndoManager(UndoModel& undoModel) 
        : _undoModel(undoModel) {}

    /**
     * 记录操作状态到撤销历史
     * @param state 包含卡牌ID、位置、区域的状态信息
     */
    void recordUndoState(const UndoCardState& state) {
        _undoModel.record(state);
    }

    /**
     * 执行撤销操作
     * @param outState 输出参数，用于接收被撤销的状态信息
     * @return 撤销成功返回true，无历史记录返回false
     */
    bool undo(UndoCardState& outState) {
        return _undoModel.undo(outState);
    }

    /**
     * 检查是否有可撤销的操作
     * @return 存在撤销历史返回true，否则返回false
     */
    bool canUndo() const {
        return _undoModel.canUndo();
    }

    /**
     * 清除所有撤销历史记录
     */
    void clearUndoHistory() {
        _undoModel.clearHistory();
    }

    /**
     * 获取当前撤销历史的记录数量
     * @return 历史记录条数
     */
    int getUndoSize() {
        return _undoModel.getSize();
    }

    /**
     * 获取最后一个操作状态但不从历史中移除
     * @param outState 输出参数，用于接收最后一个状态信息
     * @return 成功获取返回true，无历史记录返回false
     */
    bool getLastState(UndoCardState& outState) {
        if (!_undoModel.canUndo()) {
            return false;
        }
        // 创建一个临时副本用于获取最后状态
        UndoModel tempModel = _undoModel;
        return tempModel.undo(outState);
    }

private:
    UndoModel _undoModel;  // 撤销数据模型，存储操作历史记录
};
#endif