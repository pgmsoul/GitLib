
// LayoutDoc.h : CLayoutDoc ��Ľӿ�
//


#pragma once


class CLayoutDoc : public CDocument
{
protected: // �������л�����
	CLayoutDoc();
	DECLARE_DYNCREATE(CLayoutDoc)

// ����
public:
	cs::TopLayout	topLay;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CLayoutDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

extern CLayoutDoc* glbDoc;
