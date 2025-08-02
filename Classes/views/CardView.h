#ifndef CARD_VIEW_H_
#define CARD_VIEW_H_

#include "cocos2d.h"
#include "models/CardModel.h"      // 卡牌数据模型
#include "configs/CardResConfig.h" // 卡牌资源配置
#include "managers/CardManager.h"
#include <functional>

class CardManager;
USING_NS_CC;

/*
卡牌视图类，负责卡牌的视觉呈现与用户交互检测
核心组成：
- 背景层（_background）：作为基础容器承载所有元素
- 左上角小数字（_smallNumber）：显示卡牌数值的小型图标
- 中间大数字（_bigNumber）：突出显示卡牌数值的大型图标
- 右上角花色图标（_suitIcon）：展示卡牌花色的图标
通过CardManager与CardModel关联，实现数据与视图的同步
 */
class CardView : public Node {
public:
    /**
     * 静态创建方法
     * @param model 卡牌数据模型，提供花色、数值等信息
     * @param offset 位置偏移量，用于调整卡牌在场景中的显示位置
     * @return 成功返回CardView实例，失败返回nullptr
     */
    static CardView* create(const CardModel& model, const Vec2& offset);
    
    /**
     * 点击回调函数类型定义
     * 参数为触发事件的CardView实例
     */
    using ClickCallback = std::function<void(CardView* cardView)>;
    
    /**
     * 设置卡牌点击回调
     * @param callback 回调函数，触摸结束时触发
     */
    void setClickCallback(const ClickCallback& callback);

    /**
     * 加载卡牌背景图
     * 从CardResConfig获取资源路径并创建背景精灵
     */
    void loadBackground();
    
    /**
     * 加载左上角小数字图标
     * @param model 卡牌数据模型，用于确定数字样式
     */
    void loadSmallNumber(const CardModel& model);
    
    /**
     * 加载中间大数字图标
     * @param model 卡牌数据模型，用于确定数字样式
     */
    void loadBigNumber(const CardModel& model);
    
    /**
     * 加载右上角花色图标
     * @param model 卡牌数据模型，用于确定花色样式
     */
    void loadSuitIcon(const CardModel& model);

    /**
     * 初始化方法
     * @param model 卡牌数据模型
     * @param offset 位置偏移量
     * @return 初始化成功返回true，否则返回false
     */
    bool init(const CardModel& model, const Vec2& offset);
    
    /**
     * 判断触摸点是否在卡牌范围内
     * @param touchPos 触摸点在卡牌局部坐标系中的位置
     * @return 在范围内返回true，否则返回false
     */
    bool isTouchInside(const cocos2d::Vec2& touchPos);

    CardManager* _cardManager;     // 关联的卡牌管理器（处理交互逻辑）

private:
    Sprite* _background = nullptr;  // 卡牌背景精灵
    Sprite* _smallNumber = nullptr; // 左上角小数字精灵
    Sprite* _bigNumber = nullptr;   // 中间大数字精灵
    Sprite* _suitIcon = nullptr;    // 右上角花色图标精灵

    // 元素相对位置（基于背景图中心锚点的偏移）
    const Vec2 _smallNumberPos = Vec2(-80, 130);  // 左上角小数字位置
    const Vec2 _suitIconPos = Vec2(80, 130);     // 右上角花色图标位置
    const Vec2 _bigNumberPos = Vec2(0, 0);       // 中间大数字位置

    ClickCallback _clickCallback;  // 点击回调函数
    bool _isSelected = false;      // 卡牌选中状态标记
};

#endif // CARD_VIEW_H_