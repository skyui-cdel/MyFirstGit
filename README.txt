SAE(Sina App Engine)，Weibo SDK for C/C++

支持环境：
	WINXP WIN7 (VS2005 ,VS2008)


作者：翁立波(welbon)

日期：2010-12-23

=========================================================================================

工程配置:

WINDOWS平台：

一共分为两个工程组
Weibo.common
Weibo.sdk

Weibo.common
	libhttp - 实现基本的HTTP请求封装，依赖库：libcurl-7.21.1.
	libjson - 修正了中文的JSON解析问题,可以用该库来解析WEIBO SDK返回的JSON格式

Weibo.SDK
	Weibo 实现了微博开放平台基本的业务操作，具体操作流程请参考test工程，依赖库:libhttp
	test 微博操作例程。依赖库：libWeibo
	
目前只提供了VS2005与VS2008两种编译方式，

编译选项列表如下:

DLL Debug
DLL Release

DLL DebugMT
DLL ReleseMT

LIB Debug
LIB Release

LIB DebugMT
LIB ReleaseMT

说明：
DLL **  ,以动态链接的方式加载库，以动态链接的方式加载MSVCR库
DLL **MT,以动态链接的方式加载库，以静态链接的方式加载MSVCR库
LIB **  ,以静态链接的方式加载库，以动态链接的方式加载MSVCR库
LIB **MT,以静态链接的方式加载库，以静态链接的方式加载MSVCR库

默认调试请选用DLL Debug。

=========================================================================================
QA:

Q：为什么编译不过去?提示缺少一些库？
A：请检查库你的SVN下载的 \trunk\Weibo.sdk\third-party\的bin和lib下面库文件是完整的。
	如要确认，请用SVN浏览，或直接用IE访问.
	
Q：为什么编译后跑不起来？提示库加载错误？
A：可能是由于你的编译器版本过低引起的，
	请确保你的VS2005编译器环境版本号 >= 8.0.50727.867,Microsoft .NET Framework版本号 >= 2.0.50727(SP2)，
	VS2008版本号：
	若满足上述条件，则可能是由于编译环境不一致，请在common下面重新编译所需要的库，并将它拷贝到运行目录下


Q：weibo.sdk\third-party下面的库太多，下载起来太慢了。
A：这个下面关联的是weibo.common\下面的东西，若觉得下载速度太慢
	可以自行从weibo.common\拷贝到weibo.sdk\third-party
	
Q: VC6.0可以使用吗？
A: 逻辑上来讲是ok的.需要自己配置一下环境。：),配好环境的同学可以直接将配置好的发给我.

Q: 库下载不下来，GOOGLE CODE 有问题,提示该链接被关闭。
A: 为了防止下载不下来的问题，提供了一个拷贝批处理，只需要运行一下就可以将COMMON下面的库拷贝到SDK运行目录下，
	或者需要进行本地代码测试的同学也可以方便拷贝


=========================================================================================
test 如何使用：

// 设置登录方式 0 oauth,1 cookie
Please enter request method[0 = oauth/1 = cookie]: 
// 设置 APP KEY
Please enter APP KEY:

// 设置APP SECRET
Please enter APP SECRET:

// 设置返回的数据类型:xml/json
Please enter result data format(josn=0/xml=1):

// 设置Ouath 方式如何获取验证码 0:网页获取， 1. 直接获取 ，输入用户名密码
Please enter how to get verify code[0 = website,1 = direct access] :

// 循环获取输入数字 ，具体的请参考 Weibo SDK 头文件 ： "weibo.h" 中的枚举类型WEIBOoption 
//  例如 2 对应 WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE)
// 输入 q 按回车退出程序
Please enter TEST number[exit:'q']: 

=========================================================================================

由于C++ SDK 维护的时间比较紧,支持上可能还有很多问题。

大家或者有什么好的意见或建议，直接提出来，大家共同学习完善。

：）

=========================================================================================


更多问题请联系作者：

翁立波(welbon)
	Gtalk/Gmail: libo.weng@gmail.com 
	UC号:11454 (下载地址:http://uc.sina.com.cn)

胡秋云(loach.hu)
	Mail: loachmr@sina.com
	UC:11050
 
=========================================================================================