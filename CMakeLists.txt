# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.5)

# 设置应用程序名称
set(APP_NAME card_game)

# 声明项目
project(${APP_NAME})

# 设置 Cocos2d-x 根目录路径
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
# 设置 CMake 模块路径
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

# 包含 Cocos2d-x 构建设置
include(CocosBuildSet)
# 如果不使用预构建的 Cocos2d-x 库，则构建 Cocos2d-x
if(NOT USE_COCOS_PREBUILT)
    # 添加 Cocos2d-x 核心库子目录
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# 记录源文件、头文件和资源文件...
# 初始化游戏源文件列表
set(GAME_SOURCE)
# 初始化游戏头文件列表
set(GAME_HEADER)

# 设置游戏资源文件夹路径
set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"  # 主资源目录
    )
# 对于苹果和 Windows 平台，标记资源文件
if(APPLE OR WINDOWS)
    # 标记多个资源文件，用于后续复制
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# 设置包含目录
include_directories(
        Classes  # 游戏源代码目录
        ${COCOS2DX_ROOT_PATH}/cocos/audio/include/  # Cocos2d-x 音频头文件目录
)
# 添加跨平台的源文件和头文件 
# 添加游戏源文件
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp  # 应用程序委托类
     Classes/HelloWorldScene.cpp  # 主场景类
     )
# 添加游戏头文件
list(APPEND GAME_HEADER
     Classes/AppDelegate.h  # 应用程序委托类头文件
     Classes/HelloWorldScene.h  # 主场景类头文件
     )

# Android 平台特定配置
if(ANDROID)
    # 更改 Android 平台的应用名称（共享库名称），该值应与 AndroidManifest.xml 匹配
    set(APP_NAME MyGame)
    # 添加 Android 特定源文件
    # 添加 Linux 特定源文件
    # 添加 Windows 特定源文件
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp  # Android 入口文件
              )
# Linux 平台特定配置
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp  # Linux 入口文件
         )
# Windows 平台特定配置
# Windows 平台特定设置
elseif(WINDOWS)
    # 添加 Windows 特定头文件
    list(APPEND GAME_HEADER
         proj.win32/main.h  # Windows 主头文件
         proj.win32/resource.h  # Windows 资源头文件
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp  # Windows 入口文件
         proj.win32/game.rc  # Windows 资源文件
         ${common_res_files}  # Windows 资源文件
         )
# Apple 平台（iOS/macOS）特定配置
elseif(APPLE)
if(APPLE)
    # iOS 平台特定配置
    if(IOS)
        list(APPEND GAME_HEADER
                  proj.ios_mac/ios/AppController.h  # iOS 应用控制器头文件
                  proj.ios_mac/ios/RootViewController.h  # iOS 根视图控制器头文件
             )
# 设置 iOS UI 资源
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard  # iOS 启动屏幕
            proj.ios_mac/ios/LaunchScreenBackground.png  # iOS 启动背景图
            proj.ios_mac/ios/Images.xcassets  # iOS 图像资源
            )
        # 添加 iOS 特定源文件

        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m  # iOS 入口文件
             proj.ios_mac/ios/AppController.mm  # iOS 应用控制器实现
             proj.ios_mac/ios/RootViewController.mm  # iOS 根视图控制器实现
             proj.ios_mac/ios/Prefix.pch  # iOS 预编译头文件
              ${APP_UI_RES}  # iOS UI 资源
             )
    # macOS 平台特定配置
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns  # macOS 图标
            proj.ios_mac/mac/Info.plist  # macOS 信息属性列表
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp  # macOS 入口文件
             proj.ios_mac/mac/Prefix.pch  # macOS 预编译头文件
             ${APP_UI_RES}
             )
    endif()
    # 添加苹果平台资源文件
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# 标记应用编译信息和库信息
# 设置应用源文件列表
set(APP_SRC
    ${GAME_SOURCE}  # 游戏源文件
    ${GAME_HEADER}  # 游戏头文件
    )
# 构建应用
cocos_build_app(${APP_NAME}
                APP_SRC "${APP_SRC}"  # 应用源文件
                DEPEND_COMMON_LIBS "cocos2d"  # 依赖的公共库
                DEPEND_ANDROID_LIBS "cocos2d_android"  # Android 平台依赖库
                )

if(APPLE)
    # 设置目标属性，添加资源
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    # macOS 特定设置
    if(MACOSX)
        # 设置 macOS 捆绑属性
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"  # macOS 信息属性列表路径
                              )
    # iOS 特定设置
    elseif(IOS)
        # 打包 Xcode 项目
          cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        # 设置 Xcode 属性：应用图标名称
          set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)
    # 复制目标 DLL 到二进制目录
    cocos_copy_target_dll(${APP_NAME} COPY_TO ${APP_BIN_DIR})
endif()

# Linux 和 Windows 平台特定设置
if(LINUX OR WINDOWS)
    # 复制目标资源到资源目录
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
