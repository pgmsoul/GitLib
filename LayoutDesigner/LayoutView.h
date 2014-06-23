
// LayoutView.h : CLayoutView ��Ľӿ�
//


#pragma once

class Pad : public cs::View{
protected:
	cs::Pen outLine,selLine;
	cs::Font	font;

	void onDraw(cs::DC* dc,cs::View*);
	void drawLayout(cs::DC* dc,cs::Layout* lay);
public:
	cs::Layout*			selLay;
	Pad();
};
class CLayoutView : public CView
{
protected: // �������л�����
	CLayoutView();
	DECLARE_DYNCREATE(CLayoutView)

// ����
public:
	CLayoutDoc* GetDocument() const;
	void Reset();
	void Refresh(cs::Layout* sel = 0);

// ����
public:
	Pad pad;
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CLayoutView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
extern CLayoutView* glbView;

#ifndef _DEBUG  // LayoutView.cpp �еĵ��԰汾
inline CLayoutDoc* CLayoutView::GetDocument() const
   { return reinterpret_cast<CLayoutDoc*>(m_pDocument); }
#endif

