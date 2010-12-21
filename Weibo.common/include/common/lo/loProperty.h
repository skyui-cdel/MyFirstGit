#ifndef __lo_PROPERTY_H__
#define __lo_PROPERTY_H__

// namespace locom
DEFINE_NAMESPACE(locom)

/** ����ֵ
*
* @author loach
*
* @date 2009-06-21
*/
template< typename PROPERTY_KEY>
struct loNovtableM IloPropertyItem
{
	/** ���һ��(�������,�ͽ����޸�) */
	virtual int Add(const PROPERTY_KEY& key,const char* value,size_t len) =0;

	/** ɾ���ض�����ֵ */
	virtual int Remove(const PROPERTY_KEY& key) =0;

	/** �����������ֵ */
	virtual void Clear(void) = 0;

	/** ��ȡ��Ӧ��ֵ */
	virtual const char* Get(const PROPERTY_KEY& key) const =0;

	/** ��ȡ��Ӧ��ֵ */
	virtual const char* Get(const PROPERTY_KEY& key , size_t& len) const = 0;

	/** ��ȡ������ֵ */
	virtual IloPropertyItem<PROPERTY_KEY>* GetChild(void) const = 0;
	virtual bool IsChild(void) const = 0;
	virtual bool NewChild(void) = 0;	
};

END_NAMESPACE(locom)

#endif //