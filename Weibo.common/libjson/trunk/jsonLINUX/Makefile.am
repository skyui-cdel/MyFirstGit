#
lib_LIBRARIES = libjson.a

SOURCEDIR=$(top_srcdir)/libjson/trunk/src
INCLUDEDIR=$(top_srcdir)/libjson/trunk/include/json

#要copy到安装头文件路径下面的文件
INSTALLHEADERS=\
			$(INCLUDEDIR)/autolink.h\
			$(INCLUDEDIR)/forwards.h\
			$(INCLUDEDIR)/reader.h\
			$(INCLUDEDIR)/writer.h\
			$(INCLUDEDIR)/config.h\
			$(INCLUDEDIR)/json.h\
			$(INCLUDEDIR)/value.h

h_source=\
	$(SOURCEDIR)/json_batchallocator.h
		

cpp_source=\
	$(SOURCEDIR)/json_reader.cpp\
	$(SOURCEDIR)/json_writer.cpp\
	$(SOURCEDIR)/json_value.cpp
	

#关联的头文件
INCLUDES = -I../include/json


#使用的源文件
libjson_a_SOURCES = $(h_sources) $(cpp_sources)



# 设置头文件安装路径
includedir=$(prefix)/include/json
jsonincludedir=$(includedir)

#库文件安装路径
libdir=$(prefix)/lib/LINUX

#要copy到安装头文件路径下面的文件
jsoninclude_HEADERS=$(INSTALLHEADERS)


AM_CXXFLAGS=-D_LINUX -DONLY_EPOLL -D_SERVER