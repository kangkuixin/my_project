# 纸牌游戏 (CardGame)

这是一个基于 Cocos2d-x 3.17 开发的纸牌游戏示例项目，采用 MVC（Model-View-Controller）架构模式，展示了如何使用 Cocos2d-x 引擎开发跨平台纸牌游戏。

## 游戏特色

- 采用 MVC 架构设计，代码结构清晰，便于维护和扩展
- 支持从 JSON 文件加载关卡和纸牌数据
- 实现了完整的纸牌触摸交互系统（点击、拖动、翻转等）
- 具备撤销操作功能
- 可定制的纸牌样式和游戏规则
- 跨平台支持（Windows、macOS、iOS、Android）

## 技术栈

- **游戏引擎**：Cocos2d-x 3.17
- **开发语言**：C++
- **构建系统**：CMake
- **数据格式**：JSON
- **图像资源**：PNG

## 快速开始

### 环境要求

- Cocos2d-x 3.17
- CMake 3.10 或更高版本
- Visual Studio 2019（Windows）或 Xcode（macOS）
- 支持 C++11 的编译器

### 构建步骤

1. 克隆或下载项目到本地

2. 使用 CMake 生成项目文件
   ```bash
   cd cardGame-master
   mkdir build && cd build
   cmake .. -G "Visual Studio 16 2019"  # Windows
   # 或 cmake .. -G "Xcode"  # macOS
   ```

3. 编译项目
   - Windows: 打开生成的解决方案文件 (.sln) 并编译
   - macOS: 打开生成的 Xcode 项目并编译

### 运行游戏

- 编译完成后，可执行文件将位于 `proj.win32/Debug.win32/`（Windows）或相应目录（其他平台）
- 直接运行 `card_game.exe` 即可开始游戏

## 项目架构

### MVC 模式

项目采用经典的 MVC 架构：
- **Model**: 处理游戏数据和业务逻辑
- **View**: 负责游戏界面渲染
- **Controller**: 处理用户输入和游戏流程控制

### 核心模块

```
Classes/
├── models/              # 数据模型
│   ├── CardModel.h      # 纸牌数据模型
│   ├── GameModel.h      # 游戏数据模型
│   └── UndoModel.h      # 撤销操作模型
├── views/               # 视图组件
│   ├── CardView.cpp     # 纸牌视图
│   ├── CardView.h
│   ├── GameView.cpp     # 游戏视图
│   └── GameView.h
├── controllers/         # 控制器
│   ├── GameController.cpp  # 游戏控制器
│   └── GameController.h
├── managers/            # 管理器
│   ├── CardManager.cpp  # 纸牌管理器
│   ├── CardManager.h
│   └── UndoManager.h    # 撤销管理器
├── configs/             # 配置加载
│   └── loaders/
└── services/            # 服务
    ├── CardIdManagerMap.h
    └── GameModelFromLevelGenerator.h
```

## 游戏玩法

1. 游戏开始时，会从 `Resources/level_1.json` 加载初始纸牌布局
2. 通过点击或拖动纸牌进行操作
3. 根据游戏规则，将纸牌按照特定顺序排列
4. 使用撤销功能可以回退上一步操作

## 扩展指南

### 添加新关卡

1. 在 `Resources/` 目录下创建新的 JSON 文件（如 `level_2.json`）
2. 按照 `level_1.json` 的格式定义新关卡的纸牌布局和规则
3. 在代码中加载新的关卡文件

### 自定义纸牌样式

1. 替换 `Resources/res/` 目录下的纸牌图像资源
2. 修改 `CardView.cpp` 中的渲染逻辑以适应新的图像尺寸和样式

### 添加新功能

1. 创建新的模型类（如果需要）
2. 实现相应的视图和控制器
3. 在 `GameController` 中集成新功能

## 许可证

本项目采用 MIT 许可证。详情请见 LICENSE 文件。

