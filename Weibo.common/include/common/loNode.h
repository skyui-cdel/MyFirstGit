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

// 前向声明
class CloTreePrivate;


/// OnCommandItem(HTREENODE hNode , UINT eCmd,void*param0=NULL) 函数的命令值
enum enItemCommand
{
	CMD_ITEM_RELEASE, ///< CloTree 析构函数调用
	CMD_ITEM_ADD,     ///< hTreeNode 添加结点者, eCmd=CMD_ITEM_ADD , param0 =  被添加的对象(HTREENODE)
	CMD_ITEM_DELETE,  ///< hTreeNode 删除结点者, eCmd=CMD_ITEM_DELETE , param0= 被删除的对象(HTREENODE)
};

/// 单结点对象指针
typedef class CloNode* HNODE;

/// 树结点对象指针
typedef class CloTree* HTREENODE;

/**
 * @brief 结点实现类(不能实例化,IloAttribute 方法没有实现)
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
	/// 屏蔽掉这种方式的copy
	CloNode(const CloNode& n)
		:CloUnknown(NULL)
	{}	
	CloNode& operator = (const CloNode& n){ return *this;}
private:
	/** 响应命令(主要为了当 HTREENODE 发生变化是，要让HNODE知道,好让其通知发生什么变化 )
	 * @param eCmd ( 可参考 enItemCommand )
	 * @param hNode this的所属者
	*/
	virtual int OnCommandItem(HTREENODE hNode , UINT eCmd , void* param0=NULL){
		return -1;
	}
private:
	friend class CloTree;
};

/** 添加一个通知事件接口对象
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
		ENC_NODE_Remove,///< 调用 DetachNode() 通知事件在其父结点移走一个结点 ,param1= htreenode ,param0 = htreenode->Parent,param2=0
		ENC_NODE_Add,   ///< 调用 InsertSiblingNode(),InsertChildNode(), AttachNode() 通知事件在其父结点添加一个结点 ,param1= htreenode ,param0 = htreenode->Parent,param2=0
		ENC_NODE_Delete,///< 调用 DeleteNode() 通知事件在老的父结点删除一个结点,,param1= htreenode ,param0 = htreenode->Parent[老的结点],param2 =0
		ENC_NODE_Flags_update, ///< SetFlags(UINT nFlags) 发生变化 param0=htreenode,param1 = 新的&int,param2 = 老的&int
		ENC_NODE_Flags_bit,///< SetFlags(bits,isTrue)  发生变化 param0=htreenode,param1 = &bits,param2 = &isTrue
	};
	virtual void OnNotify(UINT eNotify,void* param0,void* param1,void* param2) = 0;
};

/// 比较的回调函数
typedef int  (*PNodeCompareCB)(HTREENODE node1,HTREENODE node2);

/// add by loach 2009-06-09私有处理的回调,在RemovePrivate会回调用一下[ AddPrivate() ]
typedef void (*PNodeprivateCB)(const char* key,void* pp);

/// 查询的条件
typedef bool (*PNodequeryCB)(HTREENODE hTree,void* pQueryCondition);

/// node id 值
typedef CloString SNodeID;

/**
* @ingroup locom
*
* @brief 单枝树结点的类
*
* 插入结点时,必须在外部进行分配内存,它内部不作唯一性检测,
* 如果需要进行检测唯一性,请在插入之前,调用IsHasNode(...)
* 最顶级根的结点对象m_hNode不参与其中,插入结点时,只会插入到子结点中去
*   root(最顶级根)
*    child <-- 插入
*   next  = null;
*   prev  = null;
*   parent= null;
*
* @author loach
*
* @date 2007-01-02
*
* @bug 不支持 "=" operator
*
* 
* @modified by loach 2009-06-09
*
*  1 对其结构进行一个 id含义的改创，这个id 与HNODE 不相关联的，由使用者决定其关系
*  2 添加 ReSortSliblingNode 函数
*  3 添加 CloTreePrivate 相关函数
*  4 为相应的接口添加IloNodeObjserver
*  
*/
class loCOMMON_API CloTree : public t_loPOSITION
{
public:
	typedef void* PrivateValue;

	/** 标志	 */
	enum enTNFlag 
	{
		TN_NULL        = 0x0000, ///<  空操作
		TN_VISIBLE_BIT = 0x0001, ///<  是否可见性,对根来说,与TN_EXPAND_BIT组合使用
		TN_SORT_BIT    = 0x0002, ///<  是否排序
		TN_CHECK_BIT   = 0x0004, ///<  是否被选择
		TN_EXPAND_BIT  = 0x0008, ///<  是否展开,主要用在根,控制下属子结点的可见性,如果这个标志位为0,下属不可见,1可见 
		TN_LOCK_BIT    = 0x0010, ///<  这个结点被锁了,主要用在根 ,不能进行Expand操作
		TN_ICON_EXPAND_BIT=0x0020,///< 图象也有expand功能,但TN_LOCK_BIT优先
		TN_SELECT_BIT     = 0x0040,///< 这项被选中，与checkbox不同
		TN_MOUSEON_BIT    = 0x0080,///< 鼠标在其上 
	};

	/// 结点类型
	enum enNodeType
	{
		NT_ROOT = 1, ///< 根结点
		NT_NODE = 2  ///< 叶子
	};

	/** 查询标志 */
	enum enQueryFlag
	{
		QF_ROOT=0x01,///<只查询根
		QF_NODE=0x02,///<只查询结点
		QF_ALL =0x03,///<所有
	};

	/// check 标志
	enum enCheckFlag
	{
		CF_REVERSE=0,///<反选
		CF_SELECT,///<选择
		CF_NOSELECT,///<不选
	};

	/// Constructor
	CloTree(const SNodeID& inId , enNodeType type = NT_ROOT , HNODE node = NULL , UINT nFlags = TN_VISIBLE_BIT);
	/// Destructor
	virtual ~CloTree();

private:
	/// 屏蔽掉这种方式的copy
	CloTree(const CloTree& t){}	
	CloTree& operator = (const CloTree& t){ return *this;}
public:
	/**
	* @brief 插入一个结点到同包兄弟,直接插入,没有查询是否存在,如果需要精确,请先调用IsHasNode()函数
	*
	* @param node 外界分配好的内存,可以实现多态功能,node保持它有生命周期性,不要轻意的释放
	* @param pInsNode ,插入的位置,if pInsNode!=NULL,确保 pInsNode 与 this 是同包的
	*
	* @param id 为每个HTREENODE 指定一个id( add by loach 2009-06-08)
	*/
	HTREENODE InsertSiblingNode(  enNodeType type,
		                          const SNodeID& inId,
		                          const HNODE node,								  
		                          const HTREENODE pInsNode = NULL,BOOL  bBack = TRUE ,PNodeCompareCB lpCompareCB = NULL);


	/** 
	* @brief 插入一个结点到子结点
	* 直接插入,没有查询子结点是否存在,如果需要精确,请先调用IsHasNode()函数
	*
	* @param node 外界分配好的内存,可以实现多态功能,node保持它有生命周期性,不要轻意的释放
	* @param pInsNode 插入点 ,if pInsNode!=NULL,确保 pInsNode 是 this 的子结点
	*
	* @param id 为每个HTREENODE 指定一个id( add by loach 2009-06-08)
	*/
	HTREENODE InsertChildNode(  enNodeType type,
		                        const SNodeID& inId,
		                        const HNODE node,								
		                        const HTREENODE pInsNode=NULL,BOOL bBack = TRUE ,PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief 删除一个结点,释放内存
	*/
	int DeleteNode(HTREENODE hNode);

	/**
	* @brief 移走一个结点,不是释放
	*
	* @param pObjserver 观察者(可以通过化通知都结点删除了 )
	*/
	int DetachNode(const HTREENODE hNode);

	/**
	* @brief 添加一个结点到子结点,HNODE必须在对象生命周期内有效,不能是一个局部变量
	* 可能HNODE 可能是不只一个结点,可能有很多兄弟结点,有子结点
	*/
	int AttachNode(HTREENODE hNode , PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief drag src node to dest node
	*
	* 拖动结点把 srcNode 结点移到 destNode 结点
	*
	* @param bRoot 是否拖动根,=false 如果srcNode是根,将返回 -3\n
	*                   = true 如果srcNode是根,也进行拖动
	* @return 0 成功\n
	*  -1 表示为NULL\n
	*  -2 它是是兄弟\n
	*  -3 srcNode是根
	*/
	int  DragNode(HTREENODE srcNode,
		          HTREENODE destNode,
				  BOOL bRoot = FALSE,
				  PNodeCompareCB lpCompareCB = NULL);

	/**
	* @brief 重新对其同级所有结点进行排序
	*
	*/
	int ReSortSliblingNode(PNodeCompareCB lpCompareCB);
	/**
	* @brief 设置结点选中
	* @param nFlag 选中标志,请参考 enCheckFlag
	*/
	void CheckNode(int nFlag = CloTree::CF_REVERSE);

	/**
	* @brief 设置结点选中，但是不修改子元素
	* @param nFlag 选中标志,请参考 enCheckFlag
	*/
	void CheckSelfNode(int nFlag = CloTree::CF_REVERSE);
	/**

	* @brief 是否有这样的结点存在
	* 搜索整个链表,包括子结点,兄弟结点
	*/
	BOOL IsHasNode(const HTREENODE hNode);

	/** 检测hNode是否些结点的父结点（包括所有上级的父结点)
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
	 *   调用 this->QueryPNode(hNode) 就会返回true
	*/
	BOOL QueryPNode(const HTREENODE hNode);

	/** 获取实例结点(就是最上级结点) */
	HTREENODE GetTopNode(void);

	/** 
	* @brief get the special item sid(唯一的标志),是否存在相同的sid。包括下属的子树
	*
	* @param lpid （2009-06-08 by loach 对其所定义的含义进行修正 )
	*              所指不再是 HNODE 对应的值呢，而是为特指 HTREENODE 所指定的 id
	*              node 与 lpid 是两个东西
	*
	* @param node 根据这个找
	*
	* @param nFlags 请参考 enQueryFlag 
	* == QF_ROOT 只找根 \n
	* == QF_NODE 只找结点（不是根)\n
	* == QF_ALL  所有\n
	*
	* @param pQueryCB 过滤查询满足的条件
	*/
	HTREENODE IsHasNode( const SNodeID& inId ,
		                 UINT nQFlag = QF_NODE , 
						 PNodequeryCB pQueryCB=NULL , void* pQueryCondition = NULL);

	HTREENODE IsHasNode( const HNODE node ,
		                 UINT nQFlag = QF_NODE , 
						 PNodequeryCB pQueryCB=NULL,void* pQueryCondition = NULL);
	/** 
	* @brief 是否存在子结点
	*/
	BOOL IsHasChilds() const;
	/** 
	* @brief 是否是根 
	*/
	BOOL IsRoot() const;
	/**
	* @brief 获取父结点(一级的父结点，上下级关系)
	*/
	HTREENODE GetParentNode() const;
	/**
	* @brief 获取下一个同包兄弟结点
	*/
	HTREENODE GetNextNode() const;
	/**
	* @brief 获取上一个同包兄弟结点
	*/
	HTREENODE GetPrevNode() const;
	/**
	* @brief 获取子结点
	*/
	HTREENODE GetChildNode() const;
	/**
	* @brief 获取同包兄弟的尾部结点 LOOP GetNextNode()  直到空
	*/
	HTREENODE GetTailNode() const;
	/**
	* @brief 获取同包兄弟的头部结点 LOOP GetPrevNode()  直到空
	*/
	HTREENODE GetHeadNode() const;
	/**
	* @brief Retrieves the next item having the given relationship with the
	* specified item.
	* 从hNode 开始,往下获取,可以遍历所有指定结点
	*
	* @param nFlag 请参数enTNFlag ,== 0 表示所有结点
	* @param nQFlag 请参考 enQueryFlag		
	*/
	HTREENODE GetNextNode(HTREENODE hNode,
		                  UINT nFlag = TN_NULL,
		                  UINT nQFlag = QF_ALL,
						  PNodequeryCB pQueryCB = NULL,void* pQueryCondition = NULL) const;

	/**
	* @brief 获取所有结点的数量(包括根结点在内)
	*/
	int   GetNodeCounts() const;
	/**
	* @brief 获取根结点的数量(不包括子结点在内)
	*/
	int   GetRootCounts() const;
	/** 
	* @brief 删除所有子结点
	*/
	void  RemoveAllChild(void);
	/**
	* @brief 设置标志
	*
	* @param bNotify =TRUE，希望内部调用 IloNodeObjserver接口来通知
	*/
	void  SetFlags(int bits , bool isTrue);
	void  SetFlags(int nFlags);
	/**
	* @brief 获取标志
	*/
	int   GetFlags() const;
	BOOL  GetFlags(int bits) const;
	/** 
	* @brief 获取深度级别 
	*/
	int   GetLevel(void) const;
	/**
	* @brief 设置深度级别
	*/
	void  SetLevel(unsigned short level);

	/** 
	* @brief 添加属性值 
	* @return 正常(失败 false ,成功 true )
	*/
	int  Add(UINT nKey ,IloAttribute::AttrValue attr,IloAttribute::AttrParam param=NULL)
	{
		return ( m_hNode )? m_hNode->Add(nKey,attr,param): -1 ;
	}
	/**
	* @brief 查询属性值 
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
	* @brief 获取结点对象指针
	*/
	HNODE GetNode(void) const;

	/**
	 * @brief 移走结点
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
	
	/** 对HTREENODE 设置一个id值( add by loach 2009-06-08 )
	 *
	*/
	void SetNodeId(const SNodeID& inId);
	//void GetNodeId(SNodeID& outId) const;
	/**
	 * @param pOutlen ,输入id字符串长度
	*/
	const char* GetNodeId( int* pOutlen = 0 ) const;

	/** 设置私有属性 */
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

	/** 查询私有属性 */
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
	// for UI(绘制需要信息）,add by loach , 2010-08-23
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

	/** 添加一些私有信息[UI上的一些值]
	 *  如一个界面，可以用同一个数据，但所需的表现的元素不一样，就可以通过个存起来
	 *  需要时，通过接口取
	 *
	 * @param PNodeprivateCB 的回调用(主要是用来管理 pp值)
	 *
	 * @return -1 表是已经存在
	*/
	int AddPrivate(const char* key , PrivateValue pp , PNodeprivateCB lpPrivateCB );

	/** 删除这个私有信息
	 *
	 * @param pp if pp != NULL 可以删除是返回来,然后由外面对其pp进行处理
	*/
	int RemovePrivate(const char* key , PrivateValue* pp);

	/**
	 * 查询相应的值.
	*/
	int QueryPrivate(const char* key , PrivateValue* pp);

protected:
	/** command 响应操作 */
	int  OnCommandItem( UINT eCmd,void*param0=NULL);
	/**
	* @brief 修改它的结点数
	*/
	void ModifyCounts(int nNodeCounts, int nRootCounts);
	/**
	* @brief 修正它子结点的级别
	*/
	void ModifyChildLevel();

	/** 请需要重载这个函数 */
	virtual IloNodeObjserver* GetObjserver(void){
		return NULL;
	}
private:
	/** 获取观察都对象 */
	IloNodeObjserver* GetNodeObjserver(void)
	{
		HTREENODE hParent = GetTopNode();
		return ( hParent ) ? hParent->GetObjserver() : NULL;
	}
	/** 设置计数 */
	void Counts(int nNodeCounts, int nRootCounts);
	/**
	* @brief 比较回调函数
	*	
	* @return 比较成功,返回指针值,否则返回NULL
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

	/** CloTree本身自己一些私有属性(不是指HNODE的属性值) */
	IloAttribute* m_pAttribute;

	/** counts of the 所有子结点(包括其中根结点) */
	int m_nChildCounts;

	/** counts of 所有子结点中为根的结点 */
	int m_nRootCounts;

	/** 深度级别*/
	unsigned short m_nLevel;

	/** 结点类型 */
	enNodeType m_NodeType;

	/** 请参考 enTNFlag */
	UINT m_nFlags;

	/** 结点对象 */
	HNODE m_hNode;

	/** 为了UI操作存取的变量值 */
	struct tagUIInfo
	{
		int  iIndex;
		RECT rc;
	};
	tagUIInfo m_uiInfo;

	/** 其它私有信息存取 */
	friend class CloTreePrivate;
	CloTreePrivate* private_;

	/// 一些函数的操作
	friend class CloTreeFunction;
};

END_NAMESPACE(locom)

#endif /* __loNODE_H__ */
