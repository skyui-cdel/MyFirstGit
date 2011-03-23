#include "stdafx.h"
#include "option.h"
#include <libweibo/weibo.h>

struct tagOption
{
	int  iOpt;
	char text[255];
};

#define WEIBO_OPTION_MAP(opt) {WEIBO_OPTION(opt),#opt}

static const tagOption _option_map[] = 
{
	WEIBO_OPTION_MAP(UNK),
	WEIBO_OPTION_MAP(BASE), //1

	//��ȡ�������ݼ�(timeline)
	WEIBO_OPTION_MAP(GETSTATUSES_PUBLIC_TIMELINE), //2 ��ȡ���¸��µĹ���΢����Ϣ 
	WEIBO_OPTION_MAP(GETSTATUSES_FRIENDS_TIMELINE), //3 ��ȡ��ǰ�û�����ע�û�������΢����Ϣ (����: statuses/home_timeline) 
	WEIBO_OPTION_MAP(GETSTATUSES_USE_TIMELINE), //4 ��ȡ�û�������΢����Ϣ�б� 
	WEIBO_OPTION_MAP(GETSTATUSES_MENTIONS), //5 ��ȡ@��ǰ�û���΢���б� 
	WEIBO_OPTION_MAP(GETSTATUSES_COMMENTS_TIMELINE), //6 ��ȡ��ǰ�û����ͼ��յ��������б�
	WEIBO_OPTION_MAP(GETSTATUSES_COMMENTS_BYME), //7 ��ȡ��ǰ�û����������� 
	WEIBO_OPTION_MAP(GETSTATUSES_COMMENTS_TOME), //7 ��ȡ��ǰ�û��յ������� 
	WEIBO_OPTION_MAP(GETSTATUSES_COMMENTS_LIST), //8 ��ȡָ��΢���������б� 
	WEIBO_OPTION_MAP(GETSTATUSES_COMMENTS_COUNTS), //9 ������ȡһ��΢������������ת���� 
	WEIBO_OPTION_MAP(GETSTATUSES_UNREAD), //10 ��ȡ��ǰ�û�δ����Ϣ�� 
	WEIBO_OPTION_MAP(PUTSTATUSES_RESET_COUNT),//δ����Ϣ������ӿ� 

	//΢������
	WEIBO_OPTION_MAP(GETSTATUSES_SHOW), //11 ����ID��ȡ����΢����Ϣ���� 
	WEIBO_OPTION_MAP(GOTOSTATUSES_ID),//12 ����΢��ID���û�ID��ת������΢��ҳ�� 
	WEIBO_OPTION_MAP(PUTSTATUSES_UPDATE),//13 ����һ��΢����Ϣ 
	WEIBO_OPTION_MAP(PUTSTATUSES_UPLOAD),//14 �ϴ�ͼƬ������һ��΢����Ϣ 
	WEIBO_OPTION_MAP(PUTSTATUSES_UPLOAD_PIC),// �ϴ�ͼƬ������һ��΢����Ϣ 
	WEIBO_OPTION_MAP(PUTSTATUSES_DESTROY),//15 ɾ��һ��΢����Ϣ 
	WEIBO_OPTION_MAP(PUTSTATUSES_REPOST),//16 ת��һ��΢����Ϣ���ɼ����ۣ� 
	WEIBO_OPTION_MAP(PUTSTATUSES_COMMENT),//17 ��һ��΢����Ϣ�������� 
	WEIBO_OPTION_MAP(PUTSTATUSES_COMMENT_DESTROY),//18 ɾ����ǰ�û���΢��������Ϣ 
	WEIBO_OPTION_MAP(PUTSTATUSES_REPLY),//19 �ظ�΢��������Ϣ 

	//�û�
	WEIBO_OPTION_MAP(GETUSER_INFO),//20 �����û�ID��ȡ�û����ϣ���Ȩ�û��� 
	WEIBO_OPTION_MAP(GETFRINDS_LIST),//21 ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ 
	WEIBO_OPTION_MAP(GETFOLLOWERS_LIST),//22 ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ 

	//˽��
	WEIBO_OPTION_MAP(GETDIRECTMSG),//23 ��ȡ��ǰ�û�����˽���б� 
	WEIBO_OPTION_MAP(GETDIRESTMSG_SENT),//24 ��ȡ��ǰ�û����͵�����˽���б�
	WEIBO_OPTION_MAP(PUTDIRECTMSG_NEW),//25 ����һ��˽�� 
	WEIBO_OPTION_MAP(PUTDIRECTMSG_DESTROY),//26 ɾ��һ��˽�� 
	WEIBO_OPTION_MAP(GETDIRECTMSG_WITH),//����˽���б�

	//��ע
	WEIBO_OPTION_MAP(PUTFRIENDSHIPS_CREATE),//27 ��עĳ�û� 
	WEIBO_OPTION_MAP(PUTFRIENDSHIPS_CREATE_BATCH),//������ע 
	WEIBO_OPTION_MAP(PUTFRIENDSHIPS_DESTROY),//28 ȡ����ע 
	WEIBO_OPTION_MAP(GETFRIENDSHIPS_EXISTS),//29 �ж������û��Ƿ��й�ע��ϵ�����������û���ϵ����ϸ���
	WEIBO_OPTION_MAP(GETFRIENDSHIPS_BATCH_EXISTS),//

	//Social Graph
	WEIBO_OPTION_MAP(GETFRIEND_IDS),//30 ��ע�б�
	WEIBO_OPTION_MAP(GETFOLLOWER_IDS),//31 ��˿�б�

	//�˺� 
	WEIBO_OPTION_MAP(GETACCOUNT_VERIFY),//32 ��֤��ǰ�û�����Ƿ�Ϸ� 
	WEIBO_OPTION_MAP(GETACCOUNT_RATELIMIT),//33 ��ȡ��ǰ�û�API����Ƶ������ 
	WEIBO_OPTION_MAP(PUTACCOUNT_QUITSESSION),//34 ��ǰ�û��˳���¼ 
	WEIBO_OPTION_MAP(PUTACCOUNT_UPDATE_PROFILEIMAGE),//35 ����ͷ��
	WEIBO_OPTION_MAP(PUTACCOUNT_UPDATE_PROFILE),//36 ��������
	WEIBO_OPTION_MAP(PUTACCOUNT_REGISTER),//37

	// �ղ�
	WEIBO_OPTION_MAP(GETFAVORITES),// 38��ȡ��ǰ�û����ղ��б� 
	WEIBO_OPTION_MAP(PUTFAVORITES_CREATE),// 39����ղ� 
	WEIBO_OPTION_MAP(PUTFAVORITES_DESTROY),// 40ɾ����ǰ�û��ղص�΢����Ϣ 

	//��¼/OAuth
	WEIBO_OPTION_MAP(OAUTH_REQUEST_TOKEN),// 41��ȡδ��Ȩ��Request Token 
	WEIBO_OPTION_MAP(OAUTH_AUTHORIZE),// 42�����û���ȨToken 
	WEIBO_OPTION_MAP(OAUTH_ACCESS_TOKEN),// 43��ȡ��Ȩ����Access Token

	// ����
	WEIBO_OPTION_MAP(GET_EMOTIONS),// 44 ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ� 

	// �û����� 
	WEIBO_OPTION_MAP(GET_USERS_SEARCH),// 45 ����΢���û�,���عؼ���ƥ���΢���û���(�������˺��������߿���) 

	// ΢������ 
	WEIBO_OPTION_MAP(GET_WB_SEARCH),// 46 ���عؼ���ƥ���΢����(�������˺��������߿���) 
	WEIBO_OPTION_MAP(GET_STATUSES_SEARCH),//47 ����΢��(���������) (���Ժ��������߿���) 

	WEIBO_OPTION_MAP(GET_PROVINCES), // 48 ʡ�ݳ��б���� 
	WEIBO_OPTION_MAP(REPORT),//49 �ٱ�

	WEIBO_OPTION_MAP(COOKIE),// 50 cookie
	WEIBO_OPTION_MAP(UPDATETGT), // 51 ����cookie

	//�Զ���URL
	WEIBO_OPTION_MAP(CUSTOM),//

	// �¼ӽӿ�,by welbon,2011-01-07
	WEIBO_OPTION_MAP(HOT_REPOST_DAYLIY), //����ת��-by day
	WEIBO_OPTION_MAP(HOT_REPOST_WEEKLY), //����ת��-by week
	WEIBO_OPTION_MAP(HOT_COMMENT_DAYLIY), //��������-by day
	WEIBO_OPTION_MAP(HOT_COMMENT_WEEKLY), //��������-by week

	WEIBO_OPTION_MAP(GET_USERS_HOT),// ��ȡϵͳ�Ƽ��û�
	WEIBO_OPTION_MAP(POST_USERS_REMARK),//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
	WEIBO_OPTION_MAP(GET_USERS_SUGGESTIONS), //Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

	// ����ӿ� ,by welbon,2011-01-10
	WEIBO_OPTION_MAP(GET_TRENDS),//trends ��ȡĳ�˵Ļ���
	WEIBO_OPTION_MAP(GET_TRENDS_STATUSES),//trends/statuses ��ȡĳһ�����µ�΢��
	WEIBO_OPTION_MAP(POST_TRENDS_FOLLOW),//trends/follow ��עĳһ������
	WEIBO_OPTION_MAP(DELETE_TRENDS_DESTROY),//trends/destroy ȡ����ע��ĳһ������
	WEIBO_OPTION_MAP(GET_TRENDS_HOURLY),//trends/destroy ��Сʱ�������Ż���
	WEIBO_OPTION_MAP(GET_TRENDS_DAYLIY),//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
	WEIBO_OPTION_MAP(GET_TRENDS_WEEKLIY),//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

	// �������ӿ� ,by welbon,2011-01-10
	WEIBO_OPTION_MAP(POST_BLOCKS_CREATE),//��ĳ�û����������
	WEIBO_OPTION_MAP(POST_BLOCKS_DESTROY),//��ĳ�û��Ƴ�������
	WEIBO_OPTION_MAP(GET_BLOCKS_EXISTS),//���ĳ�û��Ƿ��Ǻ������û�
	WEIBO_OPTION_MAP(GET_BLOCKS_BLOCKING),//�г��������û�(����û���ϸ��Ϣ)
	WEIBO_OPTION_MAP(GET_BLOCKS_BLOCKING_IDS),//�г���ҳ�������û���ֻ���id��

	//�û���ǩ�ӿ� ,by welbon,2011-01-10
	WEIBO_OPTION_MAP(GET_TAGS),//tags ����ָ���û��ı�ǩ�б�
	WEIBO_OPTION_MAP(POST_TAGS_CREATE),//tags/create ����û���ǩ
	WEIBO_OPTION_MAP(GET_TAGS_SUGGESTIONS),//tags/suggestions �����û�����Ȥ�ı�ǩ
	WEIBO_OPTION_MAP(POST_TAGS_DESTROY),//tags/destroy ɾ����ǩ
	WEIBO_OPTION_MAP(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch ����ɾ����ǩ

	// ����ӿ�
	WEIBO_OPTION_MAP(POST_INVITE_MAILCONTACT),//����������ϵ��
	WEIBO_OPTION_MAP(POST_INVITE_MSNCONTACT), //����MSN��ϵ��
	WEIBO_OPTION_MAP(POST_INVITE_SENDMAILS),  //���������ʼ�

	// Media
	WEIBO_OPTION_MAP(GET_MEDIA_SHORTURL_BATCH),  //������ȡSHORTURL

	//��¼/XAuth
	WEIBO_OPTION(XAUTH_ACCESS_TOKEN),//��ȡ��Ȩ����Access Token
	
};

extern void print_weibo_command_list()
{
	printf("\r\nCommand list :\r\n");

	for( int i = 0; i < sizeof(_option_map)/sizeof(_option_map[0]); ++ i ){
		printf("value :%d,option:%s , \r\n",_option_map[i].iOpt,_option_map[i].text);
	}
}

extern int convert_key_to_number(const char* key)
{
	for( int i = 0; i < sizeof(_option_map)/sizeof(_option_map[0]); ++ i ){
		return (strcmp(key,_option_map[i].text) == 0) ? _option_map[i].iOpt : 0 ;
	}
	return 0;
}