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
#include "wbParser/WBPStruct.h"
#include "wbParseMalloc.h"
#include <stdlib.h>

namespace wbParserNS
{
	EXTERN void *wbParse_Malloc_Cursor(int size)
	{
		void *p = malloc(sizeof(t_wbParse_Cursor)*size);
		memset(p,0,sizeof(t_wbParse_Cursor)*size);
		return p;
	}
	EXTERN void wbParse_Free_Cursor(void *pvoid,int size)
	{
		t_wbParse_Cursor *p = (t_wbParse_Cursor*)pvoid;
		if( p )
		{
			free(p);
		}
	}

	//用户
	EXTERN void *wbParse_Malloc_User(int size)
	{
		void *p = malloc(sizeof(t_wbParse_User)*size);
		memset(p,0,sizeof(t_wbParse_User)*size);
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
				if( item ->last_status ) wbParse_Free_Status(item ->last_status,1);
				i++;
			}while(i<size);
			free(pvoid);
		}
	}

	//状态
	EXTERN void  *wbParse_Malloc_Status(int size)
	{
		void *p = malloc(sizeof(t_wbParse_Status)*size);
		memset(p,0,sizeof(t_wbParse_Status)*size);
		return p;
	}
	EXTERN void wbParse_Free_Status(void  *pvoid,int size)
	{
		t_wbParse_Status *p = (t_wbParse_Status*)pvoid;
		if( p )
		{
			int i = 0;
			do
			{
				t_wbParse_Status *item = (p+i);
				if( item->geo ) wbParse_Free_Geo(item->geo,1);
				if( item->user) wbParse_Free_User(item->user,1);
				if( item->retweeted_status ) wbParse_Free_Status(item->retweeted_status,1);
				i++;
			}while( i < size );
			free(pvoid);
		}
	} 

	//评论
	EXTERN void  *wbParse_Malloc_Comment(int size)
	{
		void* p = malloc(sizeof(t_wbParse_Comment)*size);
		memset(p,0,sizeof(t_wbParse_Comment)*size);
		return p;
	}

	EXTERN void wbParse_Free_Comment(void  *pvoid,int size)
	{
		t_wbParse_Comment *p = (t_wbParse_Comment*)pvoid;
		if( p )
		{
			int i = 0;
			do
			{
				t_wbParse_Comment *item = (p+i);
				if( item->user)   wbParse_Free_User(item->user,1);
				if( item->status) wbParse_Free_Status(item->status,1);
				if( item->reply_comment) wbParse_Free_Comment(item->reply_comment,1);
				i++;
			}while(i<size);
			free(pvoid);
		}
	}

	//评论数
	EXTERN void *wbParse_Malloc_Commentcount(int size)
	{
		void* p = malloc(sizeof(t_wbParse_CommentCounts)*size);
		memset(p,0,sizeof(t_wbParse_CommentCounts)*size);
		return p;
	}
	EXTERN void wbParse_Free_Commentcount(void *pvoid,int size)
	{
		t_wbParse_CommentCounts *p = (t_wbParse_CommentCounts*)pvoid;
		if( p )
		{
			free(pvoid);
		}
	}

	//未读数
	EXTERN void  *wbParse_Malloc_Unread(int size)
	{
		void* p = malloc(sizeof(t_wbParse_Unread)*size);
		memset(p,0,sizeof(t_wbParse_Unread)*size);
		return p;
	}
	EXTERN void wbParse_Free_Unread(void *pvoid,int size)
	{
		t_wbParse_Unread *p = (t_wbParse_Unread*)pvoid;
		if( p ){
			free(pvoid);
		}
	}

	//地理位置
	EXTERN void *wbParse_Malloc_Geo(int size)
	{
		void *p = malloc(sizeof(t_wbParse_Geo)*size);
		memset(p,0,sizeof(t_wbParse_Geo)*size);
		return p;
	}
	EXTERN void wbParse_Free_Geo(void* pvoid,int size)
	{
		t_wbParse_Geo *p = (t_wbParse_Geo*)pvoid;
		if( p ){
			free(pvoid);
		}
	}

	// 私信
	EXTERN void *wbParse_Malloc_Directmessage(int size)
	{
		void *p = malloc(sizeof(t_wbParse_DirectMessage)*size);
		memset(p,0,sizeof(t_wbParse_DirectMessage)*size);
		return p;
	}
	EXTERN void wbParse_Free_Directmessage(void *pvoid,int size)
	{
		t_wbParse_DirectMessage *p = (t_wbParse_DirectMessage*)pvoid;
		if( p )
		{
			int i = 0;
			do
			{
				t_wbParse_DirectMessage *item = (p+i);
				if(item->sender )   wbParse_Free_User(item->sender,1);
				if(item->recipient) wbParse_Free_User(item->recipient,1);
				i++;
			}while(i < size );
			free(pvoid);
		}
	}

	// 好友关系
	EXTERN void *wbParse_Malloc_Friendship(int size)
	{
		void *p = malloc(sizeof(t_wbParse_Shipshow)*size);
		memset(p,0,sizeof(t_wbParse_Shipshow)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Friendship(void *pvoid,int size)
	{
		t_wbParse_Shipshow *p = (t_wbParse_Shipshow*)pvoid;
		if( p ){
			int i = 0;
			do
			{
				t_wbParse_Shipshow *item = (p+i);
				if( item->source ) free(item->source);
				if( item->target ) free(item->target);
				i++;
			}while(i<size);
			free(pvoid);
		}
	}
	//

	//// ids
	//EXTERN void* ids_malloc(void)
	//{
	//	t_wb_userlist *p = (t_wb_userlist*)malloc(sizeof(t_wb_userlist));
	//	memset(p,0,sizeof(t_wb_userlist));
	//	return p;
	//}
	//EXTERN void  ids_free(void *pvoid)
	//{
	//	t_wb_userlist *p = (t_wb_userlist*)pvoid;
	//	if( p )
	//	{
	//		t_wb_list * l = p->id_list;
	//		while( l )
	//		{
	//			t_wb_list *tmp = l->next;
	//			l->next = NULL;
	//			l->data = 0;
	//			free(l);
	//			l = tmp ;
	//		}
	//		free(p);
	//	}
	//}

	// 访问限制
	EXTERN void *wbParse_Malloc_Limits(int size)
	{
		void *p = malloc(sizeof(t_wbParse_LimitStatus)*size);
		memset(p,0,sizeof(t_wbParse_LimitStatus)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Limits(void *pvoid,int size)
	{
		t_wbParse_LimitStatus *p = (t_wbParse_LimitStatus*)pvoid;
		if( p ){
			free(p);
		}
	}

	// 表情
	EXTERN void *wbParse_Malloc_Emotion(int size)
	{
		void *p = malloc(sizeof(t_wbParse_Emotion)*size);
		memset(p,0,sizeof(t_wbParse_Emotion)*size);
		return p;
	}

	EXTERN void  wbParse_Free_Emotion(void *p,int size)
	{
		if( p ){
			free(p);
		}
	}


	// error 
	EXTERN void* wbParse_Malloc_Error(int size)
	{
		t_wbParse_Error *p = (t_wbParse_Error*)malloc(sizeof(t_wbParse_Error)*size);
		memset(p,0,sizeof(t_wbParse_Error)*size);
		return p;
	}
	EXTERN void wbParse_Free_Error(void* pvoid,int size)
	{
		t_wbParse_Error *p = (t_wbParse_Error*)pvoid;
		if( p ){
			free(p);
		}
	}


	EXTERN void *wbParse_Malloc_Media(int size)
	{
		t_wbParse_Media* p = (t_wbParse_Media*)malloc( sizeof(t_wbParse_Media) * size );
		memset(p,0,sizeof(t_wbParse_Error)*size);
		return p;
	}

	EXTERN void  wbParse_Free_Media(void* pvoid,int size)
	{
		t_wbParse_Media *p = (t_wbParse_Media*)pvoid;
		if( p ){
			free(p);
		}
	}

}
