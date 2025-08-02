#ifndef CARD_ID_VIEW_MAP_H
#define CARD_ID_VIEW_MAP_H

#include "managers/CardManager.h"
#include <unordered_map>

/*
卡牌管理器全局映射类，通过单例模式提供卡牌ID与管理器的快速关联
核心功能：
1. 维护卡牌ID到CardManager实例的映射关系
2. 支持添加、查询、移除映射关系，提供O(1)平均时间复杂度的操作
3. 作为全局访问点，便于控制器快速定位指定卡牌的管理器
 */
class CardIdManagerMap {
public:
    /**
     * 获取单例实例
     * @return 全局唯一的CardIdManagerMap实例引用
     */
    static CardIdManagerMap& getInstance() {
        static CardIdManagerMap instance;
        return instance;
    }

    /**
     * 添加卡牌ID与管理器的映射
     * @param cardId 卡牌唯一标识符
     * @param manager 对应的卡牌管理器实例
     */
    void addCardManager(int cardId, CardManager* manager) {
        _cardIdManagerMap[cardId] = manager;
    }

    /**
     * 根据卡牌ID查询管理器
     * @param cardId 卡牌唯一标识符
     * @return 找到返回对应的CardManager指针，否则返回nullptr
     */
    CardManager* getCardManager(int cardId) {
        auto it = _cardIdManagerMap.find(cardId);
        if (it != _cardIdManagerMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * 移除指定卡牌ID的映射关系
     * @param cardId 卡牌唯一标识符
     */
    void removeCardManager(int cardId) {
        _cardIdManagerMap.erase(cardId);
    }

private:
    // 私有构造与析构，确保单例模式
    CardIdManagerMap() {}
    ~CardIdManagerMap() {}

    // 禁用拷贝构造与赋值，防止单例被复制
    CardIdManagerMap(const CardIdManagerMap&) = delete;
    CardIdManagerMap& operator=(const CardIdManagerMap&) = delete;

    std::unordered_map<int, CardManager*> _cardIdManagerMap; // ID与管理器的映射表
};

#endif