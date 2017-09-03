# QtSudoku

By 计62 张子薇 2016011276

## 简介

基于Qt的数独游戏

## 功能清单

#### 基础功能

-   数独基本选中填数功能
-   选中数字行列高亮
-   点击数字高亮界面所有相同数字
-   游戏计时
-   重玩，暂停。暂停后切换到暂停界面，计时暂停
-   10个不同难度Level选择
-   撤销恢复功能（10步）
-   删除功能
-   方格内填多个数字
-   游戏结束判定
-   结束页面及通关信息

#### 进阶功能

-   不同难度数独问题随机生成，均只有一组解
-   数独求解器及求解页面，对于不标准数独返回最小的解
-   游戏界面按钮音效
-   游戏提示，修改一个错误数字或在方格中填入一个数字

## 项目介绍

#### 项目目录

    	.
    	├── bgm                   //音效文件
    	│   ├── click.wav
    	│   └── operate.wav
    	├── button.cpp            //重载QLabel，增加类似QPushButton属性
    	├── button.h
		├── doc                   //PDF文档
    	│   └── README.pdf
    	├── main.cpp
    	├── mainwindow.cpp
    	├── mainwindow.h
    	├── mainwindow.ui
    	├── README.md
    	├── resource.qrc          //资源文件
    	├── subwidget.cpp         //子窗口
    	├── subwidget.h
    	├── sudoku.cpp            //游戏逻辑，负责数独的产生，判定等
    	├── sudoku.h
    	├── Sudoku.pro
    	├── Sudoku.pro.user
    	├── undo.cpp              //撤销&恢复，重载QUndoCommand
    	└── undo.h

#### 界面设计

整个游戏界面由5个界面组成，每个界面对应`subwidget.h & subwidget.cpp`下的一个类，最后在`MainWindow`下添加到`QStackedWidget`内。

##### HomeWidget

主页，负责游戏难度选择，游戏界面和求解界面的切换

> 继承自`Qwidget`

##### CalWidget

抽象类，分为`topBar`,`midBoard`,`bottomInputBar`3层，游戏主界面框架

> 继承自`Qwidget`

##### PlayWidget

游戏主界面

> 继承自`CalWidget`

##### SolveWidget

求解数独界面

> 继承自`CalWidget`

##### WinWidget

游戏结束界面，包含选择的难度和完成的时间

> 继承自`QWidget`

##### PauseWidget

暂停界面，阻止用户在暂停期间继续思考数独

> 继承自`QWidget`

#### 数独设计

##### 求解器

暴力求解……

##### 生成器

先合法随机填上一些数字(据说11个效率比较高)，然后用求解器求得一个终盘。再随机去掉数字，去掉过程中保证整个数独只有一个解

##### 难度设计

依赖于挖空多少
