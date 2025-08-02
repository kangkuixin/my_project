#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

 /**
  * @brief Cocos2d-x 应用程序入口类
  *
  * 该类继承自 cocos2d::Application，是应用程序的核心控制类，
  * 负责管理应用程序的生命周期、初始化渲染环境、配置游戏窗口
  * 以及处理前后台切换等系统事件。
  */
class AppDelegate : private cocos2d::Application
{
public:
    /**
     * @brief 构造函数
     * 初始化应用程序的基础环境，设置默认配置参数
     */
    AppDelegate();

    /**
     * @brief 析构函数
     * 释放应用程序运行过程中分配的资源，包括音频引擎等
     */
    virtual ~AppDelegate();

    /**
     * @brief 初始化 OpenGL 上下文属性
     * 配置 OpenGL 渲染环境的核心参数，如颜色通道位数、深度缓存、
     * 模板缓存等，确保渲染效果符合预期
     */
    virtual void initGLContextAttrs() override;

    /**
     * @brief 应用程序启动完成时调用
     * 在此完成游戏的初始化工作：创建 OpenGL 视图、配置导演参数、
     * 设置分辨率适配策略、注册资源包并加载初始场景
     * @return 初始化成功返回 true，应用程序正常启动；返回 false 则终止运行
     */
    virtual bool applicationDidFinishLaunching() override;

    /**
     * @brief 应用程序进入后台时调用
     * 处理暂停逻辑：停止动画渲染、暂停音频播放、保存游戏状态等，
     * 确保应用程序在后台时资源高效利用
     */
    virtual void applicationDidEnterBackground() override;

    /**
     * @brief 应用程序返回前台时调用
     * 恢复应用程序运行状态：重启动画渲染、恢复音频播放、
     * 重新激活游戏逻辑等，确保用户体验的连贯性
     */
    virtual void applicationWillEnterForeground() override;
};

#endif // _APP_DELEGATE_H_