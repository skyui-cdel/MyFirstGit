Sina App 应用支持库
版权所属：新浪网技术中国有限公司
支持环境：
	WINDOWS (VS2005 ,VS2008) 

日期：2010-12-23
作者：翁立波

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
	若满足上述条件，则可能是由于编译环境不一致，请在common下面重新编译所需要的库，并将它拷贝到运行目录下
	
Q：为什么有些库没有源代码？
A：考虑到版权问题，没有将第三方的开源库的源代码放上去，
	若需要重新编译，请自行下载对应的库
	
Q：weibo.sdk\third-party下面的库太多，下载起来太慢了。
A：这个下面关联的是weibo.common\下面的东西，若觉得下载速度太慢
	可以自行从weibo.common\拷贝到weibo.sdk\third-party


=========================================================================================
更多问题请联系作者：

翁立波(welbon)
	Gtalk/Gmail: libo.weng@gmail.com 
	UC号:11454 (下载地址:http://uc.sina.com.cn)

胡秋云(loach.hu)
	Mail: loachmr@sina.com
	UC:11050
 
