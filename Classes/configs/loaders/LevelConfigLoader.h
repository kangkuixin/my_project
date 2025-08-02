#pragma once
#ifndef CONFIGS_LOADERS_LEVELCONFIGLOADER_H
#define CONFIGS_LOADERS_LEVELCONFIGLOADER_H

#include "configs/models/LevelConfig.h"
#include "cocos2d.h"
#include <memory>
#include <fstream>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/filereadstream.h"
#include "models/CardModel.h"

using namespace rapidjson;

/*
关卡配置加载器（单例模式）：负责加载JSON格式的关卡配置文件
 */
class LevelConfigLoader final {
public:
    // 加载指定关卡配置文件
    static LevelConfig* loadLevelConfig(std::string fileName);

private:
    static int currentId;  // 用于生成卡片唯一ID的计数器
    
    LevelConfigLoader() = default;
    
    LevelConfigLoader(const LevelConfigLoader&) = delete;
    
    LevelConfigLoader& operator=(const LevelConfigLoader&) = delete;
    
    // 解析卡片模型数据
    static bool parseCardModel(const rapidjson::Value& cardNode, 
                              std::vector<CardModel>& target, 
                              CardZone zone);
};

#endif // CONFIGS_LOADERS_LEVELCONFIGLOADER_H