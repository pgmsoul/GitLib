#pragma once

#define WM_CTRLUSER			0x3790					//�ؼ�����Ϣת����ϢID
//#define WM_CTRLCOLOR		(WM_CTRLUSER+0x0132)	//�ؼ�����ɫ��Ϣ.
#define WM_GET_IWND			(WM_CTRLUSER+0x01)		//��HWND��ȡWindow*ָ��.
#define WM_GET_ICTRL		(WM_CTRLUSER+0x02)		//��HWND��ȡWindow*ָ��.
//#define WM_CTRLMSG			(WM_CTRLUSER+0x03)
//#define WM_USERNOTIFY		(WM_CTRLUSER+0x4)
//#define WM_USERCOLOR		(WM_CTRLUSER+0x5)
namespace cs{

	//���ں����ķ���ֵ����.
	class CONCISEUI_API Result : public HandleType<INT_PTR>{
	public:
		enum{RESULT_DEFAULT = 0x1,WNDPROC_DISABLE = 0x2};
		INT_PTR		_Flag;	//����λ������ĳЩ�ڲ�Ӧ�á�
		Result():_Flag(0){};
		inline void operator = (INT_PTR result){_Flag |= RESULT_DEFAULT;_Handle = result;}
		//ȱʡ����ֵ����ֵΪtrue��Wndproc��������Ϣ�������ô��ڵ�ȱʡԭ�ڲ����������ҷ������ķ���ֵ��
		//��ֵΪfalse��Wndproc��������Ϣ�������û����� = �������õ�ֵ�������ô��ڵ��ڲ�������
		inline bool Default(){return (_Flag&RESULT_DEFAULT)==0;}
		//��������Ϊȱʡ״̬��
		inline void SetDefault(){_Flag &= ~RESULT_DEFAULT;}
		//�û��Լ�������Ϣ��������Wndproc�Ĵ�����롣Wndproc�ڲ�ֻ�����˺��ٵ�������Ϣ��
		//��������û�һ�㲻��Ҫ���á�
		inline bool _DisableWndproc(){return (_Flag&WNDPROC_DISABLE)!=0;}
		//�����Ƿ����Wndproc����Ϣ�Ĵ�����롣
		inline void _SetDisableWndproc(bool disable){if(disable) _Flag|=WNDPROC_DISABLE;else _Flag&=~WNDPROC_DISABLE;}
	};
	//Windows ��Ϣ�ṹ
	typedef struct CONCISEUI_API Message : public _struct{
		HWND		hWnd;		//��Ϣ�Ĵ��ھ��.
		UINT		uMsg;		//��ϢID
		WPARAM		wParam;		//����
		LPARAM		lParam;		//����
		Result		Result;		//����ֵ.
	}Message;
	//���ṹ,���������Ϣ.
	typedef struct CONCISEUI_API Mouse : public _struct{
		Mouse(){}
		//Mouse(Mouse& m){*this = m;}
		//void operator = (Mouse& m){Event = m.Event;Button = m.Button;WheelDelta = m.WheelDelta;X = m.X;Y = m.Y;Msg=m.Msg;}
		UINT Event;			//��Ӧ�������Ϣ.
		short Button,		//��������,������0,���統Event��WM_MOUSEMOVEʱ.MK_����
			WheelDelta;		//������ֹ���ʱ(Event==WM_MOUSEWHEEL),���ֵ��120��-120,������ķ����й�.
		short X,Y;			//ָ���λ��.
		Message* Msg;
	}Mouse;
	//�ǿͻ�ȥ���ṹ
	typedef struct CONCISEUI_API NcMouse : public _struct{
		UINT Event;		//��ϢID
		UINT HitTest;	//HT������֪ʶ����״̬��
		short X,Y;		//��Ļ����
		Message* Msg;	//����ֵ��
	}NcMouse;
	//��������ʱ�õ��Ľṹ,���ڳ�ʼ�����ڵı�Ҫ����.
	typedef struct CONCISEUI_API CreateStruct : public _struct{
		CreateStruct():ClassStyle(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),Style(WS_VISIBLE),ExStyle(0),Menu(0),lParam(0),Location(200,200),Size(500,300){}
		String ClassName,
			Text;					//���ڵı���,ֻ�Զ���������Ч.
		Twin16 Location,Size;	//����λ��,��С.
		DWORD ClassStyle,Style,ExStyle;	//���ڵ�������
		union{
			HMENU		Menu;				//�˵�(����������) 
			WORD		Identity;			//ID(�����Ӵ���).
		};				
		ResID		Dialog;				//���ڶԻ���
		LPVOID lParam;					//�������.
		void SetRect(int left,int top,int width,int height){
			Location.SetValue(left,top);
			Size.SetValue(width,height);
		}
		void AddStyle(DWORD style){
			Style |= style;
		}
		void RemoveStyle(DWORD style){
			Style &= ~style;
		}
		void AddExStyle(DWORD style){
			ExStyle |= style;
		}
		void RemoveExStyle(DWORD style){
			ExStyle &= ~style;
		}
	}CreateStruct;
	//���ڶ���Ļ��࣬��������ϵͳ���Զ��崰���࣬�ؼ����Ի���ȫ����������࣬�����̳������.
	//Ptr��׺���඼�ǰ�һ��ָ����󣬴�С�������ָ��Ĵ�С��������ֻ���ɺ��������������ݡ�
	class CONCISEUI_API WndPtr : public HandleType<HWND>{
	public:
		WndPtr(){}
		WndPtr(HWND hwnd){_Handle = hwnd;}
		//��һ�����ڡ�
		void operator = (HWND hwnd){_Handle = hwnd;}
		//���ش��ڵ�����,���ڶ������ڶ�Ӧ��Ļ����,�����Ӵ���,��Ӧ����������.
		bool GetRect(Rect& r);		
		//���ش��ڵĿͻ����ߴ�.�����������Ͻ�����һ����(0,0),��Ҳ���ܲ���,����б߿�Ļ�.�����ľ�������û��һ��API���������ṩ.����ʹ��Window��ĳ�Ա����ȡ.
		bool GetClientSize(Twin16& c);						
		//���ô�С�����ı�λ��.
		inline bool SetSize(int cx,int cy) {return ::SetWindowPos(_Handle,0,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER)!=0;}
		//����λ�ã����ı��С
		inline bool SetLocation(int x,int y){return ::SetWindowPos(_Handle,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER)!=0;}
		//���ô�С��λ�á�
		inline bool SetDimension(int x,int y,int cx,int cy) {return ::MoveWindow(_Handle,x,y,cx,cy,1)!=0;}
		//�ƶ����ڻ�������ڴ�С,dx,dy�Ǵ���λ�õ�ƫ����,cx,cy�Ǵ�С�ı仯��.
		void Offset(int dx, int dy, int cx, int cy);	

		//ȡ���ı��ִ��ĳ��ȣ���������β 0 �ַ���
		inline int TextLength(){return ::GetWindowTextLength(_Handle);}
		//ȡ�ô����ı���text���س�ʼ����
		int GetText(String& text);
		//���ô����ı���
		inline bool SetText(LPCWSTR text){return ::SetWindowTextW(_Handle,text)!=0;}
		//���������ü�ʱ��,�ɹ��󴰿ڻᶨʱ����WM_TIMER��Ϣ,ms�Ǽ��ʱ��(����),id�Ǽ�ʱ���ı�ʶ,������id��֮ǰ���õ�һ��timer�����滻ԭ���ġ������޴��ڰ󶨵ļ�ʱ�����Ҳ����滻���е�timer��Ӧ�ø�id��0ֵ����ϵͳ������һ��id�������������id��
		inline UINT_PTR SetTimer(UINT ms,UINT_PTR id){return ::SetTimer(_Handle,id,ms,NULL);}
		//ɾ��һ��timer��id������ʱ���ص�id��
		inline bool KillTimer(UINT_PTR id){return ::KillTimer(_Handle,id)!=0;}
		//Ҫ�󴰿��ػ�ָ��������,erase�Ƿ��ػ汳��,Ҳ�����Ƿ�����WM_ERASEBKGND��Ϣ.
		inline bool Invalidate(const RECT* clip = 0,bool erase = 1){if(_Handle==0) return 0;return ::InvalidateRect(_Handle,clip,erase)!=0;}
		//Ҫ�󴰿��ػ�ָ��������,erase�Ƿ��ػ汳��,Ҳ�����Ƿ�����WM_ERASEBKGND��Ϣ.
		inline bool Invalidate(const HRGN hRgn,bool erase = 1){if(_Handle==0) return 0;return ::InvalidateRgn(_Handle,hRgn,erase)!=0;}
		//���ش��ڲ�����
		inline LONG_PTR GetWindowParam(int gwl){return ::GetWindowLongPtr(_Handle,gwl);}
		//���ô�����ز���.
		inline LONG_PTR SetWindowParam(int gwl,LONG_PTR value){return ::SetWindowLongPtr(_Handle,gwl,(LONG)value);}
		//���ڵĸ����ھ��.
		inline HWND Parent(){return ::GetParent(_Handle);}
		//��Ϣ����,�и����ɵĲ�������,�����������ʹ�õĲ��������Ƿǳ����.
		template<typename T,typename R> inline LRESULT SendMessage(UINT ttm,T wParam,R lParam){return ::SendMessage(_Handle,ttm,(WPARAM)wParam,(LPARAM)lParam);}
		//Enable ���� Disable һ�����ڡ�
		inline bool Enable(bool enable = 1){return ::EnableWindow(_Handle,enable)!=0;}//return value is pre state of disabled.
		//�򴰿ڷ���WM_CLOSE��Ϣ������ر�û�б�ȡ������true����֮����false��
		bool Close(){return 0!=SendMessage(WM_CLOSE,0,0);}
		//���Ĵ��ڵķ��
		void ModifyStyle(DWORD dwRemove,DWORD dwAdd);
		//���Ĵ��ڵ���չ���
		void ModifyExStyle(DWORD dwRemove,DWORD dwAdd);
		//���ô��ڵĿ�ȡ�
		bool SetWidth(int cx);
		//���ô��ڵĸ߶ȡ�
		bool SetHeight(int cy);
		//���ô��ڵ�ˮƽλ�á�
		bool SetXLocation(int x);
		//���ô��ڵĴ�ֱλ�á�
		bool SetYLocation(int y);
	};
	//UIһ��ֻ��һ���̣߳�ͬһ�����ڵ����л�ͼ������ͬһ���߳���ɵģ��ܶ�ʱ�򣬲�����Ҫ
	//ÿ��������һ�������� Font ��������ʹ��һ���̱߳������������壬�ı�������壬����
	//ʹ�����Ĵ��ڶ�����ͳһ������
	CONCISEUI_API Font* GetDefaultFont();
	//�ػ񴰿���Ϣ�ľ�̬��������.
	typedef void (__stdcall*MSGPROC)(Message*); 
	//Class IWnd ���ṩ�����Ĵ��ڽṹ����Ҫʵ��������ࡣ
	interface CONCISEUI_API IWnd : virtual public WndPtr{//public HandleType<HWND>//
	public:
		//��Ϊ��һ���ⲿ����Ҫ�����������,��һ���������������������ⲿ����,������"_"����ʶ.
		LRESULT _WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		Function<LRESULT,HWND,UINT,WPARAM,LPARAM> _WndPointer;	//���ں���.
	private:
		typedef struct _UPOINT : _struct{
			UINT x,y;
			bool operator == (_UPOINT up){
				return (x==up.x)&&(y==up.y);
			}
		}_UPOINT;
		Diction<_UPOINT> _MsgList;
		ObjectList<Delegate<Message*,IWnd*> > _ProcList;
		bool _mLeave,_mNcLeave;					//����Ƿ��뿪.

	protected:
		Twin		_DragMinSize;		//������קʱ����С�ߴ硣
		Twin		_DragMaxSize;		//������קʱ����С�ߴ硣
		Twin		_MinSize;			//û�й���������С��С.
		Twin		_Scr;				//ƫ����
		Twin16		_ClientLocation;	//Client Location;
		LONG_PTR	_QuondamProc;		//����ԭ���Ĵ��ں���.����һ���û��Լ�����Ĵ���,��������޹ؽ�Ҫ,һ����DefWndProc������,����һ��Windows�ڲ��Ĵ���,����Text�ؼ�,�������������ȷ����.
		HCURSOR		_Cursor;			//���ָ����״

	public:
		IWnd(void);
		~IWnd();
		Color					FontColor;	//�ؼ���ǰ��ɫ(������ɫ).
		Color					BkColor;	//�ؼ��ı���ɫ.��Ϊ-1��ʾ����͸��(����������ػ�����ʱ,�������γ��ص���ʾ�ַ�).
		Object<CreateStruct>	Param;		//�����Ա�������ô�������ʱ�Ĳ���,�����Լ���Ĭ�ϲ���,ֻ��Ҫ������ı�Ĳ���.
		Delegate<WORD,WORD,IWnd*>	OnCommand;	//��Ϊ����¼��ᱻƵ����ʹ��,���Լ�����.
		Delegate<IWnd*>				OnCreate;	//WM_CREATE��Ϣ�����������ɿؼ���ʱ������Ϣ��ʵ��������ɺ�ģ�������һ�Ρ�
		Delegate<Mouse*,IWnd*>		OnMouse;	//MouseEvent�������������Ϣ.		
		Delegate<NcMouse*,IWnd*>	OnNcMouse;	//OnNcMouse����ǿͻ����ģ���WM_NCMOUSELEAVE�����������Ϣ.ʵ����������Ϣ����WM_NCHITTEST����DefWindowProc���������ġ�
		Delegate<bool&,IWnd*>		OnClose;	//�ػ�WM_CLOSE��Ϣ��boolֵĬ��Ϊfalse�������Ϊtrue���򴰿ڹرն���ȡ�������ڲ������ڹرյ�ʱ����������WM_CLOSE��Ϣ, ���WM_DESTROY�ǲ�ͬ��, WM_CLOSE��Ϣ���ⲿ����������, ���ڽ��յ������Ϣ��Ĭ����Ϊ������, �����Ӵ���Ҳ�����.
		Delegate<Twin16,IWnd*>		OnSize;		//WM_SIZE��Ϣ�������ǿͻ����Ĵ�С��
		//�û��Զ������ݣ������Ա�������ǿ����ڲ����¼̳����������¸���������ӳ�Ա��
		IntegerList<void*>	UserDataList;	//�����û��Զ�������ָ��.
		//����ƫ��
		inline Twin Scr(){return _Scr;}
		//���ֹ���������С�ߴ�
		inline Twin MinSize(){return _MinSize;}
		//�ͻ����ڴ��������е�λ��.;
		inline Twin16 GetClientPos() const {return _ClientLocation;}
		//���ض�Ӧ��Ϣ�Ĵ���ָ��,���Դ����ָ����ӻ�ɾ����Ӧ�ĺ���.
		Delegate<Message*,IWnd*>* GetMsgHook(UINT msg);
		//���ض�Ӧ��Ϣ�Ĵ���ָ��,���Դ����ָ����ӻ�ɾ����Ӧ�ĺ���.
		Delegate<Message*,IWnd*>* GetMsgHook(UINT msg1,UINT msg2);
		//���ÿؼ�ʹ�õ�����,font=0��ʾʹ��һ��concise�����õ������������concise��ֻ��һ��ȱʡ����������Զ��̵߳Ľ��棨�����ʹ�ã���������»�ͬʱʹ���������.
		bool SetFont(HFONT font = 0,bool reDraw = 0);
		HFONT GetFont(){return (HFONT)SendMessage(WM_GETFONT,0,0);}
		//����һ�����ڣ��������������������Param��ClassName��������������������ڵ����ͣ�������Զ��崰�ڣ������Լ�ע������ࡣ��������ʧ�ܡ�
		virtual INT_PTR Create(HWND parent){return 0;};
		//�����Ϸ�ʱ����С�ߴ�
		void SetMinDragSize(int cx,int cy){_DragMinSize.SetValue(cx,cy);}
		//�����Ϸ�ʱ�����ߴ�
		void SetMaxDragSize(int cx,int cy){_DragMaxSize.SetValue(cx,cy);}
	};
	CONCISEUI_API IWnd* GetIWnd(HWND hWnd);
	//Class View
	class CONCISEUI_API View : public IWnd{//public WndPtr//,
	protected:
		void _DropFilesProc(Message* msg,IWnd*);	
		void _DrawProc(Message* msg,IWnd*);
		void _DoubleDraw(DC* dc,IWnd*);
		void _EraseBkgndProc(Message* msg,IWnd*);
		//void _CtrlSizeProc(Message* msg);
	public:
		View();
		~View();
		//��ǰ����û������֮ǰ,�������ɵ�2������.
		INT_PTR Create(HWND hParent);

		bool Attach(HWND handle);
		HWND Detach();
		//���ô���ָ�롣
		void SetCursor(HCURSOR cursor);
		//ȡ�õ�ǰ���ָ�롣
		HCURSOR GetCursor(){return _Cursor;}
		//���ô���ָ�룬����Ϊ��Դid����Դ���ڵ�ʵ����0��ʾϵͳ��Դ��
		void SetCursor(ResID resurce);

		Delegate<DC*,View*>	OnDoubleDraw;//һ������������м��뺯����OnDraw�Ͳ��ᱻ���С�����������Զ��������ڣ�Ҳ����˵��ͼ�����Լ����Ǵ��ڵĹ������⡣
		Delegate<DC*,View*>	OnDraw;//��ͼ��Ϣ�ڳ�����ʵ�������첽��Ӧ�ó����̵߳�.		
		Delegate<HDC,View*>	OnEraseBkgnd;	//Ҳ��������������ͼ,����OnDraw֮ǰ������.�����ǵĻ����ǲ�ͬ��
		Delegate<StringMemList*,View*>	OnDropFiles;//��Ӧ��ҷ��Ϣ.
	};
	//Class ScrollView
	class CONCISEUI_API ScrollView : public View{
	public:
		ScrollView(void);
		~ScrollView();
		//������С�Ŀͻ����ߴ�,���ͻ���С������ߴ�,����������.
		void SetMinSize(Twin size);	
		//������С�Ŀͻ����ߴ�,���ͻ���С������ߴ�,����������.���cx��cyΪ��ֵ������Ӧ�Ĵ�С���䡣
		void SetMinSize(int cx,int cy);				
		//������С�Ĺ�����λ.
		void SetMinScroll(int hsline, int vsline);		
		//0-hScroll,2-NoScroll,���������ֹ������ڵķ���.
		void SetMouseWheelScroll(int vScroll = 1);		
		//�����Ƿ���Ӧ���̷��������������.
		void SetResponseKeyBoard(bool response = 1);	
		//�������ڣ�dx��ˮƽ�������룬dy����ֱ�������롣
		void Scroll(int dx,int dy);
		//����ָ����������xLine ��ˮƽ������yLine �Ǵ�ֱ���������������������
		void ScrollLine(int xLine,int yLine);
		//����ָ����ҳ����xPage ��ˮƽҳ����yPage �Ǵ�ֱҳ�������������������
		void ScrollPage(int xPage,int yPage);
		//���������㣬xEnd = 1 ���������ұߣ�xEnd = -1 ����������ߣ�yEnd = 1 ���������±ߣ�yEnd = -1 ���������ϱߡ�
		void ScrollEnd(int xEnd,int yEnd);
		//�ػ洰��.����ͻ����й���,����������Լ�����,���Բ��ض��������ƽ��.
		bool Invalidate(const RECT* clip = NULL,bool erase = 1);
		//�ػ洰��.����ͻ����й���,����������Լ�����,���Բ��ض��������ƽ��.
		bool Invalidate(const HRGN clip,bool erase = 1);
		//����һ����,���ݹ���ƫ����.
		void TransDimension(Twin& pt);			
		//����һ������,���ݹ���ƫ����.
		void TransDimension(Rect& rect);			

		Delegate<int,int,ScrollView*> OnScroll;//���ڹ���ʱ����.����Ϊƫ����,ʵ����,������������һ����0.

	private:
		void _MouseWheelProc(Message* msg,IWnd*);
		void _VScrollProc(Message* msg,IWnd*);
		void _HScrollProc(Message* msg,IWnd*);
		void _KeyDownProc(Message* msg,IWnd*);
		void _SizeProc(Message* msg,IWnd*);

		int _vsPage;
		int _hsPage;
		int _vsLine;
		int _hsLine;
		int _vsMax;
		int _hsMax;
		DWORD _MouseWheelScroll;//���͵�WM_VSCROLL��WM_HSCROLL��Ϣ.
		bool _ResponseKeyBoard;//ָʾ��Ӧ���̣��д�ֱ������ˮƽ��
	};
	//Class Frame
	//Frame�������رհ�ť���Զ��˳�Ӧ�ó��򣬵���������������߳������Ч�������ϣ���
	//�ڲ������PostQuitMessage������ֻ���˳������ڵ��̡߳�
	class CONCISEUI_API Frame : public ScrollView{
	public:
		Frame();
		~Frame();
		//����һ��û��WS_CHILD���������ڣ���Ȼ��ʵ����Ҳ������һ�����ڵ��Ӵ��ڣ�������������������������
		//�����١�quitָ���Ƿ��˳�ʱ���������
		INT_PTR Create(HWND hParent = 0,int nCmdShow = 1,bool quit = 1);
		//�˳���Ϣѭ�������ڹرգ���ǰ�߳̽���, �����Ψһ�߳�, Ӧ�ó���Ҳ���˳���
		void Quit(int code = 0);
		//big=0,for set small icon,big=1,for set big icon.
		inline HICON SetIcon(HICON icon,bool big=0){return (HICON)SendMessage(WM_SETICON,big,icon);}
		//type=0,for small icon,type=1,for large icon,type=2,for system assign icon for application.
		inline HICON GetIcon(DWORD type=ICON_SMALL){return (HICON)SendMessage(WM_GETICON,type,0);};
	private:
		void _DestroyProc(Message* msg,IWnd*);
	};
	//���������ڵ�ͼ�꣬����ͼ���Сͼ�ꡣ
	CONCISEUI_API void SetWndIcon(HWND h,ResID id);
	/* WndDragger �����϶�һ�� IWnd ��λ��
	*/
	class WndDragger : public _class{
	protected:
		int _x,_y;
		bool _drag;
		IWnd* _wnd;
		void onMouse(Mouse* m,IWnd*);
	public:
		//���� OnDrag �ķ���ֵ
		enum{
			DRAG_NONE = 0,	//����Ӧ�϶�
			DRAG_X = 1,		//ֻ��Ӧ X ������϶�
			DRAG_Y = 2		//ֻ��Ӧ Y ������϶�
		};
		Function<int,int,int,WndDragger*> OnDrag;	//�϶��ص�����������ֵָ���϶���ʽ�����������ֱ��� x �� y ������ƶ����롣
		WndDragger():_drag(0),_wnd(0){}
		//����Ҫ�϶��Ĵ��ڣ�һ�� Dragger ֻ������һ�����ڵ��϶����������´��ں�ԭ���ھͲ�����Ӧ��� dragger������󶨵Ĵ��ڣ�����ֱ�Ӵ� 0 ��
		void SetWnd(IWnd* wnd);
		IWnd* GetWnd(){return _wnd;}
	};
}
/*		HandleType<HWND>
			|			
			|          
		WndPtr		
	   /   |   \
	  /v   |v   \v
	IWnd IButton \
   /  |    |      \
  /   |    |     IDlg
View ICtrl | 
        \  |
         \ |
		  Button

����ϵͳ�ڲ�����

����ϵͳʵ�����ڲ��ṹ�ǳ��򵥣�һ�����ڵĴ����������£�

  1. ���� Create ����������������ĵĴ����� View �������棬�κη�ϵͳ���
���ڣ����� Edit��Button �ȴ�������ϵͳʵ�ֵģ������ն������ View �����
Create ������

  2. Create �������Ȳ鿴 Parma->ClassName ����������˲���Ϊ�գ�Create ��
���ȵ��� RegisterClassEx ע��һ�������࣬�����һ��ȱʡ�Ĵ����࣬�����
�� Param->ClassStyle ���������ô�����ķ���������������һ��Ϊ�գ���
��˵���ͼ�ꡢ����ɫ�ȣ���Ϊ��Щ������Ӧ���Ǵ��ڵģ������Ǵ�����ģ�����
΢�������Ƶ��˴����൱�С�
    ��� Param->ClassName ������Ϊ�գ���ʾ�û�ʹ����һ���ֳɵĻ����Լ�ע��
�Ĵ����࣬��ô Create ��������� RegisterClassEx ��������������������� 
ClassStyle ���������ģ���Ϊֻ����������ǿɱ�ģ�Ҳ����˵�����ڲ��ı���
�Ĵ����࣬�����ᴴ����ͬ�Ĵ����࣬��Ϊ RegisterClassEx �᷵���Ѵ��ڵĴ����ࡣ

  3. ���д������ lpfnWndProc ����ָ��ͬһ����̬���ڻص����� WindowProcedure��
Ҳ�����κ��Զ��崰�ڵ�һ���ص����ǵ������������
    ��Ϊ CreateWindowEx ����������ͬһ���߳�����ô��ڻص����������� View ��
Create ����ʹ��������һ����ơ�����һ��ȫ�ֵ��̱߳�����Ԥ�������Լ���ָ�룬
�� WindowProcedure ����һ�Σ����ص���ʱ��ʵ������ֻ�ᱻ����һ�Σ����Ѵ���
�Ļص�����ָ�� View �Լ��Ļص���������� View �Ļص������� _WndPointer ��Ա��
_WndPointer ������һ������������һ�� Function ����������������Լ��ļ̳���
�������� _WndPointer ָ���Լ��Ļص���������Ȼ��һ��������ô����Ĭ�� _WndPointer
ָ�� View �ĸ��� IWnd �ĳ�Ա���� _WndProc��_WndProc �Գ�����Ϣ�����˰�װ��
IWnd�Լ��κ������������Ϣ�¼���On��ͷ���¼��������� _WndProc �����ġ�
*/