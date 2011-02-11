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

//
#define PARSE_STRUCT_MALLOC(p,size) \
	p = malloc(size);\
	memset(p,0,size);\
	((t_wbParse_Header*)p)->eSource = wbParserNS::EPS_OPENAPI;\


//
#define PARSE_STRUCT_MALLOC_NORMAL(p,size)\
	p = malloc(size);\
	memset(p,0,size);\

//
#define PARSE_STRUCT_FREE(p) p ? free(p) : 0;

//
#define BEGIN_STRCUT_FREE(type,p,size)\
		type *pstruct = (type*)p;\
		if( !p ) return;\
		for( int i = 0; i < size; i ++ ){\
			type *pItem = (pstruct + i);\


#define END_STRUCT_FREE(p) } PARSE_STRUCT_FREE(p);


namespace wbParserNS
{
	EXTERN void *wbParse_Malloc_Cursor(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Cursor)*size);
		return p;
	}
	EXTERN void wbParse_Free_Cursor(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//用户
	EXTERN void *wbParse_Malloc_User(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_User)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Status)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Comment)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_CommentCounts)*size);
		return p;
	}
	EXTERN void wbParse_Free_Commentcount(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//未读数
	EXTERN void  *wbParse_Malloc_Unread(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Unread)*size);
		return p;
	}
	EXTERN void wbParse_Free_Unread(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	//地理位置
	EXTERN void *wbParse_Malloc_Geo(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Geo)*size);
		return p;
	}
	EXTERN void wbParse_Free_Geo(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 私信
	EXTERN void *wbParse_Malloc_Directmessage(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_DirectMessage)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Shipshow)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_IDs)*size);
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
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_LimitStatus)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Limits(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 表情
	EXTERN void *wbParse_Malloc_Emotion(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Emotion)*size);
		return p;
	}

	EXTERN void  wbParse_Free_Emotion(void *pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}


	// error 
	EXTERN void* wbParse_Malloc_Error(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Error)*size);
		return p;
	}
	EXTERN void wbParse_Free_Error(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}


	EXTERN void *wbParse_Malloc_Media(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Media)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Media(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// t_wbParse_Media_item
	EXTERN void *wbParse_Malloc_Media_Item(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_media_item)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Media_Item(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// Result
	EXTERN void *wbParse_Malloc_Ret(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Ret)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Ret(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 用户标签
	EXTERN void *wbParse_Malloc_Tag(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Tag)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Tag(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 话题
	EXTERN void *wbParse_Malloc_Trend(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Trend)*size);
		return p;
	}
	EXTERN void  wbParse_Free_Trend(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}
	// 话题热点查询
	EXTERN void *wbParse_Malloc_TrendHotQuery(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_TrendHotQuery)*size);
		return p;
	}
	EXTERN void  wbParse_Free_TrendHotQuery(void* pvoid,int size)
	{
		BEGIN_STRCUT_FREE(t_wbParse_TrendHotQuery,pvoid,size);
		//
		wbParse_Free_TrendHotQuery_Item(pItem->trends_,pItem->trend_counts_);
		//
		END_STRUCT_FREE(pvoid);
	}
	// 话题热点查询item
	EXTERN void *wbParse_Malloc_TrendHotQuery_Item(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_TrendHotQuery::TrendItem)*size);
		return p;
	}
	EXTERN void  wbParse_Free_TrendHotQuery_Item(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

	// 邀请联系人结构
	EXTERN void *wbParse_Malloc_InviteContact(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_InviteContact)*size);
		return p;
	}
	EXTERN void  wbParse_Free_InviteContact(void *p,int size)
	{
		BEGIN_STRCUT_FREE(t_wbParse_InviteContact,p,size);
		//
		wbParse_Free_InviteContact_Usr(pItem->in_attend_usr_     ,pItem->in_attend_usr_count_);
		wbParse_Free_InviteContact_Group(pItem->not_attend_group_,pItem->not_attend_group_count_);
		wbParse_Free_InviteContact_Group(pItem->out_attend_group_,pItem->out_attend_group_count_);
		//
		END_STRUCT_FREE(p);
	}

	// 邀请联系人，用户结构
	EXTERN void *wbParse_Malloc_InviteContact_Usr(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_InviteContact::ItemUsr)*size);
		return p;
	}
	EXTERN void wbParse_Free_InviteContact_Usr(void *p,int size)
	{
		PARSE_STRUCT_FREE(p);
	}

	// 邀请联系人，用户分组结构
	EXTERN void *wbParse_Malloc_InviteContact_Group(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC_NORMAL(p,sizeof(t_wbParse_InviteContact::ItemGroup)*size);
		return p;
	}
	EXTERN void wbParse_Free_InviteContact_Group(void *p,int size)
	{
		BEGIN_STRCUT_FREE(t_wbParse_InviteContact::ItemGroup,p,size);
		PARSE_STRUCT_FREE(pItem->usrs_);
		END_STRUCT_FREE(p);
	}

	// 短链接
	EXTERN void *wbParse_Malloc_Media_ShortURLBatch(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Media_ShortUrlBatch)*size);
		return p;
	}
	EXTERN void wbParse_Free_Media_ShortURLBatch(void *p,int size)
	{
		BEGIN_STRCUT_FREE(t_wbParse_Media_ShortUrlBatch,p,size);

		PARSE_STRUCT_FREE(pItem->annotions_);

		END_STRUCT_FREE(p);
	}

	// 短链接ITEM
	EXTERN void *wbParse_Malloc_Media_ShortURLBatch_ItemAnnotions(int size)
	{
		void *p = NULL;
		PARSE_STRUCT_MALLOC(p,sizeof(t_wbParse_Media_ShortUrlBatch::ItemAnnotions)*size);
		return p;
	}
	EXTERN void wbParse_Free_Media_ShortURLBatch_ItemAnnotions(void *p,int size)
	{
		PARSE_STRUCT_FREE(p);
	}

	EXTERN void *wbParse_Malloc_UploadPic(int size)
	{
		void *ptr = NULL;
		PARSE_STRUCT_MALLOC(ptr,sizeof(t_wbParse_UploadPic)*size);
		return ptr;
	}
	EXTERN void  wbParse_Free_UploadPic(void* pvoid,int size)
	{
		PARSE_STRUCT_FREE(pvoid);
	}

}
