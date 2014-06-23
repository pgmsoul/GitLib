
// LayoutView.h : CLayoutView 类的接口
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
protected: // 仅从序列化创建
	CLayoutView();
	DECLARE_DYNCREATE(CLayoutView)

// 属性
public:
	CLayoutDoc* GetDocument() const;
	void Reset();
	void Refresh(cs::Layout* sel = 0);

// 操作
public:
	Pad pad;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CLayoutView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // LayoutView.cpp 中的调试版本
inline CLayoutDoc* CLayoutView::GetDocument() const
   { return reinterpret_cast<CLayoutDoc*>(m_pDocument); }
#endif

