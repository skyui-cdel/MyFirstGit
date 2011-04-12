// testhttp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libhttp/lohttp+.h>
#include <stdlib.h>
#include <Json/Json.h>


void testJson()
{
	FILE* fp = fopen("e:\\eero.txt" , "rb");
	fseek(fp , 0 , SEEK_END);
	long lfilelen = ftell(fp);
	if( lfilelen <= 0 ){
		fclose(fp);
		return ;
	}
	fseek(fp , 0 , SEEK_SET);
	char* buffer = new char[lfilelen+1];
	if( NULL == buffer ){
		fclose(fp);
		return ;
	}
	buffer[ lfilelen ] = '\0';

	if( lfilelen != fread(buffer, sizeof(char), lfilelen , fp) )
	{
		fclose(fp);
		return ;
	}
	fclose(fp);

	Json::Reader reader;
	Json::Value  pRoot;
	if( !reader.parse(buffer ,pRoot,false) )
	{
		std::string str = reader.getFormatedErrorMessages();

		int i=0;
		i++;
	}
	delete[] buffer;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testJson();

	lohttp::LOHttp* pHttp =  lohttp::Http_createobject();


	lohttp::Http_seturl(pHttp , HTTP_T("http://sina.com.cn") );

#if 1
	// 异步
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod ,lohttp::E_HTTP_GET_METHOD );
	lohttp::Http_start_asyn( pHttp );
	lohttp::Http_wait( pHttp );


	// 下载到文件
    lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod ,lohttp::E_HTTP_DOWNLOAD_METHOD );
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setfile , HTTP_T("C:\\Users\\Loach\\Documents\\") , HTTP_T("sina.html") );
	lohttp::Http_start_asyn( pHttp );
	lohttp::Http_wait( pHttp );
#else
	// 同步
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod ,lohttp::E_HTTP_GET_METHOD );
    lohttp::Http_start( pHttp );

	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod ,lohttp::E_HTTP_DOWNLOAD_METHOD );
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setfile , HTTP_T("C:\\Users\\Loach\\Documents\\") , HTTP_T("sina.html") );
    lohttp::Http_start( pHttp );
#endif


	lohttp::Http_seturl(pHttp, "http://avatar.uc.sina.com.cn/sys/AnimalEx_3.png");
	lohttp::Http_setopt(pHttp, lohttp::LOHTTP_OPT_setmethod, lohttp::E_HTTP_DOWNLOAD_METHOD);
	lohttp::Http_setopt(pHttp, lohttp::LOHTTP_OPT_setfile,"D:\\1223\\","111.PNG" );
	lohttp::Http_start(pHttp);

	// 消毁
	lohttp::Http_destroyobject( pHttp );

	char * p =new char[35];
	system("pause");
	return 0;
}

