#ifndef GAME_MODEL_FROM_LEVEL_GENERATOR_H_
#define GAME_MODEL_FROM_LEVEL_GENERATOR_H_

#include "models/GameModel.h"
#include "views/GameView.h"
#include "configs/models/LevelConfig.h" 
#include "configs/loaders/LevelConfigLoader.h"
#include <vector>

USING_NS_CC;

/*
关卡模型生成器类，负责将静态关卡配置转换为运行时游戏对象
核心功能：
1. 从JSON配置文件加载数据并生成GameModel实例
2. 根据游戏模型创建并初始化对应的GameView视图
采用静态类设计，所有方法均为静态，无需实例化即可使用
 */
class GameModelFromLevelGenerator {
public:
    /*
    从关卡配置文件生成游戏数据模型
    @param levelFile 关卡配置文件路径（相对于资源目录）
    @return 生成的GameModel实例，包含从配置加载的卡牌数据
    */
    static GameModel generateGameModel(const std::string levelFile) {
        auto config = LevelConfigLoader::loadLevelConfig(levelFile);
        GameModel gameModel(config);
        return gameModel;
    }

    /*
    根据游戏模型生成并初始化游戏视图
    @param gameModel 游戏数据模型，包含需要显示的卡牌信息
    @param parent 视图的父节点，用于将游戏视图添加到场景层级
    */
    static void generateGameView(GameModel& gameModel, Node* parent) {
        // 创建游戏视图实例并关联模型
        auto gameView = GameView::create(gameModel);
        if (gameView) {
            parent->addChild(gameView, 1); // 添加到父节点，z轴层级1（确保显示在背景上层）
        }
    }

private:
    // 私有构造函数防止外部实例化（静态工具类）
    GameModelFromLevelGenerator() = default;
};

#endif // GAME_MODEL_FROM_LEVEL_GENERATOR_H_