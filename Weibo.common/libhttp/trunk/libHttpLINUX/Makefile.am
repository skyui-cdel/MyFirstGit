#设置库名称
lib_LIBRARIES=libhttp.a

SOURCEDIR=$(top_srcdir)/libhttp/trunk/src

#要copy到安装头文件路径下面的文件
INSTALLHEADERS=\
			loHttp+.h\
			loHttp.h\
			loHttpDef.h

h_source= \
	$(SOURCEDIR)/bbiconv.h\
	$(SOURCEDIR)/loBytebuffer.h\
	$(SOURCEDIR)/loFile.h\
	$(SOURCEDIR)/loHttp.h\
	$(SOURCEDIR)/loHttp+.h\
	$(SOURCEDIR)/loHttpAssoc.h\
	$(SOURCEDIR)/loHttpCurl.h\
	$(SOURCEDIR)/loHttpDef.h\
	$(SOURCEDIR)/loHttpMap.h\
	$(SOURCEDIR)/loHttpStr.h\
	$(SOURCEDIR)/loHttpStruct.h\
	$(SOURCEDIR)/lostrconv.h\
	$(SOURCEDIR)/loUrlcode.h\
	$(SOURCEDIR)/loHttpThread.h
	
cpp_sources= \
	$(SOURCEDIR)/bbiconv.cc\
	$(SOURCEDIR)/loBytebuffer.cc\
	$(SOURCEDIR)/loFile.cpp\
	$(SOURCEDIR)/loHttp.cpp\
	$(SOURCEDIR)/loHttp+.cpp\
	$(SOURCEDIR)/loHttpAssoc.cpp\
	$(SOURCEDIR)/loHttpCurl.cpp\
	$(SOURCEDIR)/loHttpStr.cpp\
	$(SOURCEDIR)/lostrconv.cpp\
	$(SOURCEDIR)/loUrlcode.cpp\
	$(SOURCEDIR)/loHttpThread.cpp



#关联的头文件
INCLUDES=-I../include -I$(top_srcdir)/include/curl

# 设置头文件安装路径
includedir=$(prefix)/include/libhttp
jsonincludedir=$(includedir)


#库文件安装路径
libdir=$(prefix)/lib/LINUX


#使用的源文件
libhttp_a_SOURCES=$(h_sources) $(cpp_sources)

#要copy到安装头文件路径下面的文件
jsoninclude_HEADERS=$(INSTALLHEADERS)


AM_CXXFLAGS=-D_LINUX -DONLY_EPOLL -D_SERVER