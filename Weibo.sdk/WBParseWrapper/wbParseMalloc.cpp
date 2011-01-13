/**
*  @brief memory malloc/free ,
*  @file  WeiboParseMemop.cpp
*  @author welbon 
*  @Email < libo.weng@gmail.com >
*
* Copyright (C) 1996-2010 SINA Corporation, All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#include "stdafx.h"
#include "wbParseStruct.h"
#include "wbParseMalloc.h"
#include <stdlib.h>

#define PARSE_STRUCT_MEMOP

#define PARSE_STRUCT_MALLOC(p,size) \
	p = malloc(size);\
	memset(p,0,size);\
	((t_wbParse_Header*)p)->eSource = wbParserNS::EPS_OPENAPI;\


#define PARSE_STRUCT_MALLOC_NORMAL(p,size)\
	p = malloc(size);\
	memset(p,0,size);\

#define PARSE_STRUCT_FREE(p) p ? free(p) : 0;


namespace wbParserNS
{
	EXTERN void *wbParse_Malloc_Cursor(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Cursor)*size);
#else
		void *p = malloc(sizeof(t_wbParse_Cursor)*size);
		memset(p,0,sizeof(t_wbParse_Cursor)*size);
#endif //PARSE_STRUCT_MEMOP

		return p;
	}
	EXTERN void wbParse_Free_Cursor(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//用户
	EXTERN void *wbParse_Malloc_User(int size)
	{
#ifdef PARSE_STRUCT_MEMOP

		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_User)*size);
#else
		void *p = malloc(sizeof(t_wbParse_User)*size);
		memset(p,0,sizeof(t_wbParse_User)*size);
#endif //0
		return p;
	}

	EXTERN void wbParse_Free_User(void *pvoid,int size)
	{
		t_wbParse_User *p = (t_wbParse_User*)pvoid;
		if( p )
		{
			int i = 0;
			do
			{
				t_wbParse_User *item = (p+i);
				if( item ->last_status ) {
					wbParse_Free_Status(item ->last_status,1);
				}
				i++;
			}
			while(i<size);
			//
			PARSE_STRUCT_FREE(pvoid);
		}
	}

	//状态
	EXTERN void  *wbParse_Malloc_Status(int size)
	{
#ifdef PARSE_STRUCT_MEMOP

		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Status)*size);
#else
		void *p = malloc(sizeof(t_wbParse_Status)*size);
		memset(p,0,sizeof(t_wbParse_Status)*size);

#endif //PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void wbParse_Free_Status(void  *pvoid,int size)
	{
		t_wbParse_Status *p = (t_wbParse_Status*)pvoid;
		if( !p )
			return ;

		int i = 0;
		do
		{
			t_wbParse_Status *item = (p+i);
			if( item->geo ) wbParse_Free_Geo(item->geo,1);
			if( item->user) wbParse_Free_User(item->user,1);
			if( item->retweeted_status ) wbParse_Free_Status(item->retweeted_status,1);
			i++;
		} 
		while( i < size );

		PARSE_STRUCT_FREE(pvoid);
	} 

	//评论
	EXTERN void  *wbParse_Malloc_Comment(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Comment)*size);
#else
		void* p = malloc(sizeof(t_wbParse_Comment)*size);
		memset(p,0,sizeof(t_wbParse_Comment)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}

	EXTERN void wbParse_Free_Comment(void  *pvoid,int size)
	{
		t_wbParse_Comment *p = (t_wbParse_Comment*)pvoid;
		if( !p )
			return ;

		int i = 0;
		do
		{
			t_wbParse_Comment *item = (p+i);
			if( item->user)   wbParse_Free_User(item->user,1);
			if( item->status) wbParse_Free_Status(item->status,1);
			if( item->reply_comment) wbParse_Free_Comment(item->reply_comment,1);
			i++;
		}while(i<size);

		PARSE_STRUCT_FREE(pvoid);
	}

	//评论数
	EXTERN void *wbParse_Malloc_Commentcount(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_CommentCounts)*size);
#else
		void* p = malloc(sizeof(t_wbParse_CommentCounts)*size);
		memset(p,0,sizeof(t_wbParse_CommentCounts)*size);
#endif //PARSE_STRUCT_MEMOP


		return p;
	}
	EXTERN void wbParse_Free_Commentcount(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//未读数
	EXTERN void  *wbParse_Malloc_Unread(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Unread)*size);
#else
		void* p = malloc(sizeof(t_wbParse_Unread)*size);
		memset(p,0,sizeof(t_wbParse_Unread)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void wbParse_Free_Unread(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//地理位置
	EXTERN void *wbParse_Malloc_Geo(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Geo)*size);
#else
		void *p = malloc(sizeof(t_wbParse_Geo)*size);
		memset(p,0,sizeof(t_wbParse_Geo)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void wbParse_Free_Geo(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 私信
	EXTERN void *wbParse_Malloc_Directmessage(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_DirectMessage)*size);
#else
		void *p = malloc(sizeof(t_wbParse_DirectMessage)*size);
		memset(p,0,sizeof(t_wbParse_DirectMessage)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void wbParse_Free_Directmessage(void *pvoid,int size)
	{
		t_wbParse_DirectMessage *p = (t_wbParse_DirectMessage*)pvoid;
		if( !p )
			return ;

		int i = 0;
		do
		{
			t_wbParse_DirectMessage *item = (p+i);
			if(item->sender )   wbParse_Free_User(item->sender,1);
			if(item->recipient) wbParse_Free_User(item->recipient,1);
			i++;
		}while(i < size );
		//
		PARSE_STRUCT_FREE(pvoid);
	}

	// 好友关系
	EXTERN void *wbParse_Malloc_Friendship(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Shipshow)*size);
#else
		void *p = malloc(sizeof(t_wbParse_Shipshow)*size);
		memset(p,0,sizeof(t_wbParse_Shipshow)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void  wbParse_Free_Friendship(void *pvoid,int size)
	{
		t_wbParse_Shipshow *p = (t_wbParse_Shipshow*)pvoid;
		if( !p )
			return ;

		int i = 0;
		do
		{
			t_wbParse_Shipshow *item = (p+i);
			if( item->source ) PARSE_STRUCT_FREE(item->source);
			if( item->target ) PARSE_STRUCT_FREE(item->target);
			i++;
		}while(i<size);
		PARSE_STRUCT_FREE(pvoid);
	}


	// ids
	EXTERN void *wbParse_Malloc_IDs(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_IDs)*size);
#else
		void *p = malloc(sizeof(t_wbParse_IDs)*size);
		memset(p,0,sizeof(t_wbParse_IDs)*size);
#endif //#ifdef PARSE_STRUCT_MEMOP
		return p;
	}
	//
	EXTERN void  wbParse_Free_IDs(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 访问限制
	EXTERN void *wbParse_Malloc_Limits(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_LimitStatus)*size);
#else
		void *p = malloc(sizeof(t_wbParse_LimitStatus)*size);
		memset(p,0,sizeof(t_wbParse_LimitStatus)*size);
#endif //PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void  wbParse_Free_Limits(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 表情
	EXTERN void *wbParse_Malloc_Emotion(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Emotion)*size);
#else
		void *p = malloc(sizeof(t_wbParse_Emotion)*size);
		memset(p,0,sizeof(t_wbParse_Emotion)*size);
#endif //PARSE_STRUCT_MEMOP
		return p;
	}

	EXTERN void  wbParse_Free_Emotion(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}


	// error 
	EXTERN void* wbParse_Malloc_Error(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Error)*size);
#else
		t_wbParse_Error *p = (t_wbParse_Error*)malloc(sizeof(t_wbParse_Error)*size);
		memset(p,0,sizeof(t_wbParse_Error)*size);
#endif //PARSE_STRUCT_MEMOP
		return p;
	}
	EXTERN void wbParse_Free_Error(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}


	EXTERN void *wbParse_Malloc_Media(int size)
	{
#ifdef PARSE_STRUCT_MEMOP
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Media)*size);
#else
		t_wbParse_Media* p = (t_wbParse_Media*)malloc( sizeof(t_wbParse_Media) * size );
		memset(p,0,sizeof(t_wbParse_Media)*size);
#endif //PARSE_STRUCT_MEMOP
		return p;
	}

	EXTERN void  wbParse_Free_Media(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

}
