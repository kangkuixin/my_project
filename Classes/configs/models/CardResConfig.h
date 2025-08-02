#ifndef CARD_RES_CONFIG_H
#define CARD_RES_CONFIG_H

#include <string>
#include "models/CardModel.h"

/*
    卡牌资源配置类
    集中管理所有卡牌相关资源路径，采用静态类设计确保资源访问的一致性
    当需要修改资源路径或扩展资源时，只需修改此类而不影响业务逻辑，符合开放封闭原则
 */
class CardResConfig
{
public:
    /*
    获取卡牌背景图片路径
    @return 背景图资源路径，示例："res/card_general.png"
     */
    static std::string getBackGround()
    {
        return "res/card_general.png";
    }

    /*
    获取花色图标资源路径（用于卡牌角落显示）
    @param suit 花色枚举值
    @return 对应花色的图标路径，示例："res/suits/club.png"，无效值返回空字符串
     */
    static std::string getSuitRes(CardSuitType suit)
    {
        switch (suit)
        {
        case CardSuitType::CST_CLUBS:    return "res/suits/club.png";    // 梅花
        case CardSuitType::CST_DIAMONDS: return "res/suits/diamond.png"; // 方块
        case CardSuitType::CST_HEARTS:   return "res/suits/heart.png";   // 红桃
        case CardSuitType::CST_SPADES:   return "res/suits/spade.png";   // 黑桃
        default: return ""; // 处理无效枚举值
        }
    }

    /*
    获取角落小数字资源路径
    @param suit 花色枚举值（用于确定颜色）
    @param face 牌面枚举值（用于确定数字/字母）
    @return 小数字资源路径，示例："res/number/small_black_3.png"，无效参数返回空字符串
     */
    static std::string getSmallNumberRes(CardSuitType suit, CardFaceType face)
    {
        std::string color = suitToColor(suit);
        std::string faceStr = faceToString(face);

        if (color.empty() || faceStr.empty())
        {
            return ""; // 无效参数处理
        }

        return "res/number/small_" + color + "_" + faceStr + ".png";
    }

    /*
    获取中间大数字资源路径
    @param suit 花色枚举值（用于确定颜色）
    @param face 牌面枚举值（用于确定数字/字母）
    @return 大数字资源路径，示例："res/number/big_red_3.png"，无效参数返回空字符串
     */
    static std::string getBigNumberRes(CardSuitType suit, CardFaceType face)
    {
        std::string color = suitToColor(suit);
        std::string faceStr = faceToString(face);

        if (color.empty() || faceStr.empty())
        {
            return ""; // 无效参数处理
        }

        return "res/number/big_" + color + "_" + faceStr + ".png";
    }

private:
    /*
    将牌面枚举值转换为对应字符串（A、2-10、J、Q、K）
    @param face 牌面枚举值
    @return 对应的字符串表示，无效值返回空字符串
     */
    static std::string faceToString(CardFaceType face)
    {
        // 与CardFaceType枚举顺序一一对应
        const std::string faceNames[] = {
            "A", "2", "3", "4", "5", "6", "7",
            "8", "9", "10", "J", "Q", "K"
        };

        int index = static_cast<int>(face);
        // 校验范围：排除CFT_NONE(-1)和CFT_NUM_CARD_FACE_TYPES(13)
        if (index < 0 || index >= static_cast<int>(CardFaceType::CFT_NUM_CARD_FACE_TYPES))
        {
            return ""; // 无效牌面处理
        }

        return faceNames[index];
    }

    /*
    根据花色判断颜色（黑/红）
    @param suit 花色枚举值
    @return "black"（梅花/黑桃）或"red"（方块/红桃），无效值返回空字符串
     */
    static std::string suitToColor(CardSuitType suit)
    {
        switch (suit)
        {
        case CardSuitType::CST_CLUBS:    // 梅花
        case CardSuitType::CST_SPADES:   // 黑桃
            return "black";
        case CardSuitType::CST_DIAMONDS: // 方块
        case CardSuitType::CST_HEARTS:   // 红桃
            return "red";
        default:
            return ""; // 无效花色处理
        }
    }
};
#endif // CARD_RES_CONFIG_H