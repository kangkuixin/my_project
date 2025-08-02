#pragma once
#ifndef CONFIGS_MODELS_LEVELCONFIG_H
#define CONFIGS_MODELS_LEVELCONFIG_H

#include "cocos2d.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#include "models/CardModel.h"

using namespace rapidjson;

/*
关卡配置数据模型类，用于存储单关卡的静态卡牌配置信息
包含游戏区（Playfield）和牌堆区（Stack）的卡牌数据集合
通过LevelConfigLoader加载JSON文件生成实例，外部通过公开接口访问数据
 */
class LevelConfig final
{
public:
    // 获取游戏区卡牌列表（返回拷贝，避免外部直接修改内部数据）
    std::vector<CardModel> getPlayfield()
    {
        return _playfieldCards;
    }
    
    // 获取牌堆区卡牌列表（返回拷贝，避免外部直接修改内部数据）
    std::vector<CardModel> getStack()
    {
        return _stackCards;
    }

private:
    std::vector<CardModel> _playfieldCards;  //< 游戏区卡牌集合，对应JSON中的"Playfield"字段
    std::vector<CardModel> _stackCards;      //< 牌堆区卡牌集合，对应JSON中的"Stack"字段

    // 限制实例化与拷贝：仅允许LevelConfigLoader创建和初始化
    LevelConfig() = default;                  //< 私有默认构造函数，禁止外部直接实例化
    ~LevelConfig() = default;                 //< 私有析构函数，由加载器管理生命周期
    LevelConfig(const LevelConfig&) = delete; //< 禁用拷贝构造
    LevelConfig& operator=(const LevelConfig&) = delete; //< 禁用赋值操作

    // 友元类声明：允许配置加载器访问私有成员进行数据初始化
    friend class LevelConfigLoader;
};

#endif // CONFIGS_MODELS_LEVELCONFIG_H