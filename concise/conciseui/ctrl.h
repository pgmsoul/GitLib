#pragma once
//�ؼ��ĺܶ���ʵ���Ϻ����õ�,���һ��кܶ����Ѿ�����,���õķ�񲢲��Ǻܶ�.�Դ��ںͿؼ��ķ��,������ȫ�˽�.
namespace cs{
	//���������ʼ��Ҫʹ�õĿؼ�,Ĭ�ϲ�����ʼ�����пؼ�.
	CONCISEUI_API bool InitControl60(DWORD icc = ICC_WIN95_CLASSES);
	//ImageList����ͼ���ͼ��,����Ҫ���������ؼ�ʹ�õ�ͼ��Դ.
	class CONCISEUI_API  ImageList : public HandleType<HIMAGELIST>{
	public:
		~ImageList();
		//��һ��HIMAGELIST���.
		void operator = (HIMAGELIST hImgl);
		//���������������ܱ�ʾû�гɹ�����ǰһ�����(��hImgl�ɹ���),��hImglΪNULL.
		bool Attach(HIMAGELIST hImgl);
		//����󶨵ľ��.
		HIMAGELIST Detach();

		//����һ��ImageList,cx,cy��ͼ��Ĵ�С,һ��ImageListֻ�ܴ洢��ͬ��С��ɫ���ͼ��.ilc��ͼ�������,initial��ʼ��С,grow����.
		bool Create(int cx,int cy,UINT ilc = ILC_COLORDDB,int initial = 16,int grow = 16);
		//��ͼ����Դ����.��������Ǵ���һ��ȫ�µ�ImageList,ԭ����HIMAGELISTʵ����ɾ��.cx�����ɵ�IMAGELISTͼ����,���������Ⱥͼ��ص�
		//ͼ������ȷ�����ɶ��ٸ�ͼ��,����һ��ͼ��Ĳ��ֻᱻ����.mask��͸��ɫ,res����ԴID,hInst = 0��ʾ��BMP�ļ�����(����ʾ�ӵ�ǰʵ������),
		//grow������.����ͼ�����.
		int LoadResource(int cx,COLORREF mask,ResID res,int grow = 16);
		//����HIMAGELIST.
		bool Destroy();

		//�滻��Ӧ������ͼ��,HICON��ԴҪ�Լ�����.
		bool Replace(HICON icon,int index);
		//���һ��ͼ��,HICON��ԴҪ�Լ�����.ͼ���ɫ���ImageList��ɫ��Ҫһ��,���򱳾���͸��.
		bool AddIcon(HICON icon);
		//��λͼ��ӵ�ImageList��,������ӵ�����,���������λͼ�Ŀ�ȿ��Էָ�ɵ�ImageList��ͼ�������,����һ���Ĳ�����ȥ.
		int AddBitmap(HBITMAP bmp,HBITMAP mask = 0);
		//ɾ����Ӧ������ͼ��.
		bool Delete(int index);
		//���ͼ��
		bool Clear();
		//��ʾͼ��.
		bool Draw(HDC hdc,int index,int x,int y,UINT style = ILD_NORMAL);
		//������Ӧ������ͼ��.
		bool Copy(int iDst,int iSrc);
		//������Ӧ������ͼ��.
		bool Swap(int iDst,int iSrc);
		//����һ������.
		HIMAGELIST Duplicate();
		//����ͼ������.
		int Count();
		//����ͼ���С.
		Twin GetImageSize();
		//������Ӧ������ͼ��.
		HICON GetIcon(int index);
		//���ر���ɫ.
		COLORREF GetBkColor();
		//���ñ���ɫ.
		COLORREF SetBkColor(COLORREF bk);
		//����ͼ���С,������������ͼ��.
		bool SetImageSize(int cx,int cy);
		//�����������ͼ�������,�����������ԭ��������,�����Լ���AddIcon�������λ.
		bool SetCount(int n);
	};
	//�ؼ��Ļ��࣬��View��ͬ���ؼ����Լ��Ĵ����࣬��ʵ��ֱ���ؼ�������ɣ����Ƕ����ղ�����Ϣ��
	//OnCreate�Ǹ�α��Ϣ�ص�������ֱ��Create���֮��ŵ��õģ���������ӦWM_CREATE��Ϣ��
	class CONCISEUI_API  ICtrl : public IWnd{
	protected:
		//��ȡ�ؼ���Ϣ�Ļص�����.
		void _OnICtrl(Message* msg,IWnd*);
		//����ؼ��ı�����Ĭ�Ϻ�����
		void _CtrlColor(Message*,ICtrl*);
		//�ؼ���ɫ��Ӧ���� ��������°�һ�����������ƿؼ�����ɫ��Ĭ�ϵ�_ControlColor���������ᱻ���á�
	public:
		Function<void,Message*,ICtrl*> OnNotify;
		Function<void,Message*,ICtrl*> OnColor;
		//���캯����
		ICtrl();
		~ICtrl();
		//���� Param �������ɿؼ�����������������Ϣ������
		INT_PTR Create(HWND parent);
		//��һ���Ѿ����ɵĴ��ڣ���������������Ϣ������
		bool Attach(HWND ctrl);
		//����󶨵Ĵ��ڣ��ͷ���Ϣ����Ϊԭ����ֵ��
		HWND Detach();
	};
	CONCISEUI_API ICtrl* GetICtrl(HWND hWnd);
	//����Ŀؼ���Ȼ��ICtrl��Ptr�̳У���û������ռ�ÿռ�ĳ�Ա�����ǲ��ܰ�һ�� new ICtrl �����ָ��
	//ת��Ϊ����ؼ���ָ�룬���ǵĴ�С�ǲ�ͬ�ģ�������iCtrlָ�벻��  new ICtrl �����ģ����Ǳ��� new ListBox
	//�����ת��Ϊ ListBoxָ�롣

	//class Label
	class CONCISEUI_API  Label : public ICtrl,public LabelPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Label();
	};
	//Button Control
	/*ֻ��XP���İ�ť�ſ�������ͼ���б�ؼ�,���ͼ���б�ؼ�ֻ��һ��ͼ��,����ʾ���ͼ��.����ж���1��ͼ����Ӧ����
	5��ͼ��,�ֱ�������,�н���,���ͣ��,DISABLE״̬�Ͱ���״̬����ʾ

	BS_GROUPBOX����Button��Ϊ����͸���������Ʊ������������ڲ���ʹ��WS_CLIPCHILDREN���

	*/
	class CONCISEUI_API  Button : public ICtrl,public ButtonPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Button();
	};
	class CONCISEUI_API  GroupBox : public ICtrl,public ButtonPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		GroupBox();
	};
	class CONCISEUI_API  CheckBox : public ICtrl,public ButtonPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		CheckBox();
	};
	class CONCISEUI_API  Radio : public ICtrl,public ButtonPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Radio();
	};
	//Edit Control
	class CONCISEUI_API  Edit : public ICtrl,public EditPtr{
		void _CtrlColor(Message*,ICtrl*);
		void _Command(WORD id,WORD type,IWnd*);
		void _OnNumberChar(Message* msg,IWnd*);
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Edit();
		Function<void,Edit*> OnChange;
		void SetNumerical(bool numerical);
	};
	//DateTimePicker Control
	class CONCISEUI_API  DateTimePicker : public ICtrl,public DateTimePickerPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		DateTimePicker();

		//Function<void,SystemTime*> OnChanged;
	protected:
	};
	//Calendar Control
	class CONCISEUI_API  MonthCalendar : public ICtrl,public MonthCalendarPtr
	{
	public:
		MonthCalendar();
		void operator = (HWND hWnd){Attach(hWnd);}
	};
	//ListBox Control ��3�ַ��ֱ��ǵ�ѡ(Ĭ��),������ѡ(LBS_EXTENDEDSEL,������SHIFT��ѡ��һ������),�����ѡ(LBS_MULTIPLESEL,���Ե����ѡȡ��ȡ������һ��)
	class CONCISEUI_API  ListBox : public ICtrl,public ListBoxPtr
	{
	public:
		ListBox();
		void operator = (HWND hWnd){Attach(hWnd);}
	protected:
	};
	//ComboBox Control �������һ��ListBox�ؼ�,�����������ؼ��ĺܶຯ���Ǻ����Ƶ�.
	class CONCISEUI_API  ComboBox : public ICtrl,public ComboBoxPtr
	{
	public:
		ComboBox();
		void operator = (HWND hWnd){Attach(hWnd);}
	};
	//CoboBoxEx Control ������ʾͼ��.ComboBoxEx��һ������ķ����CBES_EX_CASESENSITIVE,���Զ��г���Ա.
	class CONCISEUI_API  ComboBoxEx : public ICtrl,public ComboBoxExPtr//ComboBoxEx��֧�������ִ��йصĺ���.
	{
	public:
		ComboBoxEx();
		void operator = (HWND hWnd){Attach(hWnd);}
	};
	//TreeView Control
	/*TreeView�ؼ�������ν��������ʾ�ؼ�,����Ԫ�������ηֲ�,����һ��Ӧ�÷ǳ��㷺�Ľṹ,�����ļ�ϵͳ.TreeView�ؼ����԰�
	����ͼ���б�ؼ�,һ��������ͨ״̬��,һ������ѡ��״̬��.*/
	class CONCISEUI_API  TreeView : public ICtrl,public TreeViewPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		/*Function<void,LPNMTREEVIEW> OnBeginDrag;	//��ʼ��ҷ
		Function<void,LPNMTREEVIEW> OnBeginRDrag;	//��ʼ�Ҽ���ҷ
		Function<bool,LPNMTVDISPINFO> OnBeginEdit;	//��ʼ�༭��ǩ,����1,��ȡ���༭��ǩ.
		Function<bool,LPNMTVDISPINFO> OnEndEdit;	//��ɱ༭��ǩ,����1,��ȡ���༭�ı�.
		Function<void,LPNMTREEVIEW> OnDeleteItem;	//ɾ��Ԫ��.
		Function<void,LPNMTREEVIEW> OnExpanded;		//չ�����۵���һ������.
		Function<bool,LPNMTREEVIEW> OnExpanding;	//��Ҫչ�����۵�һ������,����1ȡ������.
		Function<void,LPNMTREEVIEW> OnSelChanged;	//��ǰѡ����仯.
		Function<bool,LPNMTREEVIEW> OnSelChanging;	//��Ҫ�ı䵱ǰѡ����,����1��ֹ����.
		Function<UINT,LPNMTVKEYDOWN> OnChar;		//�а�������,����ִ����������,���MSDN˵��.
		*/
		TreeView();
	};
	//ToolBar Control
	//ToolBar�ؼ���Ҫ SendMessage(TB_AUTOSIZE,0,0)����ˢ�¿ؼ��������Ϣ��
	//������ť������ߴ磬�������ÿ�Ⱥ͸����ڿͻ�����ͬ��
	class CONCISEUI_API  ToolBar : public ICtrl,public ToolBarPtr{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		ToolBar();
		//���ɿؼ�.
		INT_PTR Create(HWND hParent);
	private:
		//void _OnParentSize(Message*);
	};
	//ProgressBar Control
	class CONCISEUI_API  ProgressBar : public ICtrl,public ProgressBarPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		ProgressBar();
	};
	class CONCISEUI_API Rebar : public ICtrl,public RebarPtr{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Rebar();
	};
	//ToolTip Control ����ؼ�����������6.0�汾�ؼ�ʱ�ſ���.Ҳ����˵,����Ҫ��MANIFEST.
	class CONCISEUI_API  ToolTip : public ICtrl,public ToolTipPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		ToolTip(void);
		INT_PTR Create(HWND hParent);
	};

	//TabPage�ؼ��������ṩ�Կؼ��Ĳ���,��ֻ�ṩһ�����,Ȼ����ҳ��ť��������Ӧ����Ϣ�¼�.����Щ�¼��д���ؼ�����Ϊ,ʹ�����ֵ������ڴ��ڲ�ͬ��ҳ��.
	class CONCISEUI_API  TabPage : public ICtrl,public TabPagePtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		TabPage();
	};
	//��ʾ��ϼ���һ���ṹ.
	//�ȼ��ؼ�
	class CONCISEUI_API  HotKey : public ICtrl,public HotKeyPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		HotKey();
	};
	class CONCISEUI_API  ListView : public ICtrl,public ListViewPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		ListView();
		//ListView ����չ����������֮�����������������ã������ɴ���ʱ�ķ�����չ��������ö��������á�add��ʾ��ӻ����Ƴ�exStyleָ���ķ��
		DWORD SetExStyle(DWORD exStyle,bool add = 1);
	};
	class CONCISEUI_API  StatusBar : public ICtrl,public StatusBarPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		StatusBar();
	};
	class CONCISEUI_API  TrackBar : public ICtrl,public TrackBarPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		TrackBar();
	};
	//UpDown �ؼ����򸸴��ڷ��͹�������Ϣ
	class CONCISEUI_API  UpDown : public ICtrl,public UpDownPtr
	{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		UpDown();
		//ֱ������һ�� UDS_SETBUDDYINT UDS_ALIGNRIGHT UDS_ARROWKEYS ���� UpDown �ؼ���nUpeer�����ޣ�nLower�����ޣ�nPos����ǰλ�á�
		INT_PTR Create(HWND buddy,HWND parent,int nUpper,int nLower,int nPos);
		INT_PTR Create(HWND parent);
	};
	class CONCISEUI_API Header : public ICtrl,public HeaderPtr{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Header();
	};
	class CONCISEUI_API Link : public ICtrl,public LinkPtr{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		Link();
	};
	class CONCISEUI_API IpAddress : public ICtrl,public IpAddressPtr{
	public:
		void operator = (HWND hWnd){Attach(hWnd);}
		IpAddress();
	};
}