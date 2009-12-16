		K.L.'s MagicCube


------------------------------
2008.2.20b, beta0.5.1

	Document supplement.
	补充文档。


------------------------------
2008.2.20a, beta0.5

	Implement python script API.
	实现python脚本扩展接口。

	Multi-thread process scripts computation.
	多线程执行脚本。

	Add some interface configurations.
	添加界面配置项。


------------------------------
2008.2.4a, beta0.4

	Add lua script API.
	添加了lua脚本扩展接口。脚本开发参见MagicCube脚本开发向导：Scripts Guide.txt


------------------------------
2008.1.27a, beta0.3

	Add playback recording/playing.
	添加录像回放功能。


------------------------------
2008.1.24a, beta0.2

	Add snapshot with series code. You can reload the snapshot state.
	添加快照功能，快照保存的状态可以被重新读取。

	Add manipulatation history, i.e. undo/redo.
	添加魔方操作历史，可以撤销/重复操作。

	Add cube map effect. 8 skyboxes for choice.
	添加环境映射效果，8种可供选择的天空体。


------------------------------
2008.1.19b, beta0.1.1

	Add d3d runtime library d3dx9_34.dll.



------------------------------
2008.1.19a, beta0.1	预览版

	Specular material on cube face	
	魔方表面镜面反射材质

	Basic operation implemented: cursor sense, drag and wheel manipulatation
	已实现基本操作（光标感应，拖拽、滚轮操作）



------------------------------

Recommended Environment: Windows XP, video memory 32Mb at least, mouse with wheel.
建议系统配置：Windows XP，32M以上显存显卡，带滚轮鼠标。

No installation, just run 'MagicCube.exe'. At beginning, it may prompt "Application Configuration Error", this is because program depends VC8.0 runtime libraries, please run 'VCRedist_x86.exe' to install VC Redistribution(provided by Microsoft website, I have put it into K.L.'s MagicCube binary package, or you can dowload it from: http://www.microsoft.com/downloads/details.aspx?FamilyId=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=en).
无需安装，运行MagicCube.exe即可。普通机器可能会提示“应用程序配置不正确”，这是因为程序模块使用VC8.0编译，需要安装VC8.0运行时库，请运行VCRedist_x86.exe（微软官方网站提供的VC Redistribution程序，K.L.'s MagicCube发布包中已带，或者可以从这里下载：http://www.microsoft.com/downloads/details.aspx?FamilyId=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=en）

The first running, it will popup OGRE configuration dialog. I suggest to choose 'Full Screen: No' and 'Video Mode: 800 x 600' or '640 x 480'. Then the config will be saved as default for future running. If you want to change config, delete file 'ogre.cfg' in work path.
第一次运行时会弹出OGRE的配置对话框，建议选择非全屏(full screen: No)，分辨率采用800x600或640x480。第二次运行以后将采用默认配置，若要重新选择配置可删除工作目录下的文件ogre.cfg。

Operation Instruct: use Left mouse button to drag cube face or roll wheel to screw one face of cube, hold middle mouse button or left button in background space to rotate cube integrally, roll wheel in background space to zoom cube, press [SHUT] or Alt+F4 to quit.
操作方法：在魔方表面使用鼠标左键拖拽或滚轮旋转来转动魔方，按住中键或在背景处按住左键移动鼠标整体旋转魔方，在背景处用滚轮缩放，SHUT按钮或Alt+F4退出。

While playing playback or scripts, cube doesn't accept manipulation, press Esc or player close button([X]) to quit playing state.
播放录像/脚本时魔方不接受手动控制，按ESC或播放器关闭按钮退出播放状态。

Scripts Extend Interface
脚本扩展接口

You can control cube by writing Lua or Python scripts. See details in 'Scripts Guide.txt'.
可以使用lua和python来编写脚本来控制魔方操作，参见MagicCube脚本开发向导：Scripts Guide.txt。


Implementation. I used several open-source software, include:
关于实现，使用了若干开源软件，包括：

3D Graphics & GUI: OGRE(http://www.ogre3d.org/), CEGUI(http://www.cegui.org.uk/).

Scripts Language: lua(http://www.lua.org/), luabind(http://luabind.sourceforge.net/), python(http://www.python.org/).

Others: boost(http://www.boost.org/).



Welcome to K.L.'s MagicCube web groups: http://groups.google.com/group/k_l_s-magiccube
欢迎访问K.L.'s MagicCube专用论坛：http://groups.google.com/group/k_l_s-magiccube

Advise, Issues Discussion and Bug Submission: you could submit to web groups.
建议、问题讨论和bug提交可在该论坛中发帖。

Please explain the behave and emersion method when submit bugs. When encountered runtime error, please send 'ogre.log' to <xxxK.L.xxx@gmail.com> for error diagnose.
提交bug时请说明bug现象和重现方法，出现运行时错误可将日志文件(ogre.log)发到xxxK.L.xxx@gmail.com以帮助诊断错误原因。


Have fun!


------------------------------
魔方的简介
(a Brief Introduction for Rubik's Cube)


魔方

魔方，Rubik's Cube 又叫魔术方块，是匈牙利布达佩斯建筑学院鲁比克教授在1974年发明的。当初他发明魔方，仅仅是作为一种帮助学生增强空间思维能力的教学工具。但要使那些小方块可以随意转动而不散开，不仅是个机械难题，这牵涉到木制的轴心，座和榫头等。直到魔方在手时，他将魔方转了几下后，才发现如何把混乱的颜色方块复原竟是个有趣而且困难的问题。鲁毕克就决心大量生产这种玩具。魔方发明后不久就风靡世界，人们发现这个小方块组成的玩意实在是奥妙无穷。


魔方的构成

三阶魔方由一个连接着六个中心块的中心轴以及结构不一的20个方块构成，当它们连接在一起的时候会形成一个整体，并且任何一面都可水平转动而不影响到其他方块。 

四阶魔方相对于三阶来说就要复杂的多，它的构成分为两类，一类中心是一个球体，每个外围的小块连接着中心球的滑轨，在运动时候会沿着用力方向在滑轨上滑动。第二类是以轴为核心的四阶魔方，这类魔方的构成非常复杂，除了中心球和外围块外还有很多附加件。作为竞速运动来说第二种构成的四阶魔方运动速度快，不易在高速转动中卡住。 

五阶魔方的构成则更甚于四阶魔方。每发明一种新的高阶魔方都要经过很长时间，因为不仅要考虑到项目的可行性，还要考虑如果将魔方作出来后能不能稳定的用于转动。正是由于这个原因，五阶魔方是官方公布的最高阶魔方，其结构也不是一般的爱好者可以想象出来的。 

六阶（含）以上的魔方目前还没有官方版本。不过不久前传言说已经有日本人制作出来样品并且发送法国公司尝试生产了。高阶魔方之所以难以制作，因为如果还是按照三阶魔方同比增加和扩大的话，角块在转动中可能会因无支撑物而从空中掉落。以前在网上所看到过的所谓八阶魔方后来证明不过是用普通的三阶拼凑出来的。


魔方有多少种变化？

别看魔方只有26个小块，相对位置能变化的更只有20块，可是它的变化真不少，曾经有公司在卖魔方时做广告说魔方总变化有30亿种，有人评论说，这相当于麦当劳得意的宣布他们已经在全世界卖出了100个汉堡包了。
魔方的变化有(8!*3^8*12!*2^12)/(3*2*2)=43,252,003,274,489,856,000，约等于4.3*10^19或者4325亿亿种变化，三阶魔方总变化数的道理是这样：8个角色块全排列8!，而每个角色块又有3种朝向，所以是8!*3^8，12个棱色块全排列每个有2种朝向是12!*2^12，这样相乘就是分子，而分母上3*2*2的意义是，保持其他色块不动，不可以单独改变一个角色块朝向，改变一个棱色块朝向，和单独交换一对色块的位置。
