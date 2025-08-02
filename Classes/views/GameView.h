#ifndef GAME_VIEW_H_
#define GAME_VIEW_H_

#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <vector>
#include <memory>
#include "controllers/GameController.h"

USING_NS_CC;

/*
游戏主视图类，负责管理全局游戏界面元素与交互
核心功能：
1. 管理游戏区（Playfield）和牌堆区（Stack）的所有卡牌视图
2. 维护全局UI元素（如撤销标签）并处理其交互事件
3. 通过GameController衔接视图与数据模型，转发用户操作
 */
class GameView : public Node {
public:
    /*
    静态创建方法，初始化游戏视图并关联数据模型
    @param model 游戏数据模型，包含需要显示的卡牌信息
    @return 成功创建返回GameView实例，失败返回nullptr
    */
    static GameView* create(GameModel& model);

protected:
    /*
    初始化方法，设置视图层级与控制器
    @param model 游戏数据模型
    @return 初始化成功返回true，否则返回false
    */
    bool init(GameModel& model);

    /*
    根据游戏模型生成所有卡牌视图
    为每个卡牌创建对应的CardView并绑定点击事件
    @param model 游戏数据模型，包含卡牌集合信息
    */
    void generateCardViews(GameModel& model);

private:
    std::vector<CardView*> _playfieldCardViews; // 游戏区卡牌视图集合
    std::vector<CardView*> _stackfieldCardViews; // 牌堆区卡牌视图集合

    cocos2d::Label* _statusLabel; // 状态标签（可作为撤销按钮）
    std::unique_ptr<GameController> _gameController; // 游戏控制器，处理业务逻辑

    /*
    标签点击事件处理方法
    触发撤销操作
    */
    void onLabelClicked();

    /*
    注册触摸事件监听器
    处理全局UI元素（如标签）的触摸交互
    */
    void registerTouchEvents();
};

#endif // GAME_VIEW_H_