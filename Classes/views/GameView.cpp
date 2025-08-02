#include "GameView.h"

GameView* GameView::create(GameModel& model) {
    GameView* pRet = new(std::nothrow) GameView();
    if (pRet && pRet->init(model)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool GameView::init(GameModel& model) {
    if (!Node::init()) {
        return false;
    }

    // 初始化游戏控制器并关联模型
    _gameController = std::make_unique<GameController>(model);
    if (!_gameController) {
        CCLOG("GameView: 创建GameController失败");
        return false;
    }

    // 生成所有卡牌视图
    generateCardViews(model);

    // 1. 创建撤销标签（交互控件）
    _statusLabel = cocos2d::Label::createWithSystemFont(u8"撤销", "Microsoft YaHei", 36);
    if (!_statusLabel) {
        CCLOG("GameView: 创建状态标签失败");
        CC_SAFE_DELETE(_gameController);
        return false;
    }

    _statusLabel->setPosition(900, 400);
    _statusLabel->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(_statusLabel, 100); // z轴层级100（确保显示在最上层）

    // 输出标签属性到日志
    CCLOG("标签尺寸: %f, %f",
        _statusLabel->getContentSize().width,
        _statusLabel->getContentSize().height);
    CCLOG("标签位置: %f, %f",
        _statusLabel->getPositionX(),
        _statusLabel->getPositionY());

    // 2. 注册触摸事件监听器（处理标签点击）
    registerTouchEvents();
    return true;
}

void GameView::generateCardViews(GameModel& model) {
    // 创建游戏区卡牌视图
    const auto& playfield = model.getPlayfield();
    for (const auto& cardModel : playfield) {
        CardView* cardView = CardView::create(cardModel, Vec2(0, 0));
        if (cardView) {
            _playfieldCardViews.push_back(cardView);
            this->addChild(cardView);
        }
        else {
            CCLOG("GameView: 创建游戏区卡牌视图失败，ID: %d", cardModel._id);
        }
    }

    // 创建牌堆区卡牌视图
    const auto& stackfield = model.getStackfield();
    for (const auto& cardModel : stackfield) {
        CardView* cardView = CardView::create(cardModel, Vec2(0, 0));
        if (cardView) {
            _stackfieldCardViews.push_back(cardView);
            this->addChild(cardView);
        }
        else {
            CCLOG("GameView: 创建牌堆区卡牌视图失败，ID: %d", cardModel._id);
        }
    }

    // 卡牌点击处理逻辑（统一回调）
    auto handleCardClick = [this](CardView* cardView) {
        if (!cardView) return;

        // 视觉反馈：降低透明度表示选中
        cardView->setOpacity(180);

        // 通过管理器获取模型数据
        CardModel& cardModel = cardView->_cardManager->getModel();
        CCLOG(u8"卡牌点击 - ID: %d，区域: %d", 
            cardModel._id, static_cast<int>(cardModel.getZone()));

        // 根据卡牌所在区域分发事件到控制器
        if (cardModel.getZone() == CardZone::Playfield) {
            _gameController->selectCardFromPlayefieldAndMatch(cardModel);
        }
        else if (cardModel.getZone() == CardZone::Stack) {
            _gameController->clickStackCard(cardModel);
        }
    };

    // 为游戏区卡牌绑定点击回调
    for (auto cardView : _playfieldCardViews) {
        if (cardView) {
            cardView->setClickCallback(handleCardClick);
        }
    }

    // 为牌堆区卡牌绑定点击回调
    for (auto cardView : _stackfieldCardViews) {
        if (cardView) {
            cardView->setClickCallback(handleCardClick);
        }
    }
}

void GameView::registerTouchEvents() {
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    if (!touchListener) {
        CCLOG("GameView: 创建触摸监听器失败");
        return;
    }

    touchListener->setSwallowTouches(true);

    // 触摸开始：检测是否点击标签
    touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!touch || !_statusLabel) return false;

        cocos2d::Vec2 touchPos = this->convertToNodeSpace(touch->getLocation());
        if (_statusLabel->getBoundingBox().containsPoint(touchPos)) {
            _statusLabel->setScale(1.2f); // 触摸缩放反馈
            return true;
        }
        return false;
    };

    // 触摸结束：处理标签点击事件
    touchListener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!_statusLabel) return;

        _statusLabel->setScale(1.0f); // 恢复缩放

        if (touch) {
            cocos2d::Vec2 touchPos = this->convertToNodeSpace(touch->getLocation());
            if (_statusLabel->getBoundingBox().containsPoint(touchPos)) {
                onLabelClicked(); // 触发撤销操作
            }
        }
    };

    // 触摸取消：恢复标签状态
    touchListener->onTouchCancelled = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (_statusLabel) {
            _statusLabel->setScale(1.0f);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameView::onLabelClicked() {
    CCLOG(u8"撤销标签被点击 - 触发撤销操作");
    if (_gameController) {
        _gameController->handleLabelClick();
    }
}

// 析构函数 - 智能指针会自动释放资源
GameView::~GameView() {
}