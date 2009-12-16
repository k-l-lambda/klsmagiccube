		K.L.'s MagicCube


------------------------------
2008.2.20b, beta0.5.1

	Document supplement.
	�����ĵ���


------------------------------
2008.2.20a, beta0.5

	Implement python script API.
	ʵ��python�ű���չ�ӿڡ�

	Multi-thread process scripts computation.
	���߳�ִ�нű���

	Add some interface configurations.
	��ӽ��������


------------------------------
2008.2.4a, beta0.4

	Add lua script API.
	�����lua�ű���չ�ӿڡ��ű������μ�MagicCube�ű������򵼣�Scripts Guide.txt


------------------------------
2008.1.27a, beta0.3

	Add playback recording/playing.
	���¼��طŹ��ܡ�


------------------------------
2008.1.24a, beta0.2

	Add snapshot with series code. You can reload the snapshot state.
	��ӿ��չ��ܣ����ձ����״̬���Ա����¶�ȡ��

	Add manipulatation history, i.e. undo/redo.
	���ħ��������ʷ�����Գ���/�ظ�������

	Add cube map effect. 8 skyboxes for choice.
	��ӻ���ӳ��Ч����8�ֿɹ�ѡ�������塣


------------------------------
2008.1.19b, beta0.1.1

	Add d3d runtime library d3dx9_34.dll.



------------------------------
2008.1.19a, beta0.1	Ԥ����

	Specular material on cube face	
	ħ�����澵�淴�����

	Basic operation implemented: cursor sense, drag and wheel manipulatation
	��ʵ�ֻ�������������Ӧ����ק�����ֲ�����



------------------------------

Recommended Environment: Windows XP, video memory 32Mb at least, mouse with wheel.
����ϵͳ���ã�Windows XP��32M�����Դ��Կ�����������ꡣ

No installation, just run 'MagicCube.exe'. At beginning, it may prompt "Application Configuration Error", this is because program depends VC8.0 runtime libraries, please run 'VCRedist_x86.exe' to install VC Redistribution(provided by Microsoft website, I have put it into K.L.'s MagicCube binary package, or you can dowload it from: http://www.microsoft.com/downloads/details.aspx?FamilyId=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=en).
���谲װ������MagicCube.exe���ɡ���ͨ�������ܻ���ʾ��Ӧ�ó������ò���ȷ����������Ϊ����ģ��ʹ��VC8.0���룬��Ҫ��װVC8.0����ʱ�⣬������VCRedist_x86.exe��΢��ٷ���վ�ṩ��VC Redistribution����K.L.'s MagicCube���������Ѵ������߿��Դ��������أ�http://www.microsoft.com/downloads/details.aspx?FamilyId=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=en��

The first running, it will popup OGRE configuration dialog. I suggest to choose 'Full Screen: No' and 'Video Mode: 800 x 600' or '640 x 480'. Then the config will be saved as default for future running. If you want to change config, delete file 'ogre.cfg' in work path.
��һ������ʱ�ᵯ��OGRE�����öԻ��򣬽���ѡ���ȫ��(full screen: No)���ֱ��ʲ���800x600��640x480���ڶ��������Ժ󽫲���Ĭ�����ã���Ҫ����ѡ�����ÿ�ɾ������Ŀ¼�µ��ļ�ogre.cfg��

Operation Instruct: use Left mouse button to drag cube face or roll wheel to screw one face of cube, hold middle mouse button or left button in background space to rotate cube integrally, roll wheel in background space to zoom cube, press [SHUT] or Alt+F4 to quit.
������������ħ������ʹ����������ק�������ת��ת��ħ������ס�м����ڱ�������ס����ƶ����������תħ�����ڱ������ù������ţ�SHUT��ť��Alt+F4�˳���

While playing playback or scripts, cube doesn't accept manipulation, press Esc or player close button([X]) to quit playing state.
����¼��/�ű�ʱħ���������ֶ����ƣ���ESC�򲥷����رհ�ť�˳�����״̬��

Scripts Extend Interface
�ű���չ�ӿ�

You can control cube by writing Lua or Python scripts. See details in 'Scripts Guide.txt'.
����ʹ��lua��python����д�ű�������ħ���������μ�MagicCube�ű������򵼣�Scripts Guide.txt��


Implementation. I used several open-source software, include:
����ʵ�֣�ʹ�������ɿ�Դ�����������

3D Graphics & GUI: OGRE(http://www.ogre3d.org/), CEGUI(http://www.cegui.org.uk/).

Scripts Language: lua(http://www.lua.org/), luabind(http://luabind.sourceforge.net/), python(http://www.python.org/).

Others: boost(http://www.boost.org/).



Welcome to K.L.'s MagicCube web groups: http://groups.google.com/group/k_l_s-magiccube
��ӭ����K.L.'s MagicCubeר����̳��http://groups.google.com/group/k_l_s-magiccube

Advise, Issues Discussion and Bug Submission: you could submit to web groups.
���顢�������ۺ�bug�ύ���ڸ���̳�з�����

Please explain the behave and emersion method when submit bugs. When encountered runtime error, please send 'ogre.log' to <xxxK.L.xxx@gmail.com> for error diagnose.
�ύbugʱ��˵��bug��������ַ�������������ʱ����ɽ���־�ļ�(ogre.log)����xxxK.L.xxx@gmail.com�԰�����ϴ���ԭ��


Have fun!


------------------------------
ħ���ļ��
(a Brief Introduction for Rubik's Cube)


ħ��

ħ����Rubik's Cube �ֽ�ħ�����飬��������������˹����ѧԺ³�ȿ˽�����1974�귢���ġ�����������ħ������������Ϊһ�ְ���ѧ����ǿ�ռ�˼ά�����Ľ�ѧ���ߡ���Ҫʹ��ЩС�����������ת������ɢ���������Ǹ���е���⣬��ǣ�浽ľ�Ƶ����ģ������ͷ�ȡ�ֱ��ħ������ʱ������ħ��ת�˼��º󣬲ŷ�����ΰѻ��ҵ���ɫ���鸴ԭ���Ǹ���Ȥ�������ѵ����⡣³�Ͽ˾;��Ĵ�������������ߡ�ħ�������󲻾þͷ������磬���Ƿ������С������ɵ�����ʵ���ǰ������


ħ���Ĺ���

����ħ����һ���������������Ŀ���������Լ��ṹ��һ��20�����鹹�ɣ�������������һ���ʱ����γ�һ�����壬�����κ�һ�涼��ˮƽת������Ӱ�쵽�������顣 

�Ľ�ħ�������������˵��Ҫ���ӵĶ࣬���Ĺ��ɷ�Ϊ���࣬һ��������һ�����壬ÿ����Χ��С��������������Ļ��죬���˶�ʱ����������������ڻ����ϻ������ڶ���������Ϊ���ĵ��Ľ�ħ��������ħ���Ĺ��ɷǳ����ӣ��������������Χ���⻹�кܶ฽�Ӽ�����Ϊ�����˶���˵�ڶ��ֹ��ɵ��Ľ�ħ���˶��ٶȿ죬�����ڸ���ת���п�ס�� 

���ħ���Ĺ�����������Ľ�ħ����ÿ����һ���µĸ߽�ħ����Ҫ�����ܳ�ʱ�䣬��Ϊ����Ҫ���ǵ���Ŀ�Ŀ����ԣ���Ҫ���������ħ�����������ܲ����ȶ�������ת���������������ԭ�����ħ���ǹٷ���������߽�ħ������ṹҲ����һ��İ����߿�����������ġ� 

���ף��������ϵ�ħ��Ŀǰ��û�йٷ��汾����������ǰ����˵�Ѿ����ձ�������������Ʒ���ҷ��ͷ�����˾���������ˡ��߽�ħ��֮����������������Ϊ������ǰ�������ħ��ͬ�����Ӻ�����Ļ����ǿ���ת���п��ܻ�����֧������ӿ��е��䡣��ǰ������������������ν�˽�ħ������֤������������ͨ������ƴ�ճ����ġ�


ħ���ж����ֱ仯��

��ħ��ֻ��26��С�飬���λ���ܱ仯�ĸ�ֻ��20�飬�������ı仯�治�٣������й�˾����ħ��ʱ�����˵ħ���ܱ仯��30���֣���������˵�����൱�����͵�������������Ѿ���ȫ����������100���������ˡ�
ħ���ı仯��(8!*3^8*12!*2^12)/(3*2*2)=43,252,003,274,489,856,000��Լ����4.3*10^19����4325�����ֱ仯������ħ���ܱ仯���ĵ�����������8����ɫ��ȫ����8!����ÿ����ɫ������3�ֳ���������8!*3^8��12����ɫ��ȫ����ÿ����2�ֳ�����12!*2^12��������˾��Ƿ��ӣ�����ĸ��3*2*2�������ǣ���������ɫ�鲻���������Ե����ı�һ����ɫ�鳯�򣬸ı�һ����ɫ�鳯�򣬺͵�������һ��ɫ���λ�á�
