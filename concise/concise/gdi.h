#pragma once
#include "res.h"
namespace cs{
	//GDI������ѡ��һ��DCʱ,�ǲ��ܱ��ͷź����ٵ�,��������ɹ�ִ����InDCû��ִ��OutDC����,��Դ�Ͳ�����ȷ���ͷ�.
	//���һ��GdiObjectʵ���Ѿ�����һ��ϵͳGDI����,��ʱִ�����ɺͰ�һ���µ�GDI����ʱ,ʵ�����Զ�ȥ�ͷŻ�����
	//�Ѿ��󶨵Ķ���,���ǵ���������Ѿ�ѡ��һ��DC�������,�ǲ����ͷŵ�,���������,������DEBUGģʽ�»ᷢ��һ������,
	//���Ǻ�������ʧ��.
	template<typename T> class CONCISETL_API GdiObject : public HandleType<T>
	{
	public:
		GdiObject()
		{
		}
		//�Զ�ɾ���󶨵�GDI��Դ,���ִ����InDC��û��ִ��OutDC������������ܻ�ʧ��.
		~GdiObject()
		{
			if(_Handle) ::DeleteObject(_Handle);
		}
		template<typename R>bool GetObject(R* pr)
		{
			return ::GetObject(_Handle,sizeof(R),pr)!=0;
		}
		//�Ѱ󶨵�GDI��Դѡ��DC.ִ�����������,���������,����Ӧ�óɶԵ�ִ���������.
		//��һ��GDI����ѡ��DCʱ����ʱʵ���ľ���󶨵Ĳ���ԭGDI����hdc��һ����0��ЧDCʱ�ᵼ����Դй©��
		inline void Select(HDC hdc)
		{
			if(hdc) _Handle = (T)::SelectObject(hdc,_Handle);
		}
		//������ʵ����Ϊ��,��󶨵�ǰһ��GDI��Դ�ᱻɾ��,����ʹû����ȷɾ����һ����Դ,���統�����Դ���ڱ�ѡ��
		//һ��DC��ʱ��,������Ȼ����ɰ��µĶ���.ֻ�е�Ҫ�󶨵Ķ�����һ��NULLָ��ʱ,�����ŷ���0.
		bool Attach(T t){
#ifdef _DEBUG
			if(!t) PrintD(L"%s",L"GdiObject.Attatch:�󶨵Ķ���Ϊһ����(NULL)����");
			//_ASSERT_EXPR(0,L"GdiObject.Attatch:�󶨵Ķ���Ϊһ����(NULL)����");
			//SystemMessageBox();
#endif
			if(_Handle){
				BOOL r = ::DeleteObject(_Handle);
				if(!r) PrintD(L"%s",L"GdiObject.Attatch:ԭ����û����ȷ�ͷ�");
			}
			_Handle = t;
			return 1;
		}
		//�����ǰ��Դ��ѡ��һ��DC,���������ʧ��.
		T Detach(){
			T h = _Handle;
			_Handle = 0;
			return h;
		}
		//ɾ���󶨵�GDI��Դ,ʵ������Դ�����Ϊ0,�����ǰ��Դ��ѡ��һ��DC,����ʧ��.
		bool Delete()
		{
			if(!::DeleteObject(_Handle))	return 0;
			_Handle = 0;
			return 1;
		}
		void FromStock(int stock)
		{
			Attach((T)::GetStockObject(stock));
		}
	};
	//��ɫ�ṹ.
	typedef union CONCISE_API Color{
	public:
		Color();
		Color(int value);
		Color(BYTE r,BYTE g,BYTE b,BYTE a = 0);
		void SetValue(BYTE r,BYTE g,BYTE b,BYTE a = 0);
		void operator = (COLORREF color);
		operator COLORREF ();
		struct{BYTE red,green,blue,alpha;};
		void* operator new(size_t size){
			return MemoryAlloc((int)size);
		}
		void operator delete(void* p){
			MemoryFree(p);
		}
	private:
		struct{UINT Value;};
	}Color;
	//����.
	class CONCISE_API Rgn : public GdiObject<HRGN>{
	public:
		enum{
			And = 1,	//�൱�ڽ���.
			Or = 2,		//�൱�ڲ���.
			Xor = 3,	//����֮��Ĳ���(�Եȼ�).
			Diff = 4,	//��ȥrgn(�����Ĳ���).
			Copy = 5	//Copy rgn;
		};
		//����һ���������,���δ�СΪ0.
		bool Create();
		//����һ����������.
		bool CreateRect(int x1,int y1,int x2,int y2);
		//����һ����Բ����.
		bool CreateEllipse(int left,int top,int right,int bottom){return Attach(CreateEllipticRgn(left,top,right,bottom));}
		//����һ����Բ����.
		bool CreateEllipse(const RECT* r){return Attach(CreateEllipticRgnIndirect(r));}
		//����һ�����������,ps,pn�ֱ��ǵ�������ռ�ĸ���,modeѡ1ʱ,����ᱻ¦��,��2ʱ�����.
		bool CreatePolygon(LPPOINT ps,int pn,int mode = 1/*or 2*/){return Attach(CreatePolygonRgn(ps,pn,mode));}// ALTERNATE or WINDING
		//����һ��Բ�Ǿ�������,������������Բ�ǵĴ�С.
		bool CreateRoundRect(int left,int top,int right,int bottom,int width,int height){return Attach(CreateRoundRectRgn(left,top,right,bottom,width,height));}
		//�������.�ο�������ö������.
		int Combine(HRGN rgn,DWORD cbMode = Or);
		//ƽ��һ������.
		bool Offset(int dx,int dy)		{return 0!=::OffsetRgn(_Handle,dx,dy);}
		//ƽ��һ������.
		bool Offset(Twin dim)	{return 0!=::OffsetRgn(_Handle,dim.x,dim.y);}
		//���������Ƿ�һ��.
		inline bool operator == (HRGN rgn){return ::EqualRgn(_Handle,rgn)!=0;}
		//��һ��RGN��ֵ������һ��,���Ϊ0,��ɾ���󶨵�RGN.
		void operator = (HRGN rgn);
		//��һ��RGN��ֵ������һ��,���Ϊ0,��ɾ���󶨵�RGN.
		void operator = (Rgn& rgn);
		//��������.
		inline bool Paint(HDC hdc){return ::PaintRgn(hdc,_Handle)!=0;}
		//���ذ����������С����.
		inline bool GetRectBox(LPRECT pr){return 0!=::GetRgnBox(_Handle,pr);}
		//���Ƿ���������.
		inline bool PtInRgn(Twin pt){return ::PtInRegion(_Handle,pt.x,pt.y)!=0;}
	};
	class CONCISE_API Pen : public GdiObject<HPEN>
	{
	public:
		//����һ��PEN����.
		bool Create(COLORREF color,int style = 0,int width = 1);
		//��ϵͳ��DC_PEN����.
		bool CreateDCPen(){return Attach((HPEN)::GetStockObject(DC_PEN));}
		//���û�����ɫ.�������CreateDCPen�����ֶ���DC_PEN.
		COLORREF SetDCColor(HDC hdc,COLORREF color);
		//��ȡ������ɫ.�������CreateDCPen�����ֶ���DC_PEN.
		COLORREF GetDCColor(HDC hdc);
		//���ػ�����Ϣ.
		bool GetLogPen(LOGPEN* lp){return GetObject(lp);}
		//���ػ�����ɫ.
		COLORREF GetColor();
	};
	class CONCISE_API Brush : public GdiObject<HBRUSH>
	{
	public:
		enum {Black = 4,DarkGray = 3,Gray = 2,Null = 5,White = 0,DC = 18};//ϵͳ����ˢ��.
		//����.
		bool Create(COLORREF color,UINT style = BS_SOLID,DWORD hatch = 0);
		//��ϵͳ���õ�DC_BRUSH����.
		bool CreateDCBrush(){return Attach((HBRUSH)::GetStockObject(DC_BRUSH));}
		//set the DC_BRUSH brush color,it can be select in dc later;
		COLORREF SetDCColor(HDC hdc,COLORREF color){return ::SetDCBrushColor(hdc,color);}
		//get the DC_BRUSH brush color
		COLORREF GetDCColor(HDC hdc){return ::GetDCBrushColor(hdc);}
		//����.
		bool CreatePattern(HBITMAP bmp){return Attach(::CreatePatternBrush(bmp));}
		//����ϵͳ������ɫ.
		bool GetSysColorBrush(int sysIndex){return Attach(::GetSysColorBrush(sysIndex));}
		//����DC��λ��.
		bool SetOrigin(HDC hdc,int x,int y){return 0!=::SetBrushOrgEx(hdc,x,y,0);}
		//ȡ����ʼλ��.
		bool GetOrigin(HDC hdc,POINT* pt){return 0!=GetBrushOrgEx(hdc,pt);}
		//����ˢ����Ϣ.
		bool GetLogBrush(LOGBRUSH* lb){return GetObject(lb);}
		//������ɫ.
		COLORREF GetColor();
	};
	//Font�ľ���������Դ�LogFont()���ص�ָ��������.���û�е���Create����,ʵ���ϲ�û��������Դ����
	//������ʱ�Կ�����Ϊһ��LOGFONT�ṹʹ��,�ܶ�����Ĳ���ʵ����ֻҪ���LOGFONT����,��������������.
	class CONCISE_API Font : public GdiObject<HFONT>{
	protected:
		Object<LOGFONT> _LogFont;
	public:
		Font(void){_LogFont.New();Reset();}
		//��������,ʹ��_LogFont�Ĳ���.
		bool Create();		
		//��������Ϊȱʡ����.��Ч��Ҫ���µ���Create����.
		void Reset();	
		//�������������.��Ч��Ҫ���µ���Create����.
		void SetName(LPCWSTR fontName){WcsCopy(_LogFont->lfFaceName,fontName,31);}
		//��������Ĵ�С.��Ч��Ҫ���µ���Create����.
		void SetSize(LONG size){_LogFont->lfHeight = size;}
		//�����������������LOGFONT�ṹָ��.
		LPLOGFONT LogFont(){return _LogFont.Handle();}
		//��һ����֪��LOGFONT��ʼ���������������LOGFONT�ṹָ��.
		void SetLogFont(LPLOGFONT logfont){memcpy(_LogFont.Handle(),logfont,sizeof(LOGFONT));}
		//����������Ϣ, ����洢��logfont��Ա.
		bool GetLogFont(){return GetObject(_LogFont.Handle());}
	};
	//DC�����ֶ��ͷ���Դ��GetDC��GetWindowDC�õ��ľ��ʹ��ReleaseDC�ͷţ�Create�õ��ľ����ʹ��DeleteDC�ͷš�
	class CONCISE_API DC : public HandleType<HDC>{
	public:
		void operator = (HDC hdc){
			_Handle = hdc;
		}
		bool CreateCompatible(HDC hdc = NULL);
		template<typename T> T SelectObject(T hGdi){
			return (T)::SelectObject(_Handle,hGdi);
		}
		//���������ڵ�DC�����������ͱ߽�,����������.
		bool GetWindowDC(HWND hWnd);
		//��һ�����ڵĿͻ���DC��0 ��ʾǰ̨���ڵ� DC .
		bool GetDC(HWND hWnd = 0);
		//ɾ�����ͷŰ󶨵�DC��Դ.
		bool DeleteDC();
		bool ReleaseDC(HWND hWnd = 0);
		//�˺���������ϵͳ�ں��ڴ治���ʧ��.
		inline HBITMAP CreateCompatibleBitmap(int cx = 0,int cy = 0){return ::CreateCompatibleBitmap(_Handle,cx,cy);}
		inline bool MoveTo(int x,int y,LPPOINT pt = 0){return 0!=::MoveToEx(_Handle,x,y,pt);}
		inline bool LineTo(int x,int y){return 0!=::LineTo(_Handle,x,y);}
		inline int GetClipRect(LPRECT clip){return ::GetClipBox(_Handle,clip);}
		inline int GetDeviceCaps(int index){return ::GetDeviceCaps(_Handle,index);}//HORZRES for screen width,VERTRES for height,BITSPIXEL for color depth.
		inline UINT SetTextAlign(int align){return ::SetTextAlign(_Handle,align);}
		inline COLORREF SetTextColor(COLORREF color){return ::SetTextColor(_Handle,color);}
		inline COLORREF SetBkColor(COLORREF bk){return ::SetBkColor(_Handle,bk);}
		inline bool SetBkTransparent(bool transparent = true){return ::SetBkMode(_Handle,transparent?TRANSPARENT:OPAQUE)!=0;}
		inline int DrawText(LPCWSTR str,LPRECT rect,UINT flags){return ::DrawTextW(_Handle,str,(int)wcslen(str),rect,flags);}
		inline HPALETTE SelectPalette(HPALETTE hpl,bool force = 0){return ::SelectPalette(_Handle,hpl,force);}
		inline int SetStretchBltMode(int stretchMode){return ::SetStretchBltMode(_Handle,stretchMode);}
		inline UINT RealizePalette(){return ::RealizePalette(_Handle);}
		inline COLORREF SetDCPenColor(COLORREF color){return ::SetDCPenColor(_Handle,color);}
		inline COLORREF SetDCBrushColor(COLORREF color){return ::SetDCBrushColor(_Handle,color);}
		inline COLORREF GetDCPenColor(){return ::GetDCPenColor(_Handle);}
		inline COLORREF GetDCBrushColor(){return ::GetDCBrushColor(_Handle);}
		inline HBRUSH GetDCBrush(){return (HBRUSH)::GetStockObject(DC_BRUSH);}
		inline HPEN GetDCPen(){return (HPEN)::GetStockObject(DC_PEN);}
		inline HPEN SelectDCPen(){return SelectObject(GetDCPen());}
		inline HPEN ReleaseDCPen(HPEN old){return SelectObject(old);}
		inline HBRUSH SelectDCBrush(){return SelectObject(GetDCBrush());}
		inline HBRUSH ReleaseDCBrush(HBRUSH old){return SelectObject(old);}
	};
	/*�����ɫ������Ϊ0,DibOffset��PixelOffset�������̶���ֵ14��54,24��32λͼ��.ÿ�е�����������һ��4�ֽڶ��������,
	һ�е����������ֽ���һ����4�ı���.
	DibOffset             PixelOffset                       ��������  
	|___________________|_____________________|________________________________________________________________________________|*/
	class CONCISE_API Bitmap : public GdiObject<HBITMAP>{
	public:
		inline Bitmap(){}
		//��һ��λͼ�����ʼ��.
		Bitmap(HBITMAP bmp){Attach(bmp);}
		//����һ��ͼ����Դ����һ��BMP�ļ�.resInst = 0��ʾ����һ��λͼ�ļ�.cx,cy��ָ����λͼ�ߴ�,Ĭ�ϱ�ʾ��ԭ��С����.
		bool Load(ResID res,int cx = 0,int cy = 0);//resInst = 0 indicate load from file
		//����ͼ���ļ���ʹ��gdi+��֧�ָ����ļ���ʽ��
		bool Load(LPCWSTR file);
		//����ͼ�����ݣ�ʹ��gid+�����ݱ������ļ���ʽ��
		bool Load(void* data,int len);
		//dib����һ��bmp�ļ��������ļ�ͷ�Ĳ��֡�
		int GetDib(Memory<unsigned char>& dib,WORD bits = 0);
		//����ΪBmpͼ���ļ���ֱ�Ӳ������ݣ�û��ʹ��gid+��BitsPixel���ļ���ɫ��,0��ʾΪ��Ļ��ɫ��.
		bool SaveBmp(LPCWSTR lpFileName,WORD BitsPixel = 0);
		//����ͼ���ļ����ļ��������ļ����ĺ�׺������quality��ͼ����������ֵ�� 0����100
		bool Save(LPCWSTR file,long quality = 60);
		//����ͼ���ڴ棬����������type���������硰.jpg������Ϊjpg���͵����ݣ���
		int Save(Memory<char>& mem,LPCTSTR type,long quality = 60);
		//��ͼ����������ͼ��������һ��BITMAPINFOHEADER��ͷ��.���������һ��bmp�ļ�����file��1.
		bool CreateFromMemory(LPVOID data,bool file = 0);
		//����һ����Ļɫ���λͼ��
		bool CreateCompatibleBitmap(int cx,int cy);
		//����һ��2λ��λͼ.
		static HBITMAP CreateMonochroic(int cx = 0,int cy = 0);
		//��ͼ���ȡͼ��,��Ϊͼ���������ͼ����Դ(��ɫ��MASKͼ),color�������û�ȡ��һ��.
		bool FromIcon(HICON hIcon,bool color = 1);
		//�����������ͼ��,hBmp��Ӧ����Դ�����ͷ�.
		void operator = (HBITMAP hBmp);//copy the bitmap
		//��bitmap����ͼ��.
		void operator = (Bitmap& bitmap);

		//�������ô�С.
		bool Resize(int cx,int cy);
		//����һ��ԭ����ͼ��Ƭ����Դ.Ҫ�Լ�������Դ���ͷ�.
		HBITMAP GetClip(int cx = 0,int cy = 0,int x = 0,int y = 0);
		//����ͼ��,�����������ʵ����.
		bool ClipBitmap(int cx = 0,int cy = 0,int x = 0,int y = 0);
		//ȡ��λͼ����������,���ݱ�����data��,���ȼ���data�Ĵ�С.BitsPixel = 0��ʾ����Ϊ��Ļɫ��.����ͼ��ĸ߶�.
		//�����Ǐ����½��_ʼ�������ң��������ϵ����ÿ��32λ�D������؞�4���ֹ������飺�{���̣��죬alpha��
		int GetPixelData(Memory<unsigned char>* p,WORD bits = 0);
		//����λͼ����������,���ݱ�����data��.����ͼ�������(�߶�).
		int SetPixelData(Memory<unsigned char>* p,WORD bits = 0);
		//����λͼ����������,���ݱ�����data��.����ͼ�������(�߶�).
		int SetPixelData(const unsigned char* p,uint len,WORD bits = 0);
		//����λͼ�ĳߴ�.
		Twin Size();
		//��ͼ����ض���ɫ����ת����͸����
		bool ToAlpha(Color c1,Color c2);
		//��ͼ����ض���ɫ����ת����͸����
		bool ToAlpha(Color c);
		//StretchBlt,����ͼ��,hdc,��Ҫ���Ƶ�DC,x,y����ʼλ��,cx,cy�ǻ�������ĳߴ�,sx,sy��Դͼ�����ʼλ��.
		//scx,scy��Ҫ����Դͼ��Ĵ�С,���ܳ���ʵ�ʴ�С.raster�ǻ�ͼ��ʽ.
		bool Draw(HDC hDC,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0,DWORD raster = SRCCOPY);
		//��������ῼ��alphaͨ��������,����ֻ����32λ��λͼ(ʵ���������Ļ��32λ�ģ����м��ص�λͼ����32λ��)
		//����������Draw����ͬ��.alpha������͸����,0-255֮��ȡֵ.
		bool DrawAlpha(HDC hdc,BYTE alpha = 255,bool alphaChannel = 0,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0);
	};
	//����������ڻ���λͼ,����������һ��HDC����HBITMAP.
	/*class CONCISE_API DrawBitmap : public HandleType<HDC>
	{
	protected:
		HBITMAP _Old;//DCԭ����λͼ����������������Դ�����
	public:
		//���캯��
		DrawBitmap();
		//����.
		~DrawBitmap();
		//�����ڲ� DC ��������������Զ��ͷ�ԭ���� DC ����
		bool Create();
		//�ͷ� DC ��Դ��λͼ��Դ������Ѿ���λͼ����
		bool Close();
		//��һ��λͼ��Դ, ����ɾ��ԭ���󶨵�λͼ��Դ��hBmp λͼ��Դ��������������ͷš�
		bool AttachBitmap(HBITMAP hBmp);
		//����󶨵�λͼ��Դ����һ�����ڲ�ϣ��λͼ���ͷŵ������
		HBITMAP DetachBitmap();
		//���ذ�λͼ��һ��������
		HBITMAP GetBitmap();
		//λͼ�Ĵ�С
		Twin Size();
		//��������λͼ�Ĵ�С��
		bool Resize(int cx,int cy);
		//ɾ����Դ.
		bool Delete();
		//����ͼ��,hdc,��Ҫ���Ƶ�DC,x,y����ʼλ��,cx,cy�ǻ�������ĳߴ�,sx,sy��Դͼ�����ʼλ��.
		//scx,scy��Ҫ����Դͼ��Ĵ�С,���ܳ���ʵ�ʴ�С.raster�ǻ�ͼ��ʽ.
		bool Draw(HDC hDC,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0,DWORD raster = SRCCOPY);
		//��������ῼ��alphaͨ��������,����ֻ����32λ��λͼ(ʵ���������Ļ��32λ�ģ����м��ص�λͼ����32λ��)
		//����������Draw����ͬ��.alpha������͸����,0-255֮��ȡֵ.
		bool DrawAlpha(HDC hdc,BYTE alpha = 255,bool alphaChannel = 0,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0);
	};*/
	//Icon �� Cursor ���ڲ�Ӧ������ͬ��.�������и��Եı�ʶ.������Դ���ļ������ǵ�����ȴ�ϴ�(����΢��İܱ�,
	//��ΪGDI��Դ,����û���κ�����),��ȷҪʹ�ø��Եĺ���.
	class CONCISE_API Icon : public HandleType<HICON>
	{
	public:
		Icon(void);
		Icon(HICON icon){Attach(icon);}
		Icon(ResID res){LoadIcon(res);}
		~Icon(void);

		bool Attach(HICON hIcon);
		bool Attach(HICON hIcon,PICONINFO pii);
		HICON Detach();

		bool LoadIcon(ResID res,int cx = 0,int cy = 0);
		bool LoadCursor(ResID res);
		bool LoadFile(LPCWSTR file,int cx = 0,int cy = 0);

		bool FromResource(LPBYTE hg,DWORD bits,int cx,int cy);
		//hBmp��λͼ��alpha��͸��ͨ����cx����ȣ�cy�����ȣ�x����ʼλ��x���꣬y����ʼλ��y���꣬hotX���ȵ�xλ�ã�hotY���ȵ�yλ�á�
		bool FromBitmap(HBITMAP hBmp,COLORREF alpha = -1,int cx = 0,int cy = 0,int x = 0,int y = 0,int hotX = 0,int hotY = 0);

		bool Draw(HDC hDC,int x,int y,int cx = 0,int cy = 0);
		bool Draw(HDC hdc,Rect* rect,bool stretch = 0);
		HICON GetCopy();

		bool Delete();

		void operator = (HICON hIcon);
		void operator = (Icon& icon);
		inline bool IsIcon() const {return _IsIcon;}

		inline int HotX() const {return _HotX;}
		inline int HotY() const {return _HotY;}
		inline Twin Size() const {return _Size;}
	protected:
		Twin _Size;
		DWORD _HotX,_HotY;
	private:
		bool _IsIcon;
	};
	inline WORD PaletteSize(WORD color)
	{
		if(color>8) return 0;
		return (1<<color)*sizeof(RGBQUAD);
	}
	CONCISE_API bool IconToBlackWhite(Icon* icon);
	CONCISE_API bool BitmapToBlackWhite(Bitmap& bmp);
	CONCISE_API HBITMAP CreateBitmap(HDC hdc,int cx,int cy);
	class CONCISE_API GdiSelector : public _class{
	protected:
		HGDIOBJ _old;
		HDC		_dc;
	public:
		GdiSelector():_old(0),_dc(0){}
		GdiSelector(HDC hdc,HGDIOBJ obj){
			_old = ::SelectObject(hdc,obj);
			_dc = hdc;
		}
		void Select(HDC hdc,HGDIOBJ obj){
			_old = ::SelectObject(hdc,obj);
			_dc = hdc;
		}
		void Close(){
			if(_dc){
				::SelectObject(_dc,_old);
			}
		}
		~GdiSelector(){
			if(_dc){
				::SelectObject(_dc,_old);
			}
		}
	};
	CONCISE_API SIZE CalcTextSize(HDC hdc,String& text,HFONT font = 0);
	class CONCISE_API BitmapList : public _class{
	protected:
		HDC _dc;
		IntegerDiction<HBITMAP> _bmpList;
	public:
		BitmapList():_dc(0){
		}
		uint Count(){
			return _bmpList.Count();
		}
		HBITMAP GetBitmap(uint index){
			return _bmpList.Element(index);
		}
		bool Delete(uint index){
			HBITMAP bmp = _bmpList.Element(index);
			if(!bmp) return 0;
			::DeleteObject(bmp);
			return 1;
		}
		bool DeleteByValue(HBITMAP bmp){
			if(_bmpList.DeleteValue(bmp)){
				::DeleteObject(bmp);
				return 1;
			}
			return 0;
		}
		bool Add(HBITMAP bmp,uint index = -1){
			bool exist = 0;
			_bmpList.Add(bmp,index,&exist);
			return exist;
		}
		//���ļ����ͼ�񣬳ɹ���������ͼ��ľ��
		HBITMAP Add(LPCWSTR file,uint index = -1);
		//����Դ����ͼ�񣬳ɹ���������ͼ��ľ��
		HBITMAP Add(ResID res,ResID type,uint index = -1);
		HBITMAP Remove(uint index){
			HBITMAP bmp = _bmpList.Element(index);
			if(!bmp) return 0;
			_bmpList.Delete(index);
			return bmp;
		}
		uint IndexOf(HBITMAP bmp,uint start = 0){
			return _bmpList.IndexOf(bmp,start);
		}
		void Clear(){
			for(uint i=0;i<_bmpList.Count();i++){
				::DeleteObject(_bmpList[i]);
			}
			_bmpList.Clear();
		}
		~BitmapList(){
			if(_dc) ::DeleteDC(_dc);
		}
		bool Draw(HBITMAP bmp,HDC hdc,uchar alpha = 255,bool alphaChanel = false,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0){
			if(!bmp) return 0;
			if(!_dc) _dc = ::CreateCompatibleDC(NULL);
			if(!_dc) return 0;

			AutoStruct(BITMAP,bp);
			if(sizeof(BITMAP)!=::GetObject(bmp,sizeof(BITMAP),&bp)) return 0;

			BLENDFUNCTION bf;
			bf.AlphaFormat = alphaChanel;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;//AC_SRC_OVER = 0;
			bf.SourceConstantAlpha = alpha;
			if(cx<=0) cx = bp.bmWidth;
			if(cy<=0) cy = bp.bmHeight;
			if((sx<0)||(sy<0))
				return 0;
			if(scx==0) scx = bp.bmWidth - sx;
			if(scy==0) scy = bp.bmHeight - sy;
			if(((scx+sx)>bp.bmWidth)||((scy+sy)>bp.bmHeight))
				return 0;
			GdiSelector gs(_dc,bmp);
			return ::AlphaBlend(hdc,x,y,cx,cy,_dc,sx,sy,scx,scy,bf)!=0;
			//this function is most slowly,when alpha is not 255 and source rect width destination rect
			// is not equal in size,especially last reason.
		}
		bool Draw(int index,HDC hdc,uchar alpha = 255,bool alphaChanel = false,int x = 0,int y = 0,int cx = 0,int cy = 0,int sx = 0,int sy = 0,int scx = 0,int scy = 0){
			HBITMAP bmp = _bmpList.Element(index);
			return Draw(bmp,hdc,alpha,alphaChanel,x,y,cx,cy,sx,sy,scx,scy);
		}
	};
};