#pragma once
namespace cs{
	//LayoutParam
	struct CONCISEUI_API LayoutParam : public _struct{
		enum{
			VERTICAL = 0,		//��ֱ����: ��Ԫ�ش�ֱ��������, ˮƽ�Զ�.
			HORIZONTAL = 1		//ˮƽ����: ��Ԫ��ˮƽ��������, ��ֱ�Զ�.
		};
		Rect		Margin;
		float		Weight;	
		int			Style;
		int			Length;

		void SetVertical();
		void SetHorizontal();
		inline bool IsHorizontal(){return (Style&HORIZONTAL)==HORIZONTAL;}
		inline bool IsVertical(){return (Style&HORIZONTAL)==0;}
		void Reset();
		void SetDimen(int length,float weight);
		void SetMargin(int left,int top,int right,int bottom);
		LayoutParam();
	};
	/**����ϵͳ�������£�
	1. �����������ǳ������ڣ����������߽磬����weight��length���ԡ���Ϊ����Layoutֻ����һ����
	2. һ�����ֵĶ���Ӳ��ְ����������Horizontal����Vertical������ˮƽ����ֱ˳�����С�
	3. �Ӳ��ֵĴ�С�����¹���ˮƽ���֣������Ӳ��ֵĸ���һ���ģ���ֱ���֣������Ӳ��ֵ�
	����һ���ģ����Ǽ��ϱ߽��Ⱥ���������֡�ˮƽ����ʱ�Ӳ��ֵĿ�ȷ���ʹ�ֱ�����Ӳ��ֵ�
	�߶ȷ��䣬����ѭ�ȷ���ÿ���Ӳ���ָ�����Ⱥͱ߽�ֵ��ʣ��ߴ簴Ȩ�أ�weight�������ÿ����
	���ֵ�ԭ��ע�⣺��ʣ��ߴ�Ϊ���������������Ȩ����Сÿ���Ӳ��ֵĳߴ硣
	4. �����Ӳ���Ȩ���ܺ�Ϊ 0 �������С��0.00001�����Ӳ��ֳߴ簴ָ�����ȷ��䣬�����Ӹ����ֵĳߴ磬�����������
	���ܵ����Ӳ��ֳ��������ֵ����Ρ�

	��ǰ���ֳߴ�������ȡ����ͬ���Ĳ��ֵĲ�����Param���͸����ֵĳߴ磬�������������Ӳ��ֵ�
	Ӱ�죬Ҳ�����Ӳ��ֵ��κβ�������Ӱ�츸���ֵĳߴ硣

	���磺������ʵ�ʷֵĵĳ���Ϊ110����2���Ӳ��֣��Ӳ��ֵĳ��ȶ���10��Ȩ�طֱ�Ϊ1.0��2.0��
	��ô��110 - ��10 + 10�� = 90�����90���ǰ�Ȩ�ط���Ŀɵ��ߴ磬��������Ȩ��1.0�Ĳ��֣���
	��30��Ȩ��2.0�Ĳ��ֵַ�60�����ǵ�ʵ�ʳߴ�ֱ�Ϊ10+30��10+60��������ǵı߽�ֵ������ 0 ��
	��ô��Ҫ��90�����ȥ�߽�ֵ��ʣ�µĳߴ��ȥ���䡣*/
	//Layout Object use for View layout,but pure Layout object not bind a View object.
	class CONCISEUI_API Layout : public TPTree<Layout>{
	protected:
		Twin16	_size;
		Twin16	_location;
		WndPtr	_wnd;		//��������ǰ󶨵Ĵ��ڡ�
		ICtrl*	_createWnd; //�Լ����ɿؼ�

		Twin _calcMinSize(Layout* lay,bool parentHor);
		//set realize dimension of this object,and his child will be resize
		void _setRect(int left,int top,int width,int height);
		//return fix size
		int _setChildHor(int left,int top,int width,int height);
		//return fix size
		int _setChildVer(int left,int top,int width,int height);
		//create all control in this object or his child.
		void _createControlTree(HWND parent);
	public:
		String			Name;
		LayoutParam		Param;

		Layout();
		~Layout();
		//set _createWnd CreateParam value,if _createWnd is null,return null.
		ICtrl* Layout::GetControl(){return _createWnd;}
		//this will make layout create control (when Refresh call) and delete it when destroy himself.-1 is to make it default value.
		//�����ڲ����ɿؼ�, �⺯�������󶨵Ŀؼ�, ���������Ѿ����ɵĿؼ�, �¿ؼ���Ҫ����refresh������.
		void SetControl(LPCWSTR className,WORD id,LPCWSTR text,DWORD style = -1,DWORD exstyle = -1);
		//set a created window as bind window,this will destroy inner control of _createWnd.
		void SetControl(HWND ctrl);
		//get bind window handle.
		HWND GetCtrlHandle(){return _wnd;}
		//get realize location of object
		Twin16 GetLocation(){return _location;}
		//get realize size of object
		Twin16 GetSize(){return _size;}
		//a point whether in this layout.
		bool PtInLayout(int x,int y);
		//clear all child and set object to default state
		void Reset();
		//convert to a json object.
		void ToJson(Json* json);
		//from a xml to create layout
		bool FromJson(Json* json);
		//when Param is changed or SetControl is called,call this function to refresh it and if parent != 0,will create control.
		virtual void Refresh(HWND parent);
		//save to a file normaly a .layout file
		bool Save(LPCWSTR file,DWORD code = CP_UTF8);
		//load from a .layout file.
		bool Load(LPCWSTR file,DWORD code = CP_UTF8);
		//load from resource .
		bool Load(ResID res,ResID rt = L"RT_LAYOUT",DWORD code = CP_UTF8);
		//return a layout that name is required.
		Layout* GetElementByName(LPCWSTR name);
		//�ƶ���ǰ Layout������ƶ��ı����ǵ���������һ������һ��ͬ�� Layout �� Length ���ԣ��������һ�� Layout û��ͬ������һ������һ�� Layout�����޷�����λ�õġ�
		//����λ�õķ��ȣ�����һ������һ��ͬ�� Layout �� Length ���Բ�С�� 0 ��
		//����ʵ�ʵ�����λ�÷��ȡ�
		int Move(int pix);
	};
	//TopLayout ����հ󶨴��ڵ� WM_SIZE ��Ϣ���Ӷ����� Layout �Ĳ��ֽ��е�����TopLayout �Ƕ��� Layout �������������� Layout ���� Layout��
	class CONCISEUI_API TopLayout : public Layout{
	protected:
		IWnd* _container;
		void _onSize(Message* msg,IWnd*);
	public:
		TopLayout();
		//��ȡ������������³ߴ硣
		Twin GetMinSize(){
			Param.Length = 0;
			return _calcMinSize(this,Param.IsHorizontal());
		}
		//this can be set before view is created.if view is a dlg set isdlg = 1 else set isdlg to 0
		void SetContainer(IWnd* view);
		//create all control and calc layout size,allways set parent param is 0,it is not use,this only to make it a override virtual function.
		//call this function in initialize,because no wm_size message was send,so every layout size has not calc.
		virtual void Refresh(HWND parent = 0);
		//clear all child and set object to default state
		void Reset();
	};
}
