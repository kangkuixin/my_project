// CardModel.h
#ifndef CARD_MODEL_H_
#define CARD_MODEL_H_

#include "cocos2d.h"
USING_NS_CC;

/**
 * 卡牌所在区域枚举
 */
enum class CardZone {
    Playfield,  // 游戏区：玩家可操作的主要区域
    Stack,      // 牌堆区：存放待抽取卡牌的区域
    Hand,       // 手牌区：玩家已选中的卡牌区域
    Unknown     // 未知区域：默认初始状态
};

/**
 * 卡牌花色枚举
 */
enum class CardSuitType {
    CST_NONE = -1,          // 无效花色
    CST_CLUBS,              // 梅花
    CST_DIAMONDS,           // 方块
    CST_HEARTS,             // 红桃
    CST_SPADES,             // 黑桃
    CST_NUM_CARD_SUIT_TYPES // 花色总数（用于边界检查）
};

/**
 * 卡牌牌面枚举（数值/字母）
 */
enum class CardFaceType {
    CFT_NONE = -1,          // 无效牌面
    CFT_ACE,                // A
    CFT_TWO,                // 2
    CFT_THREE,              // 3
    CFT_FOUR,               // 4
    CFT_FIVE,               // 5
    CFT_SIX,                // 6
    CFT_SEVEN,              // 7
    CFT_EIGHT,              // 8
    CFT_NINE,               // 9
    CFT_TEN,                // 10
    CFT_JACK,               // J
    CFT_QUEEN,              // Q
    CFT_KING,               // K
    CFT_NUM_CARD_FACE_TYPES // 牌面总数（用于边界检查）
};

/**
 * 卡牌数据模型类，存储卡牌的核心属性与状态
 * 包含牌面、花色、位置、所在区域等数据，提供访问与修改接口
 */
class CardModel {
public:
    /**
     * 构造函数（简化版）
     * @param face 牌面类型（A/K/Q等）
     * @param suit 花色类型（红桃/黑桃等）
     * @param position 卡牌在场景中的坐标
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position) :
        _face(face), _suit(suit), _position(position) {}

    /**
     * 构造函数（完整版）
     * @param face 牌面类型
     * @param suit 花色类型
     * @param position 场景坐标
     * @param id 卡牌唯一标识符
     * @param zone 所在区域
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position, int id, CardZone zone) :
        _face(face), _suit(suit), _position(position), _id(id), _zone(zone) {}

    // 获取牌面类型
    CardFaceType getFace() const { return _face; }
    // 获取花色类型
    CardSuitType getSuit() const { return _suit; }
    // 获取当前位置
    const cocos2d::Vec2& getPosition() const { return _position; }
    // 获取所在区域
    const CardZone getZone() const { return _zone; }
    // 设置所在区域
    void setZone(CardZone zone) { _zone = zone; }
    // 设置位置坐标
    void setPosition(Vec2 position) { _position = position; }

    int _id; // 卡牌唯一ID（公开访问用于快速索引）

private:
    CardFaceType _face{ CardFaceType::CFT_ACE };      // 牌面类型（默认A）
    CardSuitType _suit{ CardSuitType::CST_SPADES };   // 花色类型（默认黑桃）
    CardZone _zone{ CardZone::Unknown };              // 所在区域（默认未知）
    cocos2d::Vec2 _position{ cocos2d::Vec2::ZERO };   // 位置坐标（默认原点）
};

#endif // CARD_MODEL_H_