//////////////////////////////////////////////////////////////////////////////////////////////
// �û���ǩ�ӿ� ,by welbon,2011-01-10

//tags ����ָ���û��ı�ǩ�б�
WEIBO_struct_statuscallback_init(GET_TAGS)
{

}
WEIBO_struct_bodycallback_init(GET_TAGS)
{
	printf("   GET_TAGS : \n    %s \n\n" , body );
}


//tags/create ����û���ǩ
WEIBO_struct_statuscallback_init(POST_TAGS_CREATE)
{

}
WEIBO_struct_bodycallback_init(POST_TAGS_CREATE)
{
	printf("   POST_TAGS_CREATE : \n    %s \n\n" , body );
}


//tags/suggestions �����û�����Ȥ�ı�ǩ
WEIBO_struct_statuscallback_init(GET_TAGS_SUGGESTIONS)
{

}
WEIBO_struct_bodycallback_init(GET_TAGS_SUGGESTIONS)
{
	printf("   GET_TAGS_SUGGESTIONS : \n    %s \n\n" , body );
}

//tags/destroy ɾ����ǩ
WEIBO_struct_statuscallback_init(POST_TAGS_DESTROY)
{

}
WEIBO_struct_bodycallback_init(POST_TAGS_DESTROY)
{
	printf("   POST_TAGS_DESTROY : \n    %s \n\n" , body );
}

//tags/destroy_batch ����ɾ����ǩ
WEIBO_struct_statuscallback_init(POST_TAGS_DESTROY_BATCH)
{

}
WEIBO_struct_bodycallback_init(POST_TAGS_DESTROY_BATCH)
{
	printf("   POST_TAGS_DESTROY_BATCH : \n    %s \n\n" , body );
}

