////////////////////////////////////////////////////////////////////////
//
// ����ӿ� ,by welbon,2011-01-10

//trends ��ȡĳ�˵Ļ���
WEIBO_struct_statuscallback_init(GET_TRENDS)
{
}
WEIBO_struct_bodycallback_init(GET_TRENDS)
{
	printf("   GET_TRENDS : \n    %s \n\n" , body );
}

//trends/statuses ��ȡĳһ�����µ�΢��
WEIBO_struct_statuscallback_init(GET_TRENDS_STATUSES)
{
}
WEIBO_struct_bodycallback_init(GET_TRENDS_STATUSES)
{
	printf("   GET_TRENDS_STATUSES : \n    %s \n\n" , body );
}

//trends/follow ��עĳһ������
WEIBO_struct_statuscallback_init(POST_TRENDS_FOLLOW)
{
}
WEIBO_struct_bodycallback_init(POST_TRENDS_FOLLOW)
{
	printf("   POST_TRENDS_FOLLOW : \n    %s \n\n" , body );
}

//trends/destroy ȡ����ע��ĳһ������
WEIBO_struct_statuscallback_init(DELETE_TRENDS_DESTROY)
{
}
WEIBO_struct_bodycallback_init(DELETE_TRENDS_DESTROY)
{
	printf("   DELETE_TRENDS_DESTROY : \n    %s \n\n" , body );
}


//trends/hourly ��Сʱ�������Ż���
WEIBO_struct_statuscallback_init(GET_TRENDS_HOURLY)
{
}
WEIBO_struct_bodycallback_init(GET_TRENDS_HOURLY)
{
	printf("   GET_TRENDS_HOURLY : \n    %s \n\n" , body );
}


//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
WEIBO_struct_statuscallback_init(GET_TRENDS_DAYLIY)
{

}
WEIBO_struct_bodycallback_init(GET_TRENDS_DAYLIY)
{
	printf("   GET_TRENDS_DAYLIY : \n    %s \n\n" , body );
}


//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���
WEIBO_struct_statuscallback_init(GET_TRENDS_WEEKLIY)
{

}
WEIBO_struct_bodycallback_init(GET_TRENDS_WEEKLIY)
{
	printf("   GET_TRENDS_WEEKLIY : \n    %s \n\n" , body );
}