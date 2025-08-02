// CardManager.h
#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H

#include "models/CardModel.h"
#include "views/CardView.h"
#include "cocos2d.h"
#include <functional>

class CardView;
/*
卡牌管理器类，负责卡牌的交互逻辑与数据视图绑定
核心功能：
1. 管理卡牌触摸事件生命周期（开始、移动、结束、取消）
2. 作为CardModel与CardView的中间层，同步数据与视图状态
3. 提供点击回调机制，将用户交互传递给控制器
4. 维护卡牌选中状态并提供视觉反馈（如缩放效果）
 */
class CardManager {
public:
    /**
     * 构造函数
     * @param model 卡牌数据模型，用于初始化管理器
     */
    CardManager(const CardModel& model);
    
    /**
     * 析构函数
     * 清理资源并从全局映射中移除自身
     */
    ~CardManager();

    /**
     * 绑定卡牌模型与视图
     * @param model 新的卡牌数据模型
     * @param view 对应的卡牌视图实例
     */
    void setCard(const CardModel& model, CardView* view);
    
    /**
     * 初始化触摸事件监听
     * 绑定触摸生命周期回调函数
     */
    void setupTouchEvents();

    /**
     * 触摸开始事件回调
     * @param touch 触摸对象
     * @param event 事件对象
     * @return 触摸在卡牌范围内返回true，否则返回false
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    /**
     * 触摸移动事件回调
     * 实现卡牌拖拽逻辑
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    /**
     * 触摸结束事件回调
     * 恢复卡牌状态并触发点击回调（如果需要）
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    /**
     * 触摸取消事件回调
     * 恢复卡牌状态
     * @param touch 触摸对象
     * @param event 事件对象
     */
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * 设置卡牌点击回调函数
     * @param callback 回调函数，触摸结束时触发
     */
    void setCardClickedCallback(const std::function<void(CardModel&)>& callback);
    
    /**
     * 获取关联的卡牌视图
     * @return 卡牌视图指针
     */
    CardView* getView() { return _view; }
    
    /**
     * 获取关联的卡牌模型
     * @return 卡牌数据模型
     */
    CardModel getModel() { return _model; }

private:
    CardModel _model;                          // 卡牌数据模型
    CardView* _view;                           // 卡牌视图实例
    bool _isSelected;                          // 卡牌选中状态标记
    std::function<void(CardModel&)> _cardClickedCallback;  // 点击回调函数
};

#endif // CARD_MANAGER_H