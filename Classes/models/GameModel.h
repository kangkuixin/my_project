// GameModel.h
#ifndef GAME_MODEL_H_
#define GAME_MODEL_H_

#include "cocos2d.h"
#include "CardModel.h"
#include "UndoModel.h"
#include <vector>
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/LevelConfig.h"

USING_NS_CC;

/*
游戏核心数据模型类，管理所有卡牌数据与操作历史
主要职责：
1. 存储游戏区（Playfield）和牌堆区（Stack）的卡牌集合
2. 通过UndoModel维护操作历史，支持撤销功能
3. 提供卡牌增删接口，供控制器修改游戏状态
4. 从关卡配置（LevelConfig）初始化卡牌数据
 */
class GameModel {
public:
    /**
     * 构造函数
     * @param config 关卡配置对象，用于初始化卡牌数据
     */
    GameModel(LevelConfig* config) {
        if (config) {
            // 从配置加载游戏区和牌堆区卡牌
            _playfield = config->getPlayfield();
            _stackfield = config->getStack();

            // 初始化时清空操作历史
            _undoModel.clearHistory();
        }
    }

    /**
     * 获取游戏区卡牌列表（只读）
     * @return 游戏区卡牌集合的常量引用
     */
    const std::vector<CardModel>& getPlayfield() const {
        return _playfield;
    }

    /**
     * 获取牌堆区卡牌列表（只读）
     * @return 牌堆区卡牌集合的常量引用
     */
    const std::vector<CardModel>& getStackfield() const {
        return _stackfield;
    }

    /**
     * 获取撤销模型实例（可修改）
     * @return 撤销模型的引用
     */
    UndoModel& getUndoModel() {
        return _undoModel;
    }

    /**
     * 向游戏区添加卡牌
     * @param card 待添加的卡牌模型
     */
    void addCardToPlayfield(const CardModel& card) {
        _playfield.push_back(card);
    }

    /**
     * 向牌堆区添加卡牌
     * @param card 待添加的卡牌模型
     */
    void addCardToStackfield(const CardModel& card) {
        _stackfield.push_back(card);
    }

    /**
     * 从游戏区移除指定ID的卡牌
     * @param id 待移除卡牌的唯一标识符
     */
    void removeCardFromPlayfield(int id) {
        for (auto it = _playfield.begin(); it != _playfield.end(); ++it) {
            if (it->_id == id) {
                _playfield.erase(it);
                break;
            }
        }
    }

    /**
     * 从牌堆区移除指定ID的卡牌
     * @param id 待移除卡牌的唯一标识符
     */
    void removeCardFromStackfield(int id) {
        for (auto it = _stackfield.begin(); it != _stackfield.end(); ++it) {
            if (it->_id == id) {
                _stackfield.erase(it);
                break;
            }
        }
    }

private:
    std::vector<CardModel> _playfield;   // 游戏区卡牌集合（玩家主要操作区域）
    std::vector<CardModel> _stackfield;  // 牌堆区卡牌集合（待抽取的卡牌区域）
    UndoModel _undoModel;                // 撤销模型，记录操作历史用于回溯
};

#endif // GAME_MODEL_H_