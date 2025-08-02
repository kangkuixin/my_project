#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "views/CardView.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/GameModelFromLevelGenerator.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// 资源加载失败提示函数
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 场景初始化方法
bool HelloWorld::init()
{
    // 1. 调用父类初始化方法
    if (!Scene::init())
    {
        return false;
    }

    // 获取屏幕可视区域尺寸和原点坐标
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 2. 添加关闭按钮（用于退出程序）
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        // 计算关闭按钮位置（右上角）
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // 创建菜单并添加关闭按钮
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 3. 初始化游戏界面布局

    // ========== 上方游戏区域背景层（占屏幕2/3高度） ==========
    float topHeight = visibleSize.height * 2.0f / 3.0f;
    Color4B topColor(139, 69, 19, 255); // 深棕色背景
    auto topLayer = LayerColor::create(topColor, visibleSize.width, topHeight);
    topLayer->setPosition(Vec2(0, visibleSize.height - topHeight)); // 顶部对齐
    addChild(topLayer, 0); // z轴层级0（背景层）

    // ========== 下方操作区域背景层（占屏幕1/3高度） ==========
    float bottomHeight = visibleSize.height - topHeight;
    Color4B bottomColor(128, 0, 128, 255); // 紫色背景（操作区）
    auto bottomLayer = LayerColor::create(bottomColor, visibleSize.width, bottomHeight);
    bottomLayer->setPosition(Vec2(0, 0)); // 底部对齐
    addChild(bottomLayer, 0);

    // 4. 生成游戏模型和视图
    auto gameModel = GameModelFromLevelGenerator::generateGameModel("level_1.json");
    GameModelFromLevelGenerator::generateGameView(gameModel, this);

    return true;
}

// 关闭按钮点击事件回调
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // 终止游戏主循环
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0); // iOS平台需要显式调用exit退出
#endif

    
}