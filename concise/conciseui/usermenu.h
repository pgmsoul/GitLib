#pragma once
namespace cs{
	typedef struct CONCISEUI_API USERMENUPARAM : public _struct{
		uint ItemHeight,_TextCY,LeftMargin,
			ImageX,ImageY;
		HBITMAP BmpCheck;
		int SelectBkColor,DefaultBkColor,SelectBorderColor,TextColor;
		HBRUSH Brush;
		HPEN Pen;
		Font	Font;
		USERMENUPARAM();
	}USERMENUPARAM;
	/*�˵�����������һ���������ڣ���������˵���Ϣ��������˵��޷��رգ�����κεط�������ʧ����
	������˵�������������ͨ��SetOwnerWnd�������˵���Ϣ���������������д����������˵���
	hOwner ����������ʹ�� SetOwnerWnd �Ĵ��ھ�������û�е��� SetOwnerWnd ������Ҫ�󶨵���
	IWnd�Ĵ����ϣ���������Ϣ��ֱ���� Hook �������κ�ʱ��������� hOwner ����Ϊһ����Ч��
	���ھ����

	���Ի�˵�������ID��ͬ�Ĳ˵�������Ӳ˵��ʹ���������ӵ��Ӳ˵������Ψһ ID �����ǲ�֧��
	���߳�ͬ������

	���һ�������������Ի�˵���Ҳ��������ͬ��ID����Ϊ��Ϣ�ǻ���һ��ģ�id �����ֲ˵���Ψһ�ı�ʶ��*/

	class CONCISEUI_API UserMenu : public HandleType<HMENU>{
	protected:
		IWnd* _parent;
	public:
		BitmapList*		ImageList;
		USERMENUPARAM	Param;
		UserMenu();
		~UserMenu();
		//���� WM_MEASUREITEM �ĺ��������û����Ч�ĵ��� SetParentWnd ��������Ҫ�ڲ˵��������ڵ���Ϣ�е������������
		void HookMeasureItem(Message* msg,IWnd*);
		//���� WM_DRAWITEM �ĺ��������û����Ч�ĵ��� SetParentWnd ��������Ҫ�ڲ˵��������ڵ���Ϣ�е������������
		void HookDrawItem(Message* msg,IWnd*);
		//���ô��ڵ���������parent ���Լ����ø������ HookMeasureItem �� HookDrawItem ��
		//parent = NULL��������ԭ���İ󶨣����parent��һ����ֵ��ԭ���İ󶨻��������Ұ󶨵��´����ϡ�
		void SetOwnerWnd(IWnd* parent);
		//�����ʹ�� UserMenu ���õĻ��ƣ�����ͨ������ OnMeasureItem������һ���������Լ�����
		Function<void,LPMEASUREITEMSTRUCT> OnMeasureItem;
		//�����ʹ�� UserMenu ���õĻ��ƣ�����ͨ������ OnDrawItem������һ���������Լ�����
		Function<void,LPDRAWITEMSTRUCT> OnDrawItem;

		//���ڲ˵���������Ӳ˵�,������������һ���Ӳ˵�.һ��Popup�˵�����һ���Ӳ˵�.
		bool Create(bool popup = 0);
		//����Դ���ز˵�,0��ʾ�ӵ�ǰӦ�ó���ʵ������.
		bool Load(ResID rMenu);
		//����˵��Ѿ���Ϊһ�����ڵĲ˵�,���������ʧ��,Ҳ����˵,��������һ���󶨵����ڵĲ˵�.������������ʱ���Զ�����˵���Դ.
		void Destroy();
		//����һ���˵�
		void Attach(HMENU hMenu);
		HMENU Detach(){return HandleType::Detach();}
		//uPos����ӵ�λ��,����ID����ʱ,uPos�����ǲ˵������⼶�Ӳ˵���ID,��ʾ���뵽���
		//�˵����ǰ��.����λ������ʱ,����˵���ֻ���ǵ�ǰ�˵�������,�ɹ�����˵���,����λ����������uPos.
		HMENU AddSubMenu(UINT uPos,LPCWSTR text,HMENU sub = NULL,HBITMAP image = 0,bool byPos = true);
		//���һ���˵��ָ���(��������Ҳ��һ���˵���,��������һ���ָ�����,���߱��˵�����������).
		bool AddSplit(UINT uPos,bool byPos = true);
		//һ����˵�Ӳ˵�ID��δ֪�ģ�ϵͳ�Զ����䣬��������֪�����������Ӳ˵�ֻ��λ��������
		HMENU GetSubMenu(UINT uPos);
		//ȡ�õ�ǰ�˵��Ĳ˵�����������˵�����Ӳ˵���
		int GetItemCount();
		//�Ѳ˵����Լ��������÷��Ϊ�Ի棬load��attach���Ի���Ĳ˵���Ҫ�������������
		void ToOwnerStyle();

		//�����WM_CONTEXTMENU��Ϣ�е�����Щ����������ֱ�Ӱ�lParam�����������������Ҽ���Ϣ�е�����Щ��������Ҫ������ת��Ϊ��Ļ���꣬Ҳ���ǵ��� ClientToScreen.
		//�����˵���x��y��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		//hOwner����������� parent ���ڣ�ͨ�� SetParent ��������Ч�Ĵ��ڶ��󣩣�������ھ����Ч��
		bool Popup(short x,short y,HWND hOwner = 0,UINT tpm = 0);
		//�����˵���lParam��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		//hOwner����������� parent ���ڣ�ͨ�� SetParent ��������Ч�Ĵ��ڶ��󣩣�������ھ����Ч��
		bool Popup(LPARAM lParam,HWND hOwner = 0,UINT tpm = 0);
		//�����Ӳ˵���uIndex�ǲ˵���λ��������lParam��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool PopSubMenu(UINT uIndex,LPARAM lParam,HWND hOwner,UINT tpm = 0);
		//�����Ӳ˵���uIndex�ǲ˵���λ��������x��y��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool PopSubMenu(UINT uIndex,short x,short y,HWND hOwner,UINT tpm = 0);
		//��Ϊ���ڶ����˵�������ֻ���������Ӳ˵�.
		bool Popup(HWND hOwner = 0,UINT tpm = 0);
		bool PopupSubMenu(UINT uPos,HWND hOwner = 0,UINT tpm = 0);
		//��ʵ���ʹ��ڲ˵�������������DC�����GDI��Դ��Select����.
		bool SelectWindow(HWND hWnd);
		//��һ�����ڵĲ˵����˵��ʹ��ڹ�ͬӵ������˵�����
		bool BindFromWindow(HWND hWnd);
		//�󶨵�ǰ�˵���һ�����ڣ��˵��ʹ��ڹ�ͬӵ������˵�����
		bool BindToWindow(HWND hWnd);

		bool DeleteItem(UINT uPos,bool byPos = true);//��һ���˵���(������һ���Ӳ˵�),�Ӳ˵����Ƴ�,����ɾ����.
		bool AddItem(UINT uPos,LPCWSTR text,UINT msgId,HBITMAP bmp = 0,bool byPos = true);
		//һ���Ӳ˵���Ҳ������һ��ID,�������IDֻ��Ϊ��ʶʹ��,���Ծ���ʹ��WORD֮���ֵ,Ҳ������ϵͳ��ָ��һ��ֵ(�����Ǵ���WORD).
		//һ���Ӳ˵�,����������һ���˵���,�������Ի�˵���,�����.�����Ӳ˵��ľ��,���sub=0,�����������һ���յ��Ӳ˵�.
		bool RemoveItem(UINT uPos,bool byPos = true);//��һ���˵���(������һ���Ӳ˵�),�Ӳ˵����Ƴ�,������ɾ����,�����ڱ𴦼���ʹ����.

		//�˵������غ���.
		bool SetItemState(UINT uPos,UINT mfs,bool byPos = true);
		UINT GetItemState(UINT uPos,bool byPos = true);//(8),MF_DISABLED(2),MF_MENUBREAK(64),MF_OWNERDRAW(0x100),MF_POPUP(0x10),MF_SEPARATOR(0x800) 

		bool SetItemType(UINT uPos,UINT mft,bool byPos = true);
		UINT GetItemType(UINT uPos,bool byPos = true);

		bool SetItemCheck(UINT uPos,bool checked = 1,bool byPos = true);
		bool GetItemCheck(UINT uPos,bool byPos = true);

		bool SetItemDisable(UINT uPos,bool disable = 1,bool byPos = true);
		bool GetItemDisable(UINT uPos,bool byPos = true);

		bool SetItemText(UINT uPos,LPCWSTR text,bool byPos = true);
		int GetItemText(UINT uPos,String& text,bool byPos = true);

		bool SetItemID(UINT uPos,DWORD id,bool byPos = true);
		UINT GetItemID(int index);

		bool SetItemImage(uint pos,HBITMAP bmp,bool byPos = true);//���ò˵����ͼ��
		HBITMAP GetItemImage(uint pos,bool byPos = true);//��ȡ�˵����ͼ��

		bool SetItemInfo(UINT uid,LPMENUITEMINFO mii,bool byPos = true);
		bool GetItemInfo(UINT uid,LPMENUITEMINFO mii,bool byPos = true);
	};
}