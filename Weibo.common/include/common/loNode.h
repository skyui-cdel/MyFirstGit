#ifndef __loNODE_H__
#define __loNODE_H__

#include <common/loCommon.h>
#include <common/loString.h>
#include <common/loattr.h>
#include <common/lo/lounknwn.h>

// export dll class 
DEFINE_NAMESPACE(locom)
class loCOMMON_API CloUnknown;
END_NAMESPACE(locom)


// namespace locom
DEFINE_NAMESPACE(locom)

// ǰ������
class CloTreePrivate;


/// OnCommandItem(HTREENODE hNode , UINT eCmd,void*param0=NULL) ����������ֵ
enum enItemCommand
{
	CMD_ITEM_RELEASE, ///< CloTree ������������
	CMD_ITEM_ADD,     ///< hTreeNode ��ӽ����, eCmd=CMD_ITEM_ADD , param0 =  ����ӵĶ���(HTREENODE)
	CMD_ITEM_DELETE,  ///< hTreeNode ɾ�������, eCmd=CMD_ITEM_DELETE , param0= ��ɾ���Ķ���(HTREENODE)
};

/// ��������ָ��
typedef class CloNode* HNODE;

/// ��������ָ��
typedef class CloTree* HTREENODE;

/**
 * @brief ���ʵ����(����ʵ����,IloAttribute ����û��ʵ��)
 *
 * @author loach
 *
 * @date 2007-01-02
*/
class loCOMMON_API CloNode : public locom::CloUnknown , public IloAttribute
{
public:
	/// Constructor
	CloNode();
	/// Destructor
	virtual ~CloNode();
	
	DECLARE_IUNKNOWN_LO
private:
	/// ���ε����ַ�ʽ��copy
	CloNode(const CloNode& n)
		:CloUnknown(NULL)
	{}	
	CloNode& operator = (const CloNode& n){ return *this;}
private:
	/** ��Ӧ����(��ҪΪ�˵� HTREENODE �����仯�ǣ�Ҫ��HNODE֪��,������֪ͨ����ʲô�仯 )
	 * @param eCmd ( �ɲο� enItemCommand )
	 * @param hNode this��������
	*/
	virtual int OnCommandItem(HTREENODE hNode , UINT eCmd , void* param0=NULL){
		return -1;
	}
private:
	friend class CloTree;
};

/** ���һ��֪ͨ�¼��ӿڶ���
 *
 * @author loach
 *
 * @date 2009-06-14
*/
struct IloNodeObjserver
{
	enum eNotifyCode
	{
		ENC_NODE_Unknown=0,
		ENC_NODE_Remove,///< ���� DetachNode() ֪ͨ�¼����丸�������һ����� ,param1= htreenode ,param0 = htreenode->Parent,param2=0
		ENC_NODE_Add,   ///< ���� InsertSiblingNode(),InsertChildNode(), AttachNode() ֪ͨ�¼����丸������һ����� ,param1= htreenode ,param0 = htreenode->Parent,param2=0
		ENC_NODE_Delete,///< ���� DeleteNode() ֪ͨ�¼����ϵĸ����ɾ��һ�����,,param1= htreenode ,param0 = htreenode->Parent[�ϵĽ��],param2 =0
		ENC_NODE_Flags_update, ///< SetFlags(UINT nFlags) �����仯 param0=htreenode,param1 = �µ�&int,param2 = �ϵ�&int
		ENC_NODE_Flags_bit,///< SetFlags(bits,isTrue)  �����仯 param0=htreenode,param1 = &bits,param2 = &isTrue
	};
	virtual void OnNotify(UINT eNotify,void* param0,void* param1,void* param2) = 0;
};

/// �ȽϵĻص�����
typedef int  (*PNodeCompareCB)(HTREENODE node1,HTREENODE node2);

/// add by loach 2009-06-09˽�д���Ļص�,��RemovePrivate��ص���һ��[ AddPrivate() ]
typedef void (*PNodeprivateCB)(const char* key,void* pp);

/// ��ѯ������
typedef bool (*PNodequeryCB)(HTREENODE hTree,void* pQueryCondition);

/// node id ֵ
typedef CloString SNodeID;

/**
* @ingroup locom
*
* @brief ��֦��������
*
* ������ʱ,�������ⲿ���з����ڴ�,���ڲ�����Ψһ�Լ��,
* �����Ҫ���м��Ψһ��,���ڲ���֮ǰ,����IsHasNode(...)
* ������Ľ�����m_hNode����������,������ʱ,ֻ����뵽�ӽ����ȥ
*   root(�����)
*    child <-- ����
*   next  = null;
*   prev  = null;
*   parent= null;
*
* @author loach
*
* @date 2007-01-02
*
* @bug ��֧�� "=" operator
*
* 
* @modified by loach 2009-06-09
*
*  1 ����ṹ����һ�� id����ĸĴ������id ��HNODE ��������ģ���ʹ���߾������ϵ
*  2 ��� ReSortSliblingNode ����
*  3 ��� CloTreePrivate ��غ���
*  4 Ϊ��Ӧ�Ľӿ����IloNodeObjserver
*  
*/
class loCOMMON_API CloTree : public t_loPOSITION
{
public:
	typedef void* PrivateValue;

	/** ��־	 */
	enum enTNFlag 
	{
		TN_NULL        = 0x0000, ///<  �ղ���
		TN_VISIBLE_BIT = 0x0001, ///<  �Ƿ�ɼ���,�Ը���˵,��TN_EXPAND_BIT���ʹ��
		TN_SORT_BIT    = 0x0002, ///<  �Ƿ�����
		TN_CHECK_BIT   = 0x0004, ///<  �Ƿ�ѡ��
		TN_EXPAND_BIT  = 0x0008, ///<  �Ƿ�չ��,��Ҫ���ڸ�,���������ӽ��Ŀɼ���,��������־λΪ0,�������ɼ�,1�ɼ� 
		TN_LOCK_BIT    = 0x0010, ///<  �����㱻����,��Ҫ���ڸ� ,���ܽ���Expand����
		TN_ICON_EXPAND_BIT=0x0020,///< ͼ��Ҳ��expand����,��TN_LOCK_BIT����
		TN_SELECT_BIT     = 0x0040,///< ���ѡ�У���checkbox��ͬ
		TN_MOUSEON_BIT    = 0x0080,///< ��������� 
	};

	/// �������
	enum enNodeType
	{
		NT_ROOT = 1, ///< �����
		NT_NODE = 2  ///< Ҷ��
	};

	/** ��ѯ��־ */
	enum enQueryFlag
	{
		QF_ROOT=0x01,///<ֻ��ѯ��
		QF_NODE=0x02,///<ֻ��ѯ���
		QF_ALL =0x03,///<����
	};

	/// check ��־
	enum enCheckFlag
	{
		CF_REVERSE=0,///<��ѡ
		CF_SELECT,///<ѡ��
		CF_NOSELECT,///<��ѡ
	};

	/// Constructor
	CloTree(const SNodeID& inId , enNodeType type = NT_ROOT , HNODE node = NULL , UINT nFlags = TN_VISIBLE_BIT);
	/// Destructor
	virtual ~CloTree();

private:
	/// ���ε����ַ�ʽ��copy
	CloTree(const CloTree& t){}	
	CloTree& operator = (const CloTree& t){ return *this;}
public:
	/**
	* @brief ����һ����㵽ͬ���ֵ�,ֱ�Ӳ���,û�в�ѯ�Ƿ����,�����Ҫ��ȷ,���ȵ���IsHasNode()����
	*
	* @param node ������õ��ڴ�,����ʵ�ֶ�̬����,node������������������,��Ҫ������ͷ�
	* @param pInsNode ,�����λ��,if pInsNode!=NULL,ȷ�� pInsNode �� this ��ͬ����
	*
	* @param id Ϊÿ��HTREENODE ָ��һ��id( add by loach 2009-06-08)
	*/
	HTREENODE InsertSiblingNode(  enNodeType type,
		                          const SNodeID& inId,
		                          const HNODE node,								  
		                          const HTREENODE pInsNode = NULL,BOOL  bBack = TRUE ,PNodeCompareCB lpCompareCB = NULL);


	/** 
	* @brief ����һ����㵽�ӽ��
	* ֱ�Ӳ���,û�в�ѯ�ӽ���Ƿ����,�����Ҫ��ȷ,���ȵ���IsHasNode()����
	*
	* @param node ������õ��ڴ�,����ʵ�ֶ�̬����,node������������������,��Ҫ������ͷ�
	* @param pInsNode ����� ,if pInsNode!=NULL,ȷ�� pInsNode �� this ���ӽ��
	*
	* @param id Ϊÿ��HTREENODE ָ��һ��id( add by loach 2009-06-08)
	*/
	HTREENODE InsertChildNode(  enNodeType type,
		                        const SNodeID& inId,
		                        const HNODE node,								
		                        const HTREENODE pInsNode=NULL,BOOL bBack = TRUE ,PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief ɾ��һ�����,�ͷ��ڴ�
	*/
	int DeleteNode(HTREENODE hNode);

	/**
	* @brief ����һ�����,�����ͷ�
	*
	* @param pObjserver �۲���(����ͨ����֪ͨ�����ɾ���� )
	*/
	int DetachNode(const HTREENODE hNode);

	/**
	* @brief ���һ����㵽�ӽ��,HNODE�����ڶ���������������Ч,������һ���ֲ�����
	* ����HNODE �����ǲ�ֻһ�����,�����кܶ��ֵܽ��,���ӽ��
	*/
	int AttachNode(HTREENODE hNode , PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief drag src node to dest node
	*
	* �϶����� srcNode ����Ƶ� destNode ���
	*
	* @param bRoot �Ƿ��϶���,=false ���srcNode�Ǹ�,������ -3\n
	*                   = true ���srcNode�Ǹ�,Ҳ�����϶�
	* @return 0 �ɹ�\n
	*  -1 ��ʾΪNULL\n
	*  -2 �������ֵ�\n
	*  -3 srcNode�Ǹ�
	*/
	int  DragNode(HTREENODE srcNode,
		          HTREENODE destNode,
				  BOOL bRoot = FALSE,
				  PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief ���¶���ͬ�����н���������
	*
	*/
	int ReSortSliblingNode(PNodeCompareCB lpCompareCB);
	/**
	* @brief ���ý��ѡ��
	* @param nFlag ѡ�б�־,��ο� enCheckFlag
	*/
	void CheckNode(int nFlag = CloTree::CF_REVERSE);

	/**
	* @brief ���ý��ѡ�У����ǲ��޸���Ԫ��
	* @param nFlag ѡ�б�־,��ο� enCheckFlag
	*/
	void CheckSelfNode(int nFlag = CloTree::CF_REVERSE);
	/**

	* @brief �Ƿ��������Ľ�����
	* ������������,�����ӽ��,�ֵܽ��
	*/
	BOOL IsHasNode(const HTREENODE hNode);

	/** ���hNode�Ƿ�Щ���ĸ���㣨���������ϼ��ĸ����)
	 *
	 *  +hNode(root)
	 *     +node(child)
	 *     +node(child)
	 *     +node(child)
	 *     +node(root)
	 *        +node(child)
	 *        +node(child)
	 *        +node(root)
	 *           +this(child)
	 *
	 *   ���� this->QueryPNode(hNode) �ͻ᷵��true
	*/
	BOOL QueryPNode(const HTREENODE hNode);

	/** ��ȡʵ�����(�������ϼ����) */
	HTREENODE GetTopNode(void);

	/** 
	* @brief get the special item sid(Ψһ�ı�־),�Ƿ������ͬ��sid����������������
	*
	* @param lpid ��2009-06-08 by loach ����������ĺ���������� )
	*              ��ָ������ HNODE ��Ӧ��ֵ�أ�����Ϊ��ָ HTREENODE ��ָ���� id
	*              node �� lpid ����������
	*
	* @param node ���������
	*
	* @param nFlags ��ο� enQueryFlag 
	* == QF_ROOT ֻ�Ҹ� \n
	* == QF_NODE ֻ�ҽ�㣨���Ǹ�)\n
	* == QF_ALL  ����\n
	*
	* @param pQueryCB ���˲�ѯ���������
	*/
	HTREENODE IsHasNode( const SNodeID& inId ,
		                 UINT nQFlag = QF_NODE , 
						 PNodequeryCB pQueryCB=NULL , void* pQueryCondition = NULL);

	HTREENODE IsHasNode( const HNODE node ,
		                 UINT nQFlag = QF_NODE , 
						 PNodequeryCB pQueryCB=NULL,void* pQueryCondition = NULL);
	/** 
	* @brief �Ƿ�����ӽ��
	*/
	BOOL IsHasChilds() const;
	/** 
	* @brief �Ƿ��Ǹ� 
	*/
	BOOL IsRoot() const;
	/**
	* @brief ��ȡ�����(һ���ĸ���㣬���¼���ϵ)
	*/
	HTREENODE GetParentNode() const;
	/**
	* @brief ��ȡ��һ��ͬ���ֵܽ��
	*/
	HTREENODE GetNextNode() const;
	/**
	* @brief ��ȡ��һ��ͬ���ֵܽ��
	*/
	HTREENODE GetPrevNode() const;
	/**
	* @brief ��ȡ�ӽ��
	*/
	HTREENODE GetChildNode() const;
	/**
	* @brief ��ȡͬ���ֵܵ�β����� LOOP GetNextNode()  ֱ����
	*/
	HTREENODE GetTailNode() const;
	/**
	* @brief ��ȡͬ���ֵܵ�ͷ����� LOOP GetPrevNode()  ֱ����
	*/
	HTREENODE GetHeadNode() const;
	/**
	* @brief Retrieves the next item having the given relationship with the
	* specified item.
	* ��hNode ��ʼ,���»�ȡ,���Ա�������ָ�����
	*
	* @param nFlag �����enTNFlag ,== 0 ��ʾ���н��
	* @param nQFlag ��ο� enQueryFlag		
	*/
	HTREENODE GetNextNode(HTREENODE hNode,
		                  UINT nFlag = TN_NULL,
		                  UINT nQFlag = QF_ALL,
						  PNodequeryCB pQueryCB = NULL,void* pQueryCondition = NULL) const;

	/**
	* @brief ��ȡ���н�������(�������������)
	*/
	int   GetNodeCounts() const;
	/**
	* @brief ��ȡ����������(�������ӽ������)
	*/
	int   GetRootCounts() const;
	/** 
	* @brief ɾ�������ӽ��
	*/
	void  RemoveAllChild(void);
	/**
	* @brief ���ñ�־
	*
	* @param bNotify =TRUE��ϣ���ڲ����� IloNodeObjserver�ӿ���֪ͨ
	*/
	void  SetFlags(int bits , bool isTrue);
	void  SetFlags(int nFlags);
	/**
	* @brief ��ȡ��־
	*/
	int   GetFlags() const;
	BOOL  GetFlags(int bits) const;
	/** 
	* @brief ��ȡ��ȼ��� 
	*/
	int   GetLevel(void) const;
	/**
	* @brief ������ȼ���
	*/
	void  SetLevel(unsigned short level);

	/** 
	* @brief �������ֵ 
	* @return ����(ʧ�� false ,�ɹ� true )
	*/
	int  Add(UINT nKey ,IloAttribute::AttrValue attr,IloAttribute::AttrParam param=NULL)
	{
		return ( m_hNode )? m_hNode->Add(nKey,attr,param): -1 ;
	}
	/**
	* @brief ��ѯ����ֵ 
	*/
	IloAttribute::AttrValue Get(UINT nKey,IloAttribute::AttrParam param = NULL)
	{
		return ( m_hNode )? m_hNode->Get(nKey,param):0;
	}
	/**
	* @brief Delete
	*/
	bool Remove(UINT nKey)
	{
		return ( m_hNode ) ? m_hNode->Remove(nKey): true;
	}
	/** 
	* @brief ��ȡ������ָ��
	*/
	HNODE GetNode(void) const;

	/**
	 * @brief ���߽��
	*/
	HNODE Detach(void)
	{
		HNODE node = m_hNode;
		m_hNode = NULL;
		return node;
	}

	void Attach(HNODE node)
	{
		HNODE o = m_hNode;		
		m_hNode = node;
		ReleaseM_lo(o);
	}
	
	/** ��HTREENODE ����һ��idֵ( add by loach 2009-06-08 )
	 *
	*/
	void SetNodeId(const SNodeID& inId);
	//void GetNodeId(SNodeID& outId) const;
	/**
	 * @param pOutlen ,����id�ַ�������
	*/
	const char* GetNodeId( int* pOutlen = 0 ) const;

	/** ����˽������ */
	void SetAttribute(IloAttribute* lpAttribute)
	{
		IloAttribute* old = m_pAttribute;
		if( lpAttribute )
		{
			m_pAttribute = lpAttribute;
			m_pAttribute->AddRef();
		}
		ReleaseM_lo(old);
	}

	/** ��ѯ˽������ */
	int QueryAttribute(IloAttribute** lpAttribute)
	{
		if( m_pAttribute )
		{
			(*lpAttribute) = m_pAttribute;
			m_pAttribute->AddRef();
			return 0;
		}
		return -1;
	}
	//------------------------------------------------------------------------//
	// for UI(������Ҫ��Ϣ��,add by loach , 2010-08-23
	int GetIndex(void) const{
		return m_uiInfo.iIndex;
	}
	void SetIndex(int index){
		m_uiInfo.iIndex = index;
	}
	void GetRect(RECT& rc) const{
		memcpy(&rc , &m_uiInfo.rc , sizeof(RECT));
	}
	void SetRect(const RECT& rc){
		memcpy(&m_uiInfo.rc , &rc , sizeof(RECT));
	}
	//------------------------------------------------------------------------//
	//
	// add by loach 2009-06-08

	/** ���һЩ˽����Ϣ[UI�ϵ�һЩֵ]
	 *  ��һ�����棬������ͬһ�����ݣ�������ı��ֵ�Ԫ�ز�һ�����Ϳ���ͨ����������
	 *  ��Ҫʱ��ͨ���ӿ�ȡ
	 *
	 * @param PNodeprivateCB �Ļص���(��Ҫ���������� ppֵ)
	 *
	 * @return -1 �����Ѿ�����
	*/
	int AddPrivate(const char* key , PrivateValue pp , PNodeprivateCB lpPrivateCB );

	/** ɾ�����˽����Ϣ
	 *
	 * @param pp if pp != NULL ����ɾ���Ƿ�����,Ȼ�����������pp���д���
	*/
	int RemovePrivate(const char* key , PrivateValue* pp);

	/**
	 * ��ѯ��Ӧ��ֵ.
	*/
	int QueryPrivate(const char* key , PrivateValue* pp);

protected:
	/** command ��Ӧ���� */
	int  OnCommandItem( UINT eCmd,void*param0=NULL);
	/**
	* @brief �޸����Ľ����
	*/
	void ModifyCounts(int nNodeCounts, int nRootCounts);
	/**
	* @brief �������ӽ��ļ���
	*/
	void ModifyChildLevel();

	/** ����Ҫ����������� */
	virtual IloNodeObjserver* GetObjserver(void){
		return NULL;
	}
private:
	/** ��ȡ�۲춼���� */
	IloNodeObjserver* GetNodeObjserver(void)
	{
		HTREENODE hParent = GetTopNode();
		return ( hParent ) ? hParent->GetObjserver() : NULL;
	}
	/** ���ü��� */
	void Counts(int nNodeCounts, int nRootCounts);
	/**
	* @brief �Ƚϻص�����
	*	
	* @return �Ƚϳɹ�,����ָ��ֵ,���򷵻�NULL
	*
	* @param item1
	* @param item2
	*/
	HTREENODE Compare(HTREENODE item1,HTREENODE item2,PNodeCompareCB lpCompareCB);
	
private:
	/** parent of the double link list. if m_hParent is NULL,then this item is root	*/
	HTREENODE m_hParent;

	/** child of the double link list */
	HTREENODE m_hChild;

	/** next sibling of the double link list */
	HTREENODE m_hNext;

	/** prev sibling of the double link list */
	HTREENODE m_hPrev;

	/** CloTree�����Լ�һЩ˽������(����ָHNODE������ֵ) */
	IloAttribute* m_pAttribute;

	/** counts of the �����ӽ��(�������и����) */
	int m_nChildCounts;

	/** counts of �����ӽ����Ϊ���Ľ�� */
	int m_nRootCounts;

	/** ��ȼ���*/
	unsigned short m_nLevel;

	/** ������� */
	enNodeType m_NodeType;

	/** ��ο� enTNFlag */
	UINT m_nFlags;

	/** ������ */
	HNODE m_hNode;

	/** Ϊ��UI������ȡ�ı���ֵ */
	struct tagUIInfo
	{
		int  iIndex;
		RECT rc;
	};
	tagUIInfo m_uiInfo;

	/** ����˽����Ϣ��ȡ */
	friend class CloTreePrivate;
	CloTreePrivate* private_;

	/// һЩ�����Ĳ���
	friend class CloTreeFunction;
};

END_NAMESPACE(locom)

#endif /* __loNODE_H__ */
