#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

 /**
  * @brief 游戏主场景类，继承自 cocos2d::Scene
  * 作为游戏的入口场景，负责初始化整体界面布局（包括背景分层）、
  * 加载关卡数据、创建游戏核心视图，并处理全局UI交互（如关闭按钮）
  */
class HelloWorld : public cocos2d::Scene
{
public:
    /**
     * @brief 创建场景实例的静态方法
     * 封装场景创建逻辑，返回可直接使用的场景实例
     * @return 指向HelloWorld场景实例的指针
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief 初始化场景
     * 实现场景的核心初始化逻辑：设置背景分层、添加交互控件、
     * 加载关卡配置并生成游戏模型与视图
     * @return 初始化成功返回true，否则返回false
     */
    virtual bool init() override;

    /**
     * @brief 关闭按钮的回调方法
     * 处理关闭按钮点击事件，终止游戏并退出应用程序
     * @param pSender 事件发送者（通常为按钮实例）
     */
    void menuCloseCallback(cocos2d::Ref* pSender);

    // 启用CREATE_FUNC宏，自动生成create()方法实现
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__