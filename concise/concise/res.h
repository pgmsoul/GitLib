#pragma once

namespace cs{
	//ResID�ṹ���ڱ�ʾһ����Դ�ı�ʶ,��Ϊ��ͨ����һ��LPCWSTR����,��Ҳ������һ������(��ʱ��λΪ0),����ṹ������ͳһ.
	//�����������ṹ��Ϊ�����Ĳ���,���������������������ֵ���ִ�ֵ,������д�����������߽�������ת��.
	typedef struct CONCISE_API ResID : public _class{
		HINSTANCE	Instance;
		LPCWSTR		Handle;

		inline ResID(){
			Instance = GetInstance();
			Handle = 0;
		}
		inline ResID(ResID& ri){
			*this = ri;
		}
		inline ResID(int id){
			Handle = (LPCWSTR)(INT_PTR)id;
			Instance = GetInstance();
		}
		inline ResID(LPCWSTR name){
			Handle = name;
			Instance = GetInstance();
		}
		inline ResID(LPCWSTR name,HINSTANCE inst){
			Handle = name;
			Instance = inst;
		}
		inline bool IsOrdinal(){
			return (0xFFFF0000&(INT_PTR)Handle)==0;
		}
		inline void operator = (ResID& ri){
			Handle = ri.Handle;
			this->Instance = ri.Instance;
		}
		inline void operator = (LPCWSTR name){
			Handle = name;
		}
		inline void operator = (int id){
			Handle = (LPCWSTR)(INT_PTR)id;
		}
		inline operator LPCWSTR (){return Handle;}
	}ResID;
	//ResName����һ����Դ�ı�ʶ,�������ʶ��һ��LPCWSTR����ʱ,��Ҫ�����ڴ�,���������Զ�������ͷ���Ӧ���ڴ�.
	//��Ҳ���԰�ȫ����Ϊ��������(��ֵ��ʽ),�����ֳ�����ResID���㹻��(��Դռ��ҪС).
	//ResName����ResID����,����������ԴID��ֵ,���ұ������ֵָ����ַ��������ResIDָ�����һ��
	//�ִ�����ô����ָ����ִ������ˣ���ô��Ҳ������Ч�ˣ�����ResID��������ִ�������������
	//��ֵ��ԭ�ִ���
	class CONCISE_API  ResName : public _class
	{
	protected:
		LPCWSTR _Handle;
	public:
		ResName():_Handle(0){
		}
		ResName(ResID name):_Handle(0){
			*this = name;
		}
		//����������Ա���ResName��Ϊ��������ʱ�ĳ�Ա����.���������������.
		ResName(ResName& rn):_Handle(0){
			*this = rn;
		}
		void operator = (ResID name);
		inline bool IsOrdinal(){
			return (0xFFFF0000&(INT_PTR)_Handle)==0;
		}
		inline void operator = (ResName& rn){
			*this = rn._Handle;
		}
		inline operator LPCWSTR (){
			return _Handle;
		}
		~ResName();
	};
	//��ȡ��Դ������ָ��,module = 0��ʾ�ӵ�ǰ���������Դ.������������ṩ��Դ������ָ��,����������Դ.
	//����Windows���õ���Դ����,����Ӧ��API�ṩ���غ���,��Ҫ�����������������Ӧ����Դ.
	CONCISE_API void* GetResource(ResID name,ResID type,DWORD* size = 0);

	/* �˵�������Ҳ��һ������,�˵��ľ�����Ǵ��ڵľ��.ÿһ���Ӳ˵�,����һ������(�����԰������ɲ˵���).�˵�����һ
	���������ṹ�Ĳ˵���,ÿһ���˵�������ı�,�˵�ID(Ҳ��������ID),�Ӳ˵�����Ϣ(���ֲ˵���һ���ǲ����Ե��).ID����
	����;,һ����������ʶ�˵�,��һ���ǵ���˵���,��������ڷ��͵�WM_COMMAND��ID.ǰһ������,ID��ȡֵ������UINT,��һ
	����;,ID������һWORDֵ.����,���ǿ��Ե���Ĳ˵�,ID������һ��WORDֵ,�������Ӳ˵��Ĳ˵����Split�˵����ID���Գ�
	��WORD��ֵ,��ʵ��ϵͳ���Ǹ���һ��˵������һ������WORD��ֵ(��������˵���һ�㲻����ID����,����ϵͳ�Զ�����ID).
	����,ID������ͬ,������ID�������˵���,�����ɿ�.
	    ��λ�������˵�����ٿɿ�,��ĳЩʱ����ʹ��ID��������,����ID�����ǲ˵��Ķ༶����,���������������λ������,
	λ������ֻ�����ڵ�ǰ�˵��Ĳ˵���.
	    ��ʵ�˵���Ϊ�����˵����Ӳ˵�,�����б��ʵ�����.����һ�������˵�����һ���Ӳ˵�,��һ�����ڲ˵���һ�������˵�,
	�����ǲ��ܻ��õ�.LoadMenu���ص���һ�������˵�.��Ӧ�ĺ����ֱ���CreateMenu��CreatePopupMenu(ǰ������һ�������˵�,
	��������һ���Ӳ˵�;*/
	class CONCISE_API  Menu : public HandleType<HMENU>
	{
	public:
		Menu();
		~Menu();
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
		HMENU AddSubMenu(UINT uPos,LPCWSTR text,HMENU sub = NULL,UINT id = 0,UINT mf = MF_BYPOSITION);
		//���һ���˵��ָ���(��������Ҳ��һ���˵���,��������һ���ָ�����,���߱��˵�����������).
		bool AddSplit(UINT uPos,UINT mf = MF_BYPOSITION);
		//һ����˵�Ӳ˵�ID��δ֪�ģ�ϵͳ�Զ����䣬��������֪�����������Ӳ˵�ֻ��λ��������
		HMENU GetSubMenu(UINT uPos);
		//ȡ�õ�ǰ�˵��Ĳ˵�����������˵�����Ӳ˵���
		int GetItemCount();

		//�����WM_CONTEXTMENU��Ϣ�е�����Щ����������ֱ�Ӱ�lParam�����������������Ҽ���Ϣ�е�����Щ��������Ҫ������ת��Ϊ��Ļ���꣬Ҳ���ǵ��� ClientToScreen.
		//�����˵���x��y��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool Popup(short x,short y,HWND hOwner,UINT tpm = 0);
		//�����˵���lParam��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool Popup(LPARAM lParam,HWND hOwner,UINT tpm = 0);
		//�����Ӳ˵���uIndex�ǲ˵���λ��������lParam��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool PopSubMenu(UINT uIndex,LPARAM lParam,HWND hOwner,UINT tpm = 0);
		//�����Ӳ˵���uIndex�ǲ˵���λ��������x��y��λ�ã�hOwner����Ӧ�Ĵ��ڣ�����Ϊ�㣬����˵��������٣�tpm�ǵ���ѡ�
		bool PopSubMenu(UINT uIndex,short x,short y,HWND hOwner,UINT tpm = 0);
		//��Ϊ���ڶ����˵�������ֻ���������Ӳ˵�.
		bool Popup(HWND hOwner,UINT tpm = 0);
		bool PopupSubMenu(UINT uPos,HWND hOwner,UINT tpm = 0);
		//��ʵ���ʹ��ڲ˵�������������DC�����GDI��Դ��Select����.
		bool SelectWindow(HWND hWnd);
		//��һ�����ڵĲ˵����˵��ʹ��ڹ�ͬӵ������˵�����
		bool BindFromWindow(HWND hWnd);
		//�󶨵�ǰ�˵���һ�����ڣ��˵��ʹ��ڹ�ͬӵ������˵�����
		bool BindToWindow(HWND hWnd);

		//�˵������غ���.
		bool SetItemState(UINT uPos,UINT mfs,UINT mf = MF_BYPOSITION);
		UINT GetItemState(UINT uPos,UINT mf = MF_BYPOSITION);//(8),MF_DISABLED(2),MF_MENUBREAK(64),MF_OWNERDRAW(0x100),MF_POPUP(0x10),MF_SEPARATOR(0x800) 

		bool DeleteItem(UINT uPos,UINT mf = MF_BYPOSITION);//��һ���˵���(������һ���Ӳ˵�),�Ӳ˵����Ƴ�,����ɾ����.
		bool AddItem(UINT uPos,LPCWSTR text,UINT msgId,UINT mf = MF_BYPOSITION);
		//һ���Ӳ˵���Ҳ������һ��ID,�������IDֻ��Ϊ��ʶʹ��,���Ծ���ʹ��WORD֮���ֵ,Ҳ������ϵͳ��ָ��һ��ֵ(�����Ǵ���WORD).
		//һ���Ӳ˵�,����������һ���˵���,�������Ի�˵���,�����.�����Ӳ˵��ľ��,���sub=0,�����������һ���յ��Ӳ˵�.
		bool RemoveItem(UINT uPos,UINT mf = MF_BYPOSITION);//��һ���˵���(������һ���Ӳ˵�),�Ӳ˵����Ƴ�,������ɾ����,�����ڱ𴦼���ʹ����.

		bool SetItemType(UINT uPos,UINT mft,UINT mf = MF_BYPOSITION);
		UINT GetItemType(UINT uPos,UINT mf = MF_BYPOSITION);

		bool SetItemCheck(UINT uPos,bool checked = 1,UINT mf = MF_BYPOSITION);
		bool GetItemCheck(UINT uPos,UINT mf = MF_BYPOSITION);

		bool SetItemDisable(UINT uPos,bool disable = 1,UINT mf = MF_BYPOSITION);
		bool GetItemDisable(UINT uPos,UINT mf = MF_BYPOSITION);

		bool SetItemText(UINT uPos,LPCWSTR text,UINT mf = MF_BYPOSITION);
		int GetItemText(UINT uPos,String& text,UINT mf = MF_BYPOSITION);

		bool SetItemID(UINT uPos,DWORD id,UINT mf = MF_BYPOSITION);
		UINT GetItemID(int index);
	};
};