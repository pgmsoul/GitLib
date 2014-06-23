#pragma once
//�������Ҫ��Ӧ�ؼ����¼���ֻ�ǵ��ÿؼ��Ĺ��ܺ���������ʹ������ӿڡ�

namespace cs{
	/*PBS_NORMAL = 1,PBS_HOT = 2,PBS_PRESSED = 3,PBS_DISABLED = 4,PBS_DEFAULTED = 5,PBS_STYLUSHOT = 6*/
	interface CONCISEUI_API ButtonPtr : virtual public WndPtr{
	public:
		ButtonPtr(){}
		ButtonPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���ذ�ť��ʾ��������ĳߴ�.
		bool GetIdealSize(LPSIZE ps){return SendMessage(BCM_GETIDEALSIZE,0,ps)!=0;}
		//��ȡ��ť��ѡ��״̬,������3��ֵ:0(δѡ��),1(ѡ��),2(��ȷ��).
		int GetCheck(){return (int)(INT_PTR)SendMessage(BM_GETCHECK,0,0)!=0;}
		//���ð�����ѡ��״̬.//BST_UNCHECKED(0),BST_CHECKED(1),BST_INDETERMINATE(2);
		void SetCheck(int state = BST_CHECKED){SendMessage(BM_SETCHECK,(WPARAM)state,0);}
		//����һ��ImageList��Ϊ��ť��ʾ��ͼ���б�.
		bool SetImageList(HIMAGELIST hImgl,UINT align = BUTTON_IMAGELIST_ALIGN_LEFT,LPRECT margin = 0);
		//���ذ�����ͼ��������ʾ����.
		bool GetImageList(BUTTON_IMAGELIST& bi){return SendMessage(BCM_GETIMAGELIST,0,&bi)!=0;}
		//�����ı���λ�ò���.
		bool SetTextMargin(int left,int top,int right,int bottom){return SendMessage(BCM_SETTEXTMARGIN,0,&left)!=0;}
		//��ȡ�ı���λ�ò���.
		bool GetTextMargin(LPRECT margin){return SendMessage(BCM_GETTEXTMARGIN,0,margin)!=0;}
		//ģ�ⰴ�°�ť����.
		void Click(){SendMessage(BM_CLICK,0,0);}
	};
	interface CONCISEUI_API ListBoxPtr : virtual public WndPtr{
	public:
		ListBoxPtr(){}
		ListBoxPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���һ���ִ�.str��Ҫ��ӵ��ִ�,index������λ��.�����ִ����б��е�����.
		inline int AddString(LPCWSTR str,int index = -1);
		//ɾ��һ���ִ�.index��Ҫɾ�����ִ�������.
		inline bool DeleteString(int index)					{return (int)SendMessage(LB_DELETESTRING,(WPARAM)index,0)!=0;}
		//����һ���ִ�,begin�ǿ�ʼ���ҵ�����,�����ִ���������-1.�������ֻҪ����ƥ��Ϳ���,���Ҳ��ִ�Сд.
		inline int FindString(LPCWSTR str,int begin = -1)	{return (int)SendMessage(LB_FINDSTRING,(WPARAM)begin,(LPARAM)str);}
		//��ȷ����,����Ҫ�������ִ�ƥ��,��Ҫ���ִ�Сд.
		inline int FindExactString(LPCWSTR str,int begin = -1)	{return (int)SendMessage(LB_FINDSTRINGEXACT,(WPARAM)begin,(LPARAM)str);}
		//����Ԫ������.
		inline int GetCount()								{return (int)SendMessage(LB_GETCOUNT,0,0);}
		//�ж����Ƿ��Ѿ���ѡ��.
		inline bool ItemIsSelect(int index)					{return (int)SendMessage(LB_GETSEL,(WPARAM)index,(LPARAM)0)!=0;}
		//����ѡ���������,��-1.�����ڵ��б�.
		inline int GetSelItem()								{return (int)SendMessage(LB_GETCURSEL,0,0);}//this is only use for single select list box,for multisel it return value is the focused item(but single select is always select item);
		//�����б����Ӧ������ѡ��,���index = -1��û���ѡ��.�����ڵ�ѡ�б�.
		inline void SetSelItem(int index)					{SendMessage(LB_SETCURSEL,(WPARAM)index,(LPARAM)0);}//for single select list box, must one item is selected.
		//�����ض��ִ�Ϊѡ��״̬.
		inline int SetSelItem(LPCWSTR str,int start = -1)	{return (int)SendMessage(LB_SELECTSTRING,(WPARAM)start,(LPARAM)str);}
		//��ȡ��ѡʱ��ѡ��������,selҪ�����㹻�Ĵ�С.����ѡ���������.�������ֻ�����ڶ�ѡ�б�,���ڵ�ѡ�б�,���Ƿ���-1.
		inline int GetSelItems(Memory<int>* sel)		{return (int)SendMessage(LB_GETSELITEMS,(WPARAM)sel->Length(),(LPARAM)sel->Handle());}//this is only use for multy select style,for single select style it is always return -1;
		//���ö�ѡ�б��ѡ�з�Χ,ֻ�����ڶ�ѡ�б�.���lastС��first,����һ��Χ��ѡ����ᱻ���.��������Ҫע��,�����������
		//��first��last��ͬ,������ʧ��.��Χѡ��������.�����ֻѡ��һ����Բ��ý����İ취,��ѡ2��,�����һ��.��ʹ����LBS_EXTENDEDSEL���
		//���б�,Ҳ���Ժ��������ѡȡ�������,����һ������.
		inline int SetSelItems(WORD first,WORD last)		{return (int)SendMessage(LB_SELITEMRANGEEX,(WPARAM)first,(LPARAM)last);}//if last is less than first,the range is removed from select.
		//��ȡѡ���������.
		inline int GetSelCount()							{return (int)SendMessage(LB_GETSELCOUNT,0,0);}//this is only use for multy select style,for single select style it is always return -1;
		//ȡ����Ӧ��������ı�,����Ԥ����GetTextLengthȡ���ı�����ռ��С,text�Ŀռ����Ҫ���ڷ���ֵ��1.����ȡ���ı��ĳ��Ȼ�-1.
		int GetText(int index,String& buf);
		//��������ָ������ı���Window��û���ṩLB_SETTEXT��Ϣ����������ڲ�ͨ��ɾ��������ִ����������ܡ�
		bool SetText(int index,LPCWSTR str);
		//ȡ����Ӧ��������ı�����,��������βNULL�ַ�.
		inline int GetTextLength(int index)					{return (int)SendMessage(LB_GETTEXTLEN,(WPARAM)index,0);}//�����ı�����.
		//��ȡ�ض�λ�õ��������ֵ.
		inline int GetIndexByPosition(Twin16 pos)	{return (int)SendMessage(LB_ITEMFROMPOINT,0,(LPARAM)pos.Value);}
		//���������.
		inline void Clear()									{SendMessage(LB_RESETCONTENT,0,0);}
		//���ö����б�ؼ��Ŀ��.
		inline void SetColumnWidth(int width)				{SendMessage(LB_SETCOLUMNWIDTH,(WPARAM)width,0);}
		//����Ԫ�صĻ��Ƹ߶�,�������������Ҫ�ػ�ؼ�..
		inline bool SetItemHeight(int height)				{return (SendMessage(LB_SETITEMHEIGHT,0,(LPARAM)height)!=LB_ERR);}
		//��ȡ������(������).
		inline int GetTopItem()								{return (int)SendMessage(LB_GETTOPINDEX,0,0);}
		//���ö�����.
		inline bool SetTopItem(int index)					{return (SendMessage(LB_SETTOPINDEX,(WPARAM)index,0)!=-1);}
		//ListBox�����Զ�����ˮƽ������.
		inline void SetHScrollWidth(int width)				{SendMessage(LB_SETHORIZONTALEXTENT,(WPARAM)width,0);}
		//����ͨListBoxת����DragListBox��
		inline bool MakeDrglist()							{return 0!=::MakeDragList(_Handle);}
	};
	interface CONCISEUI_API LabelPtr : virtual public WndPtr
	{
	public:
		LabelPtr(){}
		LabelPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
	};
	//Button Control
	/*ֻ��XP���İ�ť�ſ�������ͼ���б�ؼ�,���ͼ���б�ؼ�ֻ��һ��ͼ��,����ʾ���ͼ��.����ж���1��ͼ����Ӧ����
	5��ͼ��,�ֱ�������,�н���,���ͣ��,DISABLE״̬�Ͱ���״̬����ʾ*/
	//Edit Control
	interface CONCISEUI_API EditPtr : virtual public WndPtr
	{
	public:
		EditPtr(){}
		EditPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}

		//���ؿؼ����ڴ洢�ִ��Ŀռ�,ʹ��LocalLock������LocalUnlock��������ȡ���ͷ��ڴ���.��Ҫ������ڴ�д������,�ؼ����Լ���������ڴ��.
		HLOCAL LockTextBuffer(){return (HLOCAL)SendMessage(EM_GETHANDLE,0,0);}
		//����������ÿؼ����ִ�����,�������UNDO������MODIFY���.hMem�������Լ�������ڴ�(LocalLock),������ǰ��ĺ������ص��ڴ�.
		void SetTextBuffer(HLOCAL hMem){SendMessage(EM_SETHANDLE,hMem,0);}
		//���ÿؼ��Ƿ���ֻ������(ES_READONLY���).
		bool SetReadOnly(bool readonly){return SendMessage(EM_SETREADONLY,readonly,0)!=0;}
		//���������λ��.
		void ScrollCaret(){SendMessage(EM_SCROLLCARET,0,0);}
		//���ؿؼ��������ı�����.
		int GetLineCount(){return (int)SendMessage(EM_GETLINECOUNT,0,0);}
		//����֪�ı��滻ѡ���ı�.
		void ReplaceSelText(LPCWSTR str){SendMessage(EM_REPLACESEL,1,str);}
		//�����Ѿ�ѡ���ı�����ʼ�ͽ���λ��.
		Twin16 GetSelect(){return (Twin16)SendMessage(EM_GETSEL,0,0);}
		//����ѡ���ı�����ʼ�ͽ���λ��.
		void SetSelect(Twin16 sel){SendMessage(EM_SETSEL,sel.x,sel.y);}
		//��ʾһ��������ʾ��Ϣ.
		void ShowToolTip(LPCWSTR tip,LPCWSTR title = 0,int tti = TTI_NONE);
		//������ʾ��������ʾ.
		void HideToolTip();
		//UNDO����
		bool UnDo(){return SendMessage(EM_UNDO,0,0)!=0;}
	};
	//DateTimePicker Control
	interface CONCISEUI_API DateTimePickerPtr : virtual public WndPtr
	{
	public:
		DateTimePickerPtr(){}
		DateTimePickerPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}

		HWND GetMonthCalControl()			{return DateTime_GetMonthCal(_Handle);}
		void SetFormat(LPCWSTR format)		{DateTime_SetFormat(_Handle,format);}

		COLORREF GetMonthCalColor(int mcsc = MCSC_BACKGROUND)	{return (int)(INT_PTR)DateTime_GetMonthCalColor(_Handle,mcsc);}
		COLORREF SetMonthCalColor(COLORREF color,int mcsc = MCSC_BACKGROUND)	{return (int)(INT_PTR)DateTime_SetMonthCalColor(_Handle,mcsc,color);}

		HFONT GetMonthCalFont()				{return (HFONT)DateTime_GetMonthCalFont(_Handle);}
		void SetMonthCalFont(HFONT font,bool redraw = 0)		{DateTime_SetMonthCalFont(_Handle,font,redraw);}

		DWORD GetRange(LPSYSTEMTIME pst)	{return DateTime_GetRange(_Handle,pst);}//pst��һ��������SystemTimeԪ�������ָ��.
		bool SetRange(LPSYSTEMTIME pst,DWORD flag = GDTR_MIN|GDTR_MAX)			{return DateTime_SetRange(_Handle,flag,pst)!=0;}

		DWORD GetTime(LPSYSTEMTIME pst)		{return DateTime_GetSystemtime(_Handle,pst);}
		bool SetTime(LPSYSTEMTIME pst,DWORD flag = GDT_VALID)	{return DateTime_SetSystemtime(_Handle,flag,pst)!=0;}
	};
	//Calendar Control
	interface CONCISEUI_API MonthCalendarPtr : virtual public WndPtr
	{
	public:
		MonthCalendarPtr(){}
		MonthCalendarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
	};
	//ComboBox Control �������һ��ListBox�ؼ�,�����������ؼ��ĺܶຯ���Ǻ����Ƶ�.
	interface CONCISEUI_API ComboBoxPtr : virtual public WndPtr
	{
	public:
		ComboBoxPtr(){}
		ComboBoxPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���һ���ִ�.������ӵ�λ��.
		inline int AddString(LPCWSTR str,int index = -1)		{return (int)SendMessage(CB_INSERTSTRING,(WPARAM)index,(LPARAM)str);}//retrun index of insert item.
		//ɾ��һ���ִ�(ͨ������).
		inline bool DeleteString(int index)						{return SendMessage(CB_DELETESTRING,(WPARAM)index,0)!=-1;}
		//����ִ�.
		inline void Clear()										{SendMessage(CB_RESETCONTENT,0,0);}
		//�����ض�����ı�.
		inline bool SetItemText(int index,LPCWSTR str)			{return SendMessage(CB_SETITEMDATA,(WPARAM)index,(LPARAM)str)!=-1;}
		//�����ض�����ı�.
		bool GetItemText(int index,String& str);
		//��ȡԪ�ص�������.
		inline int GetCount()									{return (int)SendMessage(CB_GETCOUNT,0,0);}
		//���ص�ǰѡ���������.
		inline int GetCurSelect()								{return (int)SendMessage(CB_GETCURSEL,0,0);}
		//�����ض�������Ϊѡ��״̬.
		inline int SetCurSelect(int index)						{return (int)SendMessage(CB_SETCURSEL,(WPARAM)index,0);}
		//����һ���ִ�.
		inline int FindString(LPCWSTR str,int start = -1)		{return (int)SendMessage(CB_FINDSTRING,(WPARAM)start,(LPARAM)str);}
		//��ȷ����һ���ִ�.
		inline int FindExactString(LPCWSTR str,int start = -1)	{return (int)SendMessage(CB_FINDSTRINGEXACT,(WPARAM)start,(LPARAM)str);}
		//����ѡ����(ͨ���ִ�).
		inline int SelectString(LPCWSTR str,int start = -1)		{return (int)SendMessage(CB_SELECTSTRING,(WPARAM)start,(LPARAM)str);}
		//չ��������.
		inline void ShowDropDown(bool show = 1)					{SendMessage(CB_SHOWDROPDOWN,(WPARAM)show,0);}
		//ʹ�ض���ɼ�.
		inline bool EnsureVisible(int index)					{return (SendMessage(CB_SETTOPINDEX,(WPARAM)index,0)!=-1);}
		//�����û����Լ��������ַ�����,Ĭ��ֵ��30000.
		inline void SetEditLimit(int count)						{SendMessage(CB_LIMITTEXT,(WPARAM)count,0);}//count = 0 for no limit;
	};
	//CoboBoxEx Control ������ʾͼ��.ComboBoxEx��һ������ķ����CBES_EX_CASESENSITIVE,���Զ��г���Ա.
	interface CONCISEUI_API ComboBoxExPtr : virtual public WndPtr//ComboBoxEx��֧�������ִ��йصĺ���.
	{
	public:
		ComboBoxExPtr(){}
		ComboBoxExPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���ÿؼ���ͼ���б�ؼ�
		inline HIMAGELIST SetImageList(HIMAGELIST hImgl)	{return (HIMAGELIST)SendMessage(CBEM_SETIMAGELIST,0,(LPARAM)hImgl);}
		//��ȡ�ؼ���ͼ�񾲾��ؼ�.
		inline HIMAGELIST GetImageList()					{return (HIMAGELIST)SendMessage(CBEM_GETIMAGELIST,0,0);}
		//���һ����,str:�ı�,image:ͼ����ͼ���б�ؼ��е�����,indent:ͼ������ֵ����ƿ��,1��indent���Ϊ10��pix,
		//index:��ӵ�λ��,selImage:��ѡ��ʱ��ͼ������,lParam:���ӵĲ���.һ�������ǵ�image��selImage��ͬʱ�������ʾ����.
		int AddItem(LPCWSTR str,int image = -1,int indent = 0,int index = -1,int selImage = -1,LPARAM lParam = 0);
		//ɾ��һ��.
		inline bool DeleteItem(int index)					{return (SendMessage(CBEM_DELETEITEM,(WPARAM)index,0)==-1);}
		//���.
		inline void Clear()									{SendMessage(CB_RESETCONTENT,0,0);}
		//���ö�����,��Щ���ֻ�����������������,�����������ɲ���������.
		inline DWORD SetExStyle(DWORD cbs)					{return (DWORD)SendMessage(CBEM_SETEXSTYLE,0,(LPARAM)cbs);}
		//���ض�����.
		inline DWORD GetExStyle()							{return (DWORD)SendMessage(CBEM_GETEXSTYLE,0,0);}
		//���������.
		inline bool SetItem(PCOMBOBOXEXITEM pcbi)			{return (int)SendMessage(CBEM_SETITEM,0,(LPARAM)pcbi)!=0;}
		//��ȡ�����.
		inline bool GetItem(PCOMBOBOXEXITEM pcbi)			{return (int)SendMessage(CBEM_GETITEM,0,(LPARAM)pcbi)!=0;}
		//û�а취֪��Ҫȡ���ִ��ĳ���,���Ը�text����һ���ϴ��Capacity,�������ֻȡ���ִ��Ĳ���,��������º����Է���1.
		bool GetText(int index,String& text);
		//��index = -1ʱ,���õ��Ǳ༭����ı�.
		bool SetText(int index,LPCWSTR text);
		//��ȡ���ͼ������.
		int GetImage(int index,int flag = 0);// 0 for image,1 for selImage,2 for overlay.
		//�������ͼ������.
		bool SetImage(int index,int image,int flag = 0);
		//����ƽ��.
		bool SetIndent(int index,int indent);
		//����ƽ����
		int GetIndent(int index);
		//����Ԫ������.
		inline int GetCount()								{return (int)SendMessage(CB_GETCOUNT,0,0);}
		//����ѡ���������.
		inline int GetCurSelect()							{return (int)SendMessage(CB_GETCURSEL,0,0);}
		//����ѡ����.
		inline int SetCurSelect(int index)					{return (int)SendMessage(CB_SETCURSEL,(WPARAM)index,0);}
		//���ÿɼ��������.
		inline bool SetVisibleCount(int count)				{return (int)SendMessage(CB_SETMINVISIBLE,(WPARAM)count,0)!=0;}
		//��ȡѡ���������.
		inline int GetVisibleCount()						{return ComboBox_GetMinVisible(_Handle);}
		//���ñ༭�ؼ�������ַ���.
		inline void SetEditLimit(int count)					{SendMessage(CB_LIMITTEXT,(WPARAM)count,0);}//count = 0 for no limit;
		//��ǰ���ô洢�ռ�.
		inline bool ExpendStorage(int count,int strStorage)	{return SendMessage(CB_INITSTORAGE,(WPARAM)count,(LPARAM)strStorage)!=CB_ERRSPACE;}
	};
	//TreeView Control
	/*TreeView�ؼ�������ν��������ʾ�ؼ�,����Ԫ�������ηֲ�,����һ��Ӧ�÷ǳ��㷺�Ľṹ,�����ļ�ϵͳ.TreeView�ؼ����԰�
	����ͼ���б�ؼ�,һ��������ͨ״̬��,һ������ѡ��״̬��.*/
	interface CONCISEUI_API TreeViewPtr : virtual public WndPtr
	{
	public:
		TreeViewPtr(){}
		TreeViewPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//�������,parent:��ӵ�λ��,������TVI_ROOT.text:��ʾ���ı�,imgIndex:ͼ������,selIndex:ѡ��״̬�µ�ͼ������,insAfter:ͬ����������λ��.
		HTREEITEM AddItem(HTREEITEM parent,LPCWSTR text,int imgIndex,int selIndex,HTREEITEM tvi = 0);
		//ɾ������.
		bool DeleteItem(HTREEITEM hItem){return TreeView_DeleteItem(_Handle,hItem)!=0;}
		//������е���.
		bool Clear(){return TreeView_DeleteAllItems(_Handle)!=0;}
		//��������str�㹻������,����ֵ���ִ��ĳ���.
		int GetItemText(HTREEITEM hti,String& str);
		//����������ı�.
		bool SetItemText(HTREEITEM hti,LPCWSTR str);
		//ȡ�������ͼ������,flag������TVSIL_STATE��TVSIL_NORMAL֮һ.
		int GetImageIndex(HTREEITEM hti,UINT flag = TVSIL_NORMAL);
		//���������ͼ������,-1��ʾ�������������.
		bool SetImageIndex(HTREEITEM hti,int image,int selImage = -1);
		//ȡ�ÿؼ�ʹ�õ�ͼ���б�ؼ��ľ��,flag��������ѡֵ:TVSIL_NORMAL��TVSIL_STATE.
		HIMAGELIST GetImageList(UINT flag = TVSIL_NORMAL)					{return TreeView_GetImageList(_Handle,flag);}
		//���ÿؼ�ʹ�õ�ͼ���б�ؼ�,flag��������ѡֵ,TVSIL_NORMAL��TVSIL_STATE.
		HIMAGELIST SetImageList(HIMAGELIST hImgl,UINT flag = TVSIL_NORMAL)	{return (HIMAGELIST)SendMessage(TVM_SETIMAGELIST,flag,(HIMAGELIST)hImgl);}//����ImageList,����ѡ��ɾ��ǰһ��ImageList�򷵻�����.hImglΪ0������.
		//���ÿؼ��ı���ɫ,�������������Control���ͬ������,��Ҫ���Ǹ����������ñ���ɫ.-1ʹ��ϵͳ��ȱʡɫ.
		COLORREF SetBkgndColor(COLORREF color)	{return TreeView_SetBkColor(_Handle,color);}
		//��ȡ�ؼ��ı���ɫ.
		COLORREF GetBkgndColor()				{return TreeView_GetBkColor(_Handle);}
		//�����ı�����ɫ,�������ͬ��������Control���ͬ������.��Ϊ�Ǹ�����������ؼ�������.color = -1��ʾʹ��ϵͳ��ȱʡ��ɫ.
		COLORREF SetFontColor(COLORREF color)	{return TreeView_SetTextColor(_Handle,color);}
		//��ȡ�ı���ɫ.
		COLORREF GetFontColor()					{return TreeView_GetTextColor(_Handle);}
		//����ˮƽ�ߵĳ���,���С���������Сֵ,��Ϊ��Сֵ.
		void SetLineIndent(int width)			{TreeView_SetIndent(_Handle,width);}
		//����ˮƽ�ߵĳ���,ȱʡֵ��ԼΪ20.
		int GetLineIndent()						{return TreeView_GetIndent(_Handle);}
		//���û��ߵ���ɫ.
		COLORREF SetLineColor(COLORREF color)	{return TreeView_SetLineColor(_Handle,color);}
		//��ȡ���ߵ���ɫ.
		COLORREF GetLineColor()					{return TreeView_GetLineColor(_Handle);}
		//��ʼ�༭��ǩ,���ر༭�ؼ��ľ��.
		HWND BeginEdit(HTREEITEM hItem)			{return TreeView_EditLabel(_Handle,hItem);}
		//��ɱ�ǩ�༭,cancel = 1��ʾ��ԭ�ı�.
		bool EndEdit(bool cancel = 0)			{return TreeView_EndEditLabelNow(_Handle,cancel)!=0;}
		//��������;all--�Ƿ�Զ༶�����������.
		bool SortChildren(HTREEITEM hti,bool all = 0)	{return TreeView_SortChildren(_Handle,hti,all)!=0;}
		//ʹ�ض�����ɼ�.(ͨ��չ�������).��������ǹ���ʹ��ɼ�,����1,���򷵻�0.
		bool EnsureVisible(HTREEITEM hItem)				{return TreeView_EnsureVisible(_Handle,hItem)!=0;}
		//չ���������ض�����.
		bool Expand(HTREEITEM hItem,UINT tve = TVE_TOGGLE)	{return TreeView_Expand(_Handle,hItem,tve)!=0;}
		//��ȡ�󶨵ı༭�ؼ��ľ��.
		HWND GetEditControl()			{return TreeView_GetEditControl(_Handle);}
		//�����������.���Ϊ16λ���������ֵ.
		int GetItemCount()				{return TreeView_GetCount(_Handle);}
		//��ȡ��ľ���.
		bool GetItemRect(HTREEITEM hItem,LPRECT rect,bool onlyText = 1){return TreeView_GetItemRect(_Handle,hItem,rect,onlyText)!=0;}
		//��ȡ��ĸ߶�.
		int GetItemHeight(){return TreeView_GetItemHeight(_Handle);}
		//������ĸ߶�.
		int SetItemHeight(int height){return TreeView_SetItemHeight(_Handle,height);}		
		//����ToolTip�ؼ�.
		HWND SetToolTipCtrl(HWND hToolTip){return TreeView_SetToolTips(_Handle,hToolTip);}
		//ȡ�ù�����ToolTip�ؼ�.
		HWND GetToolTipCtrl(){return TreeView_GetToolTips(_Handle);}		
		//������Ĳ���
		bool SetItemInfo(LPTVITEMEX pti){return TreeView_SetItem(_Handle,pti)!=0;}
		//��ȡ��Ĳ���.
		bool GetItemInfo(LPTVITEMEX pti){return TreeView_GetItem(_Handle,pti)!=0;}
		//���õ�ǰѡ����Ϊָ����.
		bool SetSelectItem(HTREEITEM hti){return TreeView_SelectItem(_Handle,hti)!=0;}
		//��ȡ��Ķ������ݲ���.
		bool GetItemData(HTREEITEM hti,LPARAM* pParam);
		//������Ķ������ݲ���.
		bool SetItemData(HTREEITEM hti,LPARAM data);
		//���ظ�.
		HTREEITEM GetRoot()							{return TreeView_GetRoot(_Handle);}
		//�����������չ����.
		HTREEITEM GetLastItem()						{return TreeView_GetLastVisible(_Handle);}
		//���ص�һ������(��0���û��).
		HTREEITEM GetFirstChild(HTREEITEM hti)		{return TreeView_GetChild(_Handle,hti);}
		//��һ���ɼ���(�����Ǽ�),hitem����ɼ�.
		HTREEITEM GetNextVisible(HTREEITEM hti)		{return TreeView_GetNextVisible(_Handle,hti);}
		//ǰһ���ɼ���(�����Ǽ�),hitem����ɼ�.
		HTREEITEM GetPreVisible(HTREEITEM hti)		{return TreeView_GetPrevVisible(_Handle,hti);}
		//������һ��ͬ����.
		HTREEITEM GetNext(HTREEITEM hti)			{return TreeView_GetNextSibling(_Handle,hti);}
		//����ǰһ��ͬ����.
		HTREEITEM GetPrevious(HTREEITEM hti)		{return TreeView_GetPrevSibling(_Handle,hti);}
		//���ظ���ľ��;���û�и���,����0.
		HTREEITEM GetParent(HTREEITEM hti)			{return TreeView_GetParent(_Handle,hti);}
		//���ص�ǰѡ����.
		HTREEITEM GetSelectItem()					{return TreeView_GetSelection(_Handle);}
		//ȡ���ض�λ�õ��
		HTREEITEM GetPosItem(int x,int y)			{TVHITTESTINFO ti;ti.pt.x = x;ti.pt.y = y;return TreeView_HitTest(_Handle,&ti);}
		//����һ����ľ��,������tvgnȷ��.��ʵ������ǰ�����к������ܵ����,���Ҷ�������ǰ�����Щ����ȡ��.
		HTREEITEM GetItem(UINT tvgn,HTREEITEM hti = 0)	{return TreeView_GetNextItem(_Handle,hti,tvgn);}
		//��ȡ���CheckBoxѡ��״̬��0��δѡ�У�1��ѡ�У�-1��û��CheckBox��
		int GetItemChecked(HTREEITEM hti)			{return TreeView_GetCheckState(_Handle,hti);}
		//�������CheckBoxѡ��״̬��0��δѡ�У�1��ѡ�С�
		void SetItemChecked(HTREEITEM hti,bool check)	{TreeView_SetCheckState(_Handle,hti,(UINT)check);}

	protected:
		//void _OnNotify(LPCTRLMSGPARAM pcm);
	};
	//ToolBar Control
	//����AddButton�����toolBar.SendMessage(TB_SETMAXTEXTROWS,(WPARAM)0,0)��
	//��ť������ʾ���֣�������Ϊtooltip����ҪTBSTYLE_TOOLTIPS���
	//MSDN�ϵ� TBSTYLE_EX_MIXEDBUTTONS ������ TBN_GETINFOTIP ��Ч;
	interface CONCISEUI_API ToolBarPtr : virtual public WndPtr
	{
	public:
		ToolBarPtr(){}
		ToolBarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���ɿؼ�.
		INT_PTR Create(HWND hParent);
		//��Ӱ�ť,cmdID:��ť��WM_COMMAND��ϢID,image:ͼ�����б��е�����,text:��ʾ���ı�,tbn:��ť�ķ��,tbstate:��ť��״̬,index:��ӵ�λ��.
		bool AddButton(WORD cmdID,int image,LPCWSTR text,BYTE tbn = BTNS_BUTTON|BTNS_AUTOSIZE,BYTE tbstate = TBSTATE_ENABLED,int index = -1);
		//���һ���ָ���,index��λ������.
		bool AddSplit(int index = -1);
		//����ͼ���б�ؼ�,ToolBar�ؼ�����3��ͼ���б�ؼ�,0:��ͨ,1:DISABLE״̬,2:���ͣ��״̬.
		HIMAGELIST SetImageList(int type,HIMAGELIST hImgl);
		//��ȡͼ���б�ؼ�.0:��ͨ,1:DISABLE״̬,2:���ͣ��״̬.
		HIMAGELIST GetImageList(int type);
		//���ذ�����״̬,ͨ��ID����.
		inline int GetButtonState(int id)	{return (int)SendMessage(TB_GETSTATE,id,0);}
		//���ð�ť��״̬,ͨ��ID����.
		bool SetButtonState(int id,int state,DWORD option = 1);
		//ͨ��TBBUTTON�ṹ����һ����Ӷ����ť.
		inline bool AddButton(TBBUTTON* tbb,int count = 1){return SendMessage(TB_ADDBUTTONS,count,(LPARAM)tbb)!=0;}
		//ɾ���ض��İ�ť.
		inline bool DelButton(int index){return (int)SendMessage(TB_DELETEBUTTON,index,0)!=0;}
		//Enable��Disableһ����ť.
		inline bool EnableButton(int id,bool enable = 0){return (int)SendMessage(TB_ENABLEBUTTON,id,enable)!=0;}
		//���ػ���ʾһ����ť.
		inline bool HideButton(WORD id,bool hide = 1){return (int)SendMessage(TB_HIDEBUTTON,id,MAKELONG(hide,0))!=0;}
		//�л�һ����ť.
		inline bool CheckButton(WORD id,bool check = 1){return (int)SendMessage(TB_CHECKBUTTON,id,MAKELONG(check,0))!=0;}//ʹһ����ť����check״̬,�����������ť�Ƿ���һ��check��ť,����ⲻ��һ��check��ť,����ͨ��������ʹ�����ڷ�check״̬.
		//ģ�ⰴ������.
		inline bool PressButton(WORD id,bool pressed = 1){return (int)SendMessage(TB_PRESSBUTTON,id,MAKELONG(pressed,0))!=0;}//ʹһ����ť������ѹ״̬,����ͨ��������������״̬.
		//
		inline bool GetAnchorHighLight(){return (int)SendMessage(TB_GETANCHORHIGHLIGHT ,0,0)!=0;}
		//
		inline bool SetAnchorHighLight(bool high){return (int)SendMessage(TB_SETANCHORHIGHLIGHT,high,0)!=0;}
		//
		bool LoadBitmap(int type,int cx,COLORREF mask,ResID,bool delpre = 1);//�����ϣ��ǰһ��Image List���ͷ�,��delpre = 0.
		//
		inline bool SetIconSize(int cx,int cy){return (int)SendMessage(TB_SETBITMAPSIZE,0,MAKELONG(cx,cy))!=0;}//����ȱʡֵ��ͼ��ߴ�.
		//
		inline bool SetButtonSize(int cx,int cy){return (int)SendMessage(TB_SETBUTTONSIZE,0,MAKELONG(cx,cy))!=0;}//��AddButton֮�����,ֻ�ܵ���,����С��ͼ��߶�.
		//
		inline Twin16 GetButtonSize(){return (int)SendMessage(TB_GETBUTTONSIZE,0,0);}
		//
		bool GetButtonRect(int index,LPRECT lpRect,bool byIndex = 1);
		//
		bool SetLimitButtonWidth(int min,int max){return (int)SendMessage(TB_SETBUTTONWIDTH,0,MAKELONG(min,max))!=0;}
		//
		inline UINT GetStyle(){return (int)SendMessage(TB_GETSTYLE,0,0);}
		//����ToolBar�ķ��option = 0�����������option = 1���Ƴ�������option = 2������Ϊ������
		UINT SetStyle(UINT style,DWORD option = 0);
		//
		inline UINT GetExStyle(){return (int)SendMessage(TB_GETEXTENDEDSTYLE,0,0);}
		//����ToolBar�Ķ�����option = 0�����������option = 1���Ƴ�������option = 2������Ϊ������
		UINT SetExStyle(UINT exStyle,DWORD option = 0);
		//
		inline bool Refresh(){return (int)SendMessage(TB_AUTOSIZE,0,0)!=0;}
		//
		inline int Count(){return (int)SendMessage(TB_BUTTONCOUNT,0,0);}
		//
		inline int IndexOf(int id){return (int)SendMessage(TB_COMMANDTOINDEX,id,0);}
		//
		inline bool SetButtonIcon(int id,int image){return (int)SendMessage(TB_CHANGEBITMAP,id,image)!=0;}//I_IMAGENONE = -2
		//
		inline int GetButtonIcon(int id){return (int)SendMessage(TB_GETBITMAP,id,0);} 
		//
		inline bool SetButtonInfo(WORD id,LPTBBUTTONINFO pbi){return (int)SendMessage(TB_SETBUTTONINFO,id,pbi)!=0;}
		//
		inline int GetButtonInfo(WORD id,LPTBBUTTONINFO pbi){return (int)SendMessage(TB_GETBUTTONINFO,id,pbi);}
		//
		bool SetButtonInfo(WORD id,DWORD tbif,INT_PTR value);
		//
		INT_PTR GetButtonInfo(WORD id,DWORD tbif);//return -1 represnt false.-2 indecate no image,����TBIF_BYINDEX�������ֻ����һ��.
		//
		inline HWND GetToolTipCtrl(){return (HWND)SendMessage(TB_GETTOOLTIPS,0,0);}
		//
		inline HWND SetToolTipCtrl(HWND hToolTip){return (HWND)SendMessage(TB_SETTOOLTIPS,hToolTip,0);}
		//
		inline int GetButtonText(int id,LPWSTR buffer){return (int)SendMessage(TB_GETBUTTONTEXT,id,buffer);}
		//
		inline Twin GetBarSize(){Twin size(0,0);SendMessage(TB_GETMAXSIZE,0,&size);return size;}
	};
	//ProgressBar Control
	interface CONCISEUI_API ProgressBarPtr : virtual public WndPtr
	{
	public:
		ProgressBarPtr(){}
		ProgressBarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//����ֵ�����޺����޵ĵ�16ֵ.�����������ֵ��һ��32λֵ,��GetRange����ȡ��.
		inline Twin16 SetRange(int low,int hight){return (Twin16)SendMessage(PBM_SETRANGE32,low,hight);}
		//ȡ��32λ��������ֵ.
		inline void GetRange(Twin& range){SendMessage(PBM_GETRANGE,0,&range);}
		//���õ�ǰλ��
		inline void SetPosition(int pos){SendMessage(PBM_SETPOS,pos,0);}
		//ȡ�õ�ǰλ��.
		inline uint GetPosition(){return (uint)SendMessage(PBM_GETPOS,0,0);}
		//������С����.
		inline void SetIncrement(int delta){SendMessage(PBM_DELTAPOS,delta,0);}
		//���ò�������.
		inline void SetStepValue(int step){SendMessage(PBM_SETSTEP,step,0);}
		//ǰ��һ��������ֵ.
		inline void Step(){SendMessage(PBM_STEPIT,0,0);}
		//��ʼ��ֹͣ��������������û�У�PBS_MARQUEE������÷�Χ�������refresh��ˢ�����ڣ���λ�Ǻ��롣
		inline void SetMarquee(bool stop = 0,int refresh = 100){SendMessage(PBM_SETMARQUEE,!stop,refresh);}
	protected:
	};
	//ToolTip Control ����ؼ�����������6.0�汾�ؼ�ʱ�ſ���.Ҳ����˵,����Ҫ��MANIFEST.
	interface CONCISEUI_API ToolTipPtr : virtual public WndPtr
	{
	public:
		ToolTipPtr(){}
		ToolTipPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		INT_PTR Create(HWND hParent);
		//�رջ�򿪿ؼ�����ʾ����.
		inline void Enable(bool enable = 1){this->SendMessage(TTM_ACTIVATE,enable,0);}
		//��һ���������һ��ToolTip,owner:�����ͣ��ʱҪ��ʾTip�Ĵ���,text:��ʾ��Ϣ,id,pr,����������ָ������Ҫ��ʾ��ʾ
		//�������ID,��ΪҪ��ʾ��ͬ����ʾ,��ID����.ttf:��ʾ���.�ظ���ID(����HWND)����,�Ḳ����һ�����á�
		bool AddTip(HWND owner,LPCWSTR text,UINT id = 0,LPRECT pr = 0,UINT ttf = 0);
		//ɾ��һ����ʾ��
		void DelTip(HWND owner,UINT uID = 0);
		//������Ӧ��ʾ�Ĵ�������
		bool SetTipRect(LPRECT pr,HWND owner,UINT uID = 0);
		//����һ����ʾ����Ϣϸ��,��ΪTI�Ľṹ�Ƚϸ��ӣ��������������ȵ���GetTipInfo��ʼ��ti��ֵ����cbSize,hwnd �� uId �����ʼ����
		//Ȼ������Ҫ�ı�ĳ�Ա��ֵ���ٵ������������
		inline bool SetTipInfo(LPTOOLINFO ti){return SendMessage(TTM_SETTOOLINFO,0,&ti)!=0;}
		//ȡ��һ����ʾ����Ϣϸ��,ti��cbSize��Ա�����ʼ��,hwnd �� uId ��ԱҲ����ָ��,��Ϊ��Ϣ��������Ϊ������.
		inline bool GetTipInfo(LPTOOLINFO ti){return 0!=SendMessage(TTM_GETTOOLINFO,0,ti);}
		//ֻ�е�TTS_BALLOON�������ʱ����������ſ��á�title:Ҫ��ʾ�ı����ı�,tti��Ҫ��ʾ��ͼ�꣬0��û��ͼ�꣬1����Ϣ��2�����棬3������
		inline bool SetTitle(LPCWSTR title,int tti = TTI_NONE){return 0!=SendMessage(TTM_SETTITLE,tti,title);}
		//������ʾ���ı���
		bool SetTipText(LPCWSTR text,HWND owner,UINT id);
		//������ʾ�ķ��
		bool SetTipFlag(UINT ttf,HWND owner,UINT id);
		//������ʾ�ı���ɫ��
		inline bool SetTextColor(COLORREF color){return 0!=SendMessage(TTM_SETTIPTEXTCOLOR,color,0);}
		//������ʾ���ڵı�����ɫ��
		inline bool SetBkColor(COLORREF color){return 0!=SendMessage(TTM_SETTIPBKCOLOR,color,0);}
		//������ʾ���ڵĿ�ȡ�
		inline int SetTextWidth(UINT width){return (int)this->SendMessage(TTM_SETMAXTIPWIDTH,0,width);}
		//������ʾ��ʾ���ӳ�ʱ�䡣
		inline void SetDelayTime(UINT ms,UINT ttdt = TTDT_INITIAL){this->SendMessage(TTM_SETDELAYTIME,ttdt,ms);}
		//ǿ����ʾ�������»��ơ�
		inline void UpDate(){SendMessage(TTM_UPDATE,0,0);}
		//������ʾ����ʾʱ���������λ�Ǻ��롣flag:2-show,3-delay,1-reshow��0-all default.
		inline void SetTimeInfo(int t,UINT flag){SendMessage(TTM_SETDELAYTIME,flag,t);}
		//ȡ��ʱ�������flag:2-show,3-delay,1-reshow
		inline int GetTimeInfo(UINT flag){return (int)SendMessage(TTM_GETDELAYTIME,flag,0);}
	};

	//TabPage�ؼ��������ṩ�Կؼ��Ĳ���,��ֻ�ṩһ�����,Ȼ����ҳ��ť��������Ӧ����Ϣ�¼�.����Щ�¼��д���ؼ�����Ϊ,ʹ�����ֵ������ڴ��ڲ�ͬ��ҳ��.
	interface CONCISEUI_API TabPagePtr : virtual public WndPtr
	{
	public:
		TabPagePtr(){}
		TabPagePtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//����һ������ҳ,index�ǲ����λ������,title����ʾ�ı���,image��ͼ����ͼ���б�ؼ��е�����,param��һ���û��Զ������.
		bool Add(int index,LPWSTR title = 0,int image = -1,LPARAM param = 0);
		//ɾ��ָ����������.
		bool Delete(int index){return 0!=SendMessage(TCM_DELETEITEM,index,0);}
		//������Ŀ����.
		int GetCount(){return (int)SendMessage(TCM_GETITEMCOUNT,0,0);}
		//�����������ҳ.
		bool Clear(){return 0!=SendMessage(TCM_DELETEALLITEMS,0,0);}
		//���ص�ǰ����ҳ������.
		int GetCurSel(){return (int)SendMessage(TCM_GETCURSEL,0,0);}
		//����ָ������������ҳΪ��ǰҳ.
		void SetCurSel(int index){SendMessage(TCM_SETCURSEL,index,0);}
		//����ͼ���б�ؼ�.
		HIMAGELIST SetImageList(HIMAGELIST imgl){return (HIMAGELIST)SendMessage(TCM_SETIMAGELIST,0,imgl);}
		//ȡ��ͼ���б�ؼ�.
		HIMAGELIST GetImageList(){return (HIMAGELIST)SendMessage(TCM_GETIMAGELIST,0,0);}
	};
	typedef union HotkeyParam
	{
		struct{
			unsigned char VirtualKey,CombineKey;
		};
		struct{
			unsigned short Value;
		};
		struct{
			WPARAM wParam;
		};
		void* operator new(size_t size){return MemoryAlloc((int)size);}
		void operator delete(void* p){MemoryFree(p);}
	}HotkeyParam;
	//��ʾ��ϼ���һ���ṹ.
	//�ȼ��ؼ�
	interface CONCISEUI_API HotKeyPtr : virtual public WndPtr
	{
	public:
		HotKeyPtr(){}
		HotKeyPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//�������������Ч�ļ����,hkcomb��Ҫ���ε���ϼ�,�����Ƕ��HKCOMB_�ೣ�������,��������Щ���ʱ,�ᱻָ����hotkeyfȡ��.
		void SetRules(int hkcomb,int hotkeyf){SendMessage(HKM_SETRULES,MAKEWPARAM(hkcomb,0),MAKELPARAM(hotkeyf,0));}
		//������ϼ�.
		void SetHotKey(unsigned char comb,unsigned char vk);
		//������ϼ�.
		void SetHotKey(HotkeyParam hk){SendMessage(HKM_SETHOTKEY,hk.wParam,0);}
		//ȡ����ϼ�,��λ�Ǽ�,��λ�����.
		HotkeyParam GetHotKey();
		//�󶨵�һ������,���������Ӵ���.
		bool Register(HWND frame){return (int)SendMessage(WM_SETHOTKEY,GetHotKey().wParam,0)>0;}
	};
	interface CONCISEUI_API RebarPtr : virtual public WndPtr{
		RebarPtr(){}
		RebarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
	};
	interface CONCISEUI_API ListViewPtr : virtual public WndPtr
	{
	public:
		ListViewPtr(){}
		ListViewPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���һ���У������е�����λ�û���-1���������б���������ȣ�λ�ã�-1��ʾ��ӵ���󣩡�
		int AddColumn(LPCWSTR title,int width,int index = -1);
		//���һ����Ŀ����������һ�е��ı�����λ�ã�-1����ʾ��ӵ���󣬷���λ��������-1.
		int AddItem(LPCWSTR text,int index = -1);
		//����һ����Ŀ���ı����������ı�������������������
		bool SetItemText(LPCWSTR text,int index,int column);
		//���ð󶨵��������
		bool SetItemData(int index,INT_PTR data);
		//ɾ��ָ���������У�����0�������޷�ɾ�������Ҫɾ�����Ȳ���һ��0��ȵ��У���ɾ����
		bool DelColumn(int index){return 0!=this->SendMessage(LVM_DELETECOLUMN,index,0);}
		//ɾ��ָ���С�
		bool DelItem(int index){return 0!=this->SendMessage(LVM_DELETEITEM,index,0);}
		//������С�
		bool Clear(){return 0!=this->SendMessage(LVM_DELETEALLITEMS,0,0);}
		//�༭��ǩ��
		HWND EditItem(int index){SetFocus(_Handle);return (HWND)this->SendMessage(LVM_EDITLABEL,index,0);}
		//ȡ����ǩ�༭��
		void CancelEdit(){this->SendMessage(LVM_CANCELEDITLABEL,0,0);}
		//�����е�������
		int GetColumnCount();
		//�����е�������
		int GetItemCount(){return (int)this->SendMessage(LVM_GETITEMCOUNT,0,0);}
		//ȡ��ָ����column����index���ı���
		int GetItemText(String& text,int index,int column);
		//ȡ�������ض����������������falg ����ȷ��������ͺ�ѡ����� LVNI_SELECTED ����ȡ��ѡ�����������
		//after ָ����������һ��������濪ʼ��-1 ��ʾ���������ע��������ֵ�� 0 ������������0�
		int GetItemIndex(DWORD flag = LVNI_SELECTED,int after = -1){return (int)SendMessage(LVM_GETNEXTITEM,after,flag);}
		//
		inline LRESULT SetItem(LPLVITEM plvi){
			return SendMessage(LVM_SETITEM,0,(LPARAM)plvi);
		}
		//
		inline LRESULT GetItem(LPLVITEM plvi){
			return SendMessage(LVM_GETITEM,0,(LPARAM)plvi);
		}
		//
		//
		//
	};
	interface CONCISEUI_API StatusBarPtr : virtual public WndPtr
	{
	public:
		StatusBarPtr(){};
		StatusBarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//���� StatusBar ��Ϊ�������֣�����������Է������ã������һ�ε���Ϊ׼������ԭ���������ӷֶΣ�
		//����ԭ������ɾ�������ķֶΡ�width ��һ���������飬����Ϊ parts��ÿ������Ԫ�ص�ֵ��Ӧÿ����
		//�ε��ұ߽磬��λ�����أ����ĳ��Ԫ�ر�ǰ���ֵС����ζ������λᱻǰ��Ķ��ڵ������ɼ������
		//���ֵ��-1�����ʾ�����ֱ���ؼ���ĩβ������Ķζ������ڵ�����
		bool SetParts(int parts,int* width){return 0!=SendMessage(SB_SETPARTS,parts,width);}
		//���öε��ı���index �Ƕε�������type �����ֵ���ʾ��񣬱��� SBT_POPOUT ��
		bool SetText(int index,LPCWSTR text,int type = 0){return 0!=SendMessage(SB_SETTEXT,index|type,text);}
	};
	interface CONCISEUI_API UpDownPtr : virtual public WndPtr
	{
	public:
		UpDownPtr(){}
		UpDownPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}

		//���ù������ڣ�һ����һ���ı����ڣ�����ǰһ�������ؼ������
		HWND SetBuddy(HWND buddy){return (HWND)SendMessage(UDM_SETBUDDY,buddy,0);}
		//ȡ�ù������ڡ�
		HWND GetBuddy(){return (HWND)SendMessage(UDM_GETBUDDY,0,0);}
		//���÷�Χ��
		void SetRange(int nLower,int nUpper){SendMessage(UDM_SETRANGE32,nLower,nUpper);}
		//ȡ�÷�Χ��
		Twin GetRange(){Twin rg;SendMessage(UDM_GETRANGE32,&rg.x,&rg.y);return rg;}
		//����λ�á�
		int SetPosition(int nPos){return (int)SendMessage(UDM_SETPOS32,0,nPos);}
		//ȡ��λ�ã�ʵ���������ֵ�Ǵ�buddy����ȡ�õģ��������buddy���ڲ����ڻ����ı�����һ�����֣����ص�ֵ�ǲ����õġ�
		int GetPosition(){return (int)SendMessage(UDM_GETPOS32,0,0);}
	};
	interface CONCISEUI_API TrackBarPtr : virtual public WndPtr
	{
		TrackBarPtr(){}
		TrackBarPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		void SetRange(WORD min,WORD max){SendMessage(TBM_SETRANGE,1,MAKELPARAM(min,max));}
		void SetPos(LONG pos){SendMessage(TBM_SETPOS,1,pos);}
		int GetPos(){return (LONG)SendMessage(TBM_GETPOS,0,0);}
	};
	interface CONCISEUI_API HeaderPtr : virtual public WndPtr{
		HeaderPtr(){}
		HeaderPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
		//set header control size to appropriate parent window dimension,
		//return the control height.when width<0,width will equal parent client width.
		int AdjustSize(HWND parent,int x = 0,int y = 0,int width = -1);
		//insert a item to header control,return the new column item position index or -1 if failed.
		int AddItem(LPCWSTR text,int width,int index = -1);
		//delete item of index.
		bool DelItem(int index){return 0!=SendMessage(HDM_DELETEITEM,(WPARAM)index,0);}
		//set header control column text and width.
		bool SetItem(int index,LPCWSTR text,int width);
		//get column's width and title text,return width of column.
		int GetItem(int index,String& text);
		//get count of all column.
		int GetCount(){return (int)SendMessage(HDM_GETITEMCOUNT,0,0);}
		//set item width.
		bool SetItemWidth(int index,int width);
		//set item text
		bool SetItemText(int index,LPCWSTR title);
		//set item feature
		bool SetItem(int index,HDITEM* hdi);
		//get item info.
		bool GetItem(int index,HDITEM* hdi);
	};
	interface CONCISEUI_API LinkPtr : virtual public WndPtr{
		LinkPtr(){}
		LinkPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
	};
	interface CONCISEUI_API IpAddressPtr : virtual public WndPtr{
		IpAddressPtr(){}
		IpAddressPtr(HWND ctrl){_Handle = ctrl;}
		void operator = (HWND ctrl){_Handle = ctrl;}
	};
}