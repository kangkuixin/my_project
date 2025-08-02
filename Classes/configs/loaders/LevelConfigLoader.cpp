#include "LevelConfigLoader.h"

// 初始化静态成员变量：用于生成卡牌唯一ID的计数器
int LevelConfigLoader::currentId = 0;

/*
从指定JSON文件加载关卡配置数据并转换为LevelConfig对象
@param fileName 配置文件路径（相对于资源目录）
@return 成功返回LevelConfig实例指针，解析失败返回nullptr
*/
LevelConfig* LevelConfigLoader::loadLevelConfig(std::string fileName)
{
    // 读取JSON文件内容到字符串
    std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(fileName);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(jsonStr.c_str());

    // 检查JSON解析错误
    if (doc.HasParseError())
    {
        CCLOG("JSON解析错误: %s", doc.GetParseError());
        return nullptr;
    }

    // 创建配置对象并验证根节点类型
    auto config = new LevelConfig();
    if (!doc.IsObject())
    {
        CCLOG("LevelConfigLoader: 根节点不是JSON对象");
        return nullptr;
    }

    // 重置ID计数器（确保每次加载关卡时ID从0开始）
    currentId = 0;

    // 解析Playfield区域卡牌数组
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const rapidjson::Value& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); ++i)
        {
            const rapidjson::Value& cardNode = playfieldArray[i];
            if (!parseCardModel(cardNode, config->_playfieldCards, CardZone::Playfield))
            {
                CCLOG("LevelConfigLoader: Playfield区域第%d张卡牌解析失败", i);
            }
        }
    }

    // 解析Stack区域卡牌数组
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& stackArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); ++i)
        {
            const rapidjson::Value& cardNode = stackArray[i];
            if (!parseCardModel(cardNode, config->_stackCards, CardZone::Stack))
            {
                CCLOG("LevelConfigLoader: Stack区域第%d张卡牌解析失败", i);
            }
        }
    }

    return config;
}

/*
将JSON中的单张卡牌节点解析为CardModel对象并添加到目标容器
@param cardNode JSON中单个卡牌的节点数据
@param target 存储解析结果的CardModel向量容器
@param zone 该卡牌所属的游戏区域（Playfield/Stack）
@return 解析成功返回true，字段缺失或格式错误返回false
*/
bool LevelConfigLoader::parseCardModel(const rapidjson::Value& cardNode,
    std::vector<CardModel>& target,
    CardZone zone)
{
    // 基础校验：节点必须是JSON对象
    if (!cardNode.IsObject())
        return false;

    // 校验必要字段是否存在且类型正确
    if (!cardNode.HasMember("CardFace") || !cardNode["CardFace"].IsInt())
        return false;
    if (!cardNode.HasMember("CardSuit") || !cardNode["CardSuit"].IsInt())
        return false;
    if (!cardNode.HasMember("Position") || !cardNode["Position"].IsObject())
        return false;

    // 解析位置信息（x,y坐标）
    const rapidjson::Value& posNode = cardNode["Position"];
    if (!posNode.HasMember("x") || !posNode["x"].IsInt() ||
        !posNode.HasMember("y") || !posNode["y"].IsInt())
    {
        return false;
    }

    // 提取字段原始值
    int faceInt = cardNode["CardFace"].GetInt();
    int suitInt = cardNode["CardSuit"].GetInt();
    float x = posNode["x"].GetFloat();
    float y = posNode["y"].GetFloat();

    // 校验数值范围（CardFace: 0~12对应ACE~KING，CardSuit: 0~3对应四种花色）
    if (faceInt < 0 || faceInt > 12)
        return false;
    if (suitInt < 0 || suitInt > 3)
        return false;

    // 类型转换：整数->枚举，坐标->Vec2
    CardFaceType face = static_cast<CardFaceType>(faceInt);
    CardSuitType suit = static_cast<CardSuitType>(suitInt);
    cocos2d::Vec2 pos(x, y);

    // 生成唯一ID并根据区域偏移位置（区分游戏区和牌堆区的显示位置）
    int id = currentId++;
    if (zone == CardZone::Stack)
        pos += cocos2d::Vec2(300, 400);  // 牌堆区卡牌位置偏移
    else
        pos += cocos2d::Vec2(0, 600);    // 游戏区卡牌位置偏移

    // 直接在容器中构造CardModel对象（避免临时对象拷贝）
    target.emplace_back(face, suit, pos, id, zone);
    return true;
}