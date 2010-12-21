///////////////////////////////////////////////////////
//	***             * * * *      ****        ****    //
//	***  	      *         *     **          **     //
//	***  	     *           *    **          **     //
//	***  	     *           *    **************     //
//	***	     **  *           *    **          **     //
//	***********   *         *     **          **     //
//	***********	    * * * *      ****        ****    // 
///////////////////////////////////////////////////////2009-08-20 @loach
/*
*
* �˿�����ɶ�������ѧϰ֮��,����������ҵ��;
* ���������ҵ��;,һ�鷨��׷���Լ��е�!
*
* ���ߣ� loach.h ( ������)
*
* ʱ�䣺 2009-09-21
*/

class ParserT_lo;
class loAssocRowT;

/// �з����ص�����
/// cspacing �ָ�����( , # ... )
typedef int (*PLineParseCBT_lo)(ParserT_lo* lpCompiler , CharT cspacing);

/**
* @brief �ַ��б������ӿ���,������ٵ��ҵ���Ӧ��ֵ,�����ڴ�,��ȡ�ٶ�
* ��������Ҫ�Ľӿڽ��б���
* 
* ʹ��ʱע�� : AddRef() �� Release() �����ĵ���
*/
class loCOMMON_API ParserT_lo
{
public:
	/// construcot
    ParserT_lo();
	/// destructor
    ~ParserT_lo();
public:
	/**
	 * @brief ��������
	*/
	int     Handle(const CharT* value,bool bDefCB = true );
	/**
	 * @brief ��Ҫ���������
	*/
	int     Handle(const CharT* lpszTxt ,size_t len,bool bDefCB = true );
	/**
	 * @brief ����
	 * @param pCB �ص�����
	 * @param cspacing �ָ�����
	*/
	int     HandleCB(PLineParseCBT_lo pCB , CharT cspacing = L',' );

	/**
	 * @brief �ж��ٲ���ֵ 
	*/
	size_t  Counts(void) const;

	/**
	* @brief ��ȡ�ַ�����ֵ
	* @param nRow ����ֵ
	* @param lpszDefault Ĭ��ֵ
	*/
	const CharT*  String(size_t nRow , int& len,const CharT *lpszDefault = NULL);
	/**
	* @brief ��ȡ��������ֵ
	* @param nRow ����ֵ
	* @param nDefault Ĭ��ֵ
	*/
	int    Int(size_t nRow , int nDefault = 0);

	/**
	* @brief ��ȡ˫������������ֵ
	* @param nRow ����ֵ
	* @param dDefault Ĭ��ֵ
	*/
	double Double(size_t nRow , double dDefault = 0.0);
	/**
	* @brief ֵ
	*/
    const CharT* c_str(void);
	/**
	 * @brief ����
	*/
	size_t Length(void);
	/**
	 * @brief ���
	*/
    void  Clear();

	/*
	 * @brief λ�õ�
	*/
	void push(short row);
private:
	friend class loAssocRowT;
	loAssocRowT* _pAssoc;
};
