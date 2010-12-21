#设置库名称

lib_LIBRARIES = libWeiboSDK.a

SOURCEDIR=$(top_srcdir)/src

h_source = \
	$(SOURCEDIR)/wbCheck.h\
	$(SOURCEDIR)/wbLog.h\
	$(SOURCEDIR)/wbOption.h\
	$(SOURCEDIR)/wbRequest.h\
	$(SOURCEDIR)/wbSession.h\
	$(SOURCEDIR)/wbString.h\
	$(SOURCEDIR)/wbURL.h\
	$(SOURCEDIR)/weibo.h\
	$(SOURCEDIR)/common/strconv.h\
	$(SOURCEDIR)/common/Urlcode.h\
	$(SOURCEDIR)/common/bbiconv.h\
	$(SOURCEDIR)/oauth/oauth.h\
	$(SOURCEDIR)/oauth/xmalloc.h


	
cpp_sources = \
	$(SOURCEDIR)/wbCheck.cpp\
	$(SOURCEDIR)/wbOption.cpp\
	$(SOURCEDIR)/wbRequest.cpp\
	$(SOURCEDIR)/wbSession.cpp\
	$(SOURCEDIR)/wbString.cpp\
	$(SOURCEDIR)/wbURL.cpp\
	$(SOURCEDIR)/weibo.cpp\
	$(SOURCEDIR)/common/strconv.cpp\
	$(SOURCEDIR)/common/Urlcode.cpp\
	$(SOURCEDIR)/common/bbiconv.cc\
	$(SOURCEDIR)/oauth/oauth.c\
	$(SOURCEDIR)/oauth/hash.c\
	$(SOURCEDIR)/oauth/xmalloc.c


#关联的头文件
INCLUDES = -I$(top_srcdir)/include

#关联的库

#使用的源文件
libWeiboSDK_a_SOURCES = $(h_sources) $(cpp_sources)

# 设置安装路径
includedir=$(prefix)/include/libweibo
libdir=$(prefix)/lib
WeiboSDKincludedir=$(includedir)
WeiboSDKinclude_HEADERS=$(SOURCEDIR)/weibo.h



AM_CXXFLAGS=-D_LINUX -DONLY_EPOLL -D_SERVER
