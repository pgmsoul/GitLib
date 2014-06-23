#pragma once
#define DEFTYPE(name) typedef struct name##__{int unused;}name##__,*name
#define WINMAIN		int __stdcall wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
#define MAIN		int _tmain(int argc, _TCHAR* argv[])
#define CONASSERT(msg) _ASSERT_EXPR(0,msg)
//��ȷһ�� 0 ������, ������ָ��, �������غ������ò���.
#define IntZero (int)0
#define UIntZero (uint)0
#define usexpstyle comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef ushort
typedef unsigned short ushort;
#endif
#ifndef uchar
typedef unsigned char uchar;
#endif
#ifndef ulong
typedef unsigned long ulong;
#endif
#ifndef uint64
typedef unsigned __int64 uint64;
#endif
#ifndef int64
typedef __int64 int64;
#endif

namespace cs{
	//ʹ��ϵͳ�ڴ���亯��, �ӳ����Ĭ�϶��Ϸ����ڴ�
	//��Ĭ�϶��Ϸ����ڴ�, ����������ڴ�, ���Կ�DLLʹ�úͷ���.
	//����һ���������ڴ�,��size = 0ʱ,���������Ȼ�����һ��1�ֽڵ��ڴ�,����һ����Ч���ڴ�ָ��.
	CONCISE_API void* SYSAlloc(int size);
	//���·���һ���ڴ�ָ��Ĵ�С,��p=0ʱ,����������ú�MemoryAlloc��ͬ.
	CONCISE_API void* SYSRealloc(void* p,int size);
	//�ͷ��Ѿ�������ڴ�.�ɹ�����true,ʧ�ܻ�pΪ0����false .
	CONCISE_API bool SYSFree(void* p);
	//�����ڴ�Ĵ�С,����0���p=0,���p����һ����Ч���ڴ�ָ��,������һ���쳣.
	CONCISE_API int SYSSize(void* p);
	
	//��ʼ�������ڴ�, ����������������κ�ʹ�ø����ڴ�ĺ���֮ǰ����, �����Զ���ʼ���Ķ���.
	/*@function bool SetFastMemory(int size = 16)
	@text ����Ԥ��������ڴ�Ĵ�С��Ԥ��������ڴ��Ӧ FastAlloc��FastRealloc��FastFree��FastSize��������Щ
	�����������ڴ涼��Ԥ����ģ������ٶȿ졣ϵͳĬ�ϵ�С�ڴ�����Ҳ�Ǵ�Ԥ�����ڴ���䣬�����ڴ�ֻ�Ƿ��䣬
	���·�����ͷŵ��ٶȿ죬����ָ�ڴ�����ݴ����ٶȿ죬�����ٶȺ�ϵͳ�ڴ���һ���ģ�����
	������ͷ��ٶȱ�ϵͳ�ڴ���ܸ߼�ʮ�����ϰٱ���
	@text �˺���ֻ���ڳ����ʼ��ʱ����һ�Σ�Ҳ����û���κ��ڴ溯��������֮ǰ��һ���и����ڴ溯��
	��ʹ�ã��������û�ʧ�ܣ��ظ�����Ҳ��ʧ�ܡ�
	@param size -- int
	@text Ҫ������ڴ��С����λ��MB��Ĭ��ֵ��16MB�����û�е������������ϵͳĬ�Ϸ���16MB�ĸ����ڴ档
	@return bool
	@text �����Ƿ�ɹ���*/
	//CONCISE_API bool SetFastMemory(int size = 16);
	/*@function void* FastAlloc(int size)
	@text ʹ�ÿ��ٷ����ڴ�, ����ڴ��ܵĿ�������ͨ��SetFastMemory�����õģ�
	Ĭ����16M. 32λ����ĵ�ַ�ռ�ֻ��4G��ʵ���ϵ��з��䳬��1G���ڴ�ʧ�ܵĸ���
	�ǳ��ߣ����ϵͳ���õ������ڴ��޹أ����ǵ�ַ�ռ�ľ������Դ��ڴ������32λ
	�����У���Ҫ����Ĳ�����������һ����ڴ���亯����
	@text ���������ڴ����1M������Ԥ�����ڴ��Ѿ��ľ����˺��������ϵͳ�ڴ�������ռ䡣
	@text һ����˵ String��Memory�ȶ���Ĭ�϶����ø����ڴ�������ģ����ԣ����
	��Щ�����д��ڴ�������Ҫ�ر�ָ��ʹ��ϵͳ�ڴ档
	@text Fast�ڴ������Windowsϵͳ�������100��, ���Ĳ�����Ԥ����һ���ڴ�, ע��, 
	�����Щ�ڴ�û��ʹ��, ������ռ�������ڴ�. ���ǻ�ռ�������ڴ�ҳ��, ���������
	�ܶ��ʵ��, �����ڴ�ҳ����ܻ��þ�. ��Ϊ�����ڴ�Ҳ�������޵ġ�
	@param size -- int
	@text ������ڴ��С��
	@return void*
	@text �ɹ������ڴ�ָ�룬ʧ�ܷ��� NULL ��
	*/
	CONCISE_API void* FastAlloc(int size);
	/*@function void* FastRealloc(void* p,int size)
	@text ���·���һ���ڴ�ָ��Ĵ�С,�� p=0 ʱ,����������ú�FastAlloc��ͬ. 
	��� size ���� 1M ����Ԥ�����ڴ��Ѿ��ľ��������������ϵͳ�ڴ������ռ䡣
	*/
	CONCISE_API void* FastRealloc(void* p,int size);
	/*@function bool FastFree(void* p)
	@text �ͷ��Ѿ�������ڴ�. ��Ϊ Fast ������Ȼ���ܷ���ϵͳ�ڴ棬���Դ˺���ʵ���Ͽɿ�������ϵͳ�ڴ��Ԥ�����ڴ档
	@return bool
	@text �ɹ�����true, ʧ�ܻ� p Ϊ 0 ���� false.
	*/
	CONCISE_API bool FastFree(void* p);
	//�����ڴ�Ĵ�С,ע��, ���p ��һ����Ч�ڴ�, ��Ȼ���ܷ���һ�������õĳߴ�.
	CONCISE_API int FastSize(void* p);
	//һ���ڴ�ָ���Ƿ���Ԥ�����ڴ棬�˺���ֻ�Ǽ򵥵ļ��ָ���Ƿ���Ԥ�����ڴ淶Χ�ڣ���Χ�ڵĴ���ָ����Ȼ���� true��
	CONCISE_API bool IsFastMemory(void* p);

	//FAST_MEMORY���ؿ���Memory��ͷ���ڴ���亯��ʹ���ĸ��汾(����Fast����ϵͳSYS).
	CONCISE_API void* MemoryAlloc(int size,bool sys = 0);
	CONCISE_API void* MemoryRealloc(void* p,int size,bool sys = 0);
	CONCISE_API bool MemoryFree(void* p,bool sys = 0);
	CONCISE_API int MemorySize(void* p,bool sys = 0);
	//���λ
	template<typename T> inline void BitAdd(T& bit,T add){
		bit |= add;
	}
	//�Ƴ�λ
	template<typename T> inline void BitRemove(T& bit,T remove){
		bit = bit&~remove;
	}
	//�ϴ�ֵ.
	template<typename T> T Maximum(T t1,T t2){
		if(t1>t2) return t1;
		else return t2;
	}
	//��Сֵ.
	template<typename T> T Minimum(T t1,T t2){
		if(t1<t2) return t1;
		else return t2;
	}
	//�Ƿ���һ�����(��λ�Ƿ�Ϊ0).
	template<typename T> bool IsHandle(T t){
		return (0xffff0000&(INT_PTR)t)!=0;
	}
	//�Ѿ����Ϊ����ֵ�����.
	template<typename T,typename R> inline int SubHandle(T a,R b){
		return (int)((UINT_PTR)a - (UINT_PTR)b);
	}
	//32λ��������ת��.
	template<typename OutType,typename InType> OutType Cast(InType inValue){
		return (OutType)(INT_PTR)inValue;
	}
	//ת��һ������Ϊ������һ������.
	template<typename OutType,typename InType> OutType SuperCast(InType inValue){
		union{
			OutType Out;
			InType In;
		}addr;
		addr.In = inValue;
		return addr.Out;
	}

	//������Ա��򵥸��Ƶ����������һ���ṹ������
	struct CONCISE_API _struct{
		_struct(){}
		void* operator new(size_t size){
			return MemoryAlloc((int)size);
		}
		void operator delete(void* p){
			MemoryFree(p);
		}
		inline void* operator new(size_t,void* where){
			return where;
		}
		inline void operator delete(void*,void*){}
		inline void* operator new(size_t size,LPCSTR lpszFileName,int nLine){  
			return MemoryAlloc((int)size);
		}
		inline void operator delete(void* p, LPCSTR lpszFileName, int nLine){  
			MemoryFree(p);
		}
	};
	//��������ڱ�����ȫ�ֵ�new�����ɶ���.�����Ķ��������DLL������,����Ӧ�ó������ͷ�.�������Ͷ���������.
	class CONCISE_API _class{
	public:
		_class(){}
		inline void* operator new(size_t size){
			return MemoryAlloc((int)size);
		}
		inline void operator delete(void* p){
			MemoryFree(p);
		}
		inline void* operator new(size_t,void* where){
			return where;
		}
		inline void operator delete(void*,void*){}
		inline void* operator new(size_t size,LPCSTR lpszFileName,int nLine){  
			return MemoryAlloc((int)size);
		}
		inline void operator delete(void* p, LPCSTR lpszFileName, int nLine){  
			MemoryFree(p);
		}
	private:
		//����������������������.(������������Ϊ����������Ӧ����������������,���������������ʾ����).
		_class(const _class& obj){};				
		//���ھ��������,�������Ҫ������Ϊ����,Ӧ�ò��ô���ַ�ķ�ʽ.
		void operator = (const _class& obj){};	
	};
	//Object����������װһ����, ���������ʹ�ð�װ��Ķ�����Ϊ�����������߷���ֵ, 
	//�����ÿ��Ǹ��ƹ��������, ��Ϊ������Զ�������Դ�ͷ�. ������൱�ڶ԰�װ�����
	//һ������, ����Object<A> a = b. a��bʵ���ϰ�װ����ͬһ������.
	template<typename T>class CONCISETL_API Object : public _struct{
	private:
		struct _H{
			T* obj;
			int icount;
		};
		_H* h;
	public:
		inline Object():h(0){
		}
		//���츴��
		inline Object(Object& a):h(0){
			if(!a.h) return;
			h = a.h;
			h->icount++;
		}
		//�Զ�����һ��ʵ��
		inline Object(bool create):h(0){
			if(create) New(new T);
		}
		//��ʼ����װһ������, obj��Ҫ�������ͷ�, ���ᱻ�Զ�����.
		inline Object(T* obj):h(0){
			New(obj);
		}
		//��װһ������, obj��Ҫ�������ͷ�, ���ᱻ�Զ�����.
		bool New(T* obj = 0){
			if(h){
				h->icount--;
				if(h->icount==0){
					delete h->obj;
					delete h;
				}
			}
			h = new _H;
			if(h==NULL) return 0;
			if(obj==NULL) obj = new T;
			h->obj = obj;
			h->icount = 1;
			return 1;
		}
		//��ֵ
		void operator = (Object& a){
			if(h){
				h->icount--;
				if(h->icount==0){
					delete h->obj;
					delete h;
				}
			}
			h = a.h;
			if(h) h->icount++;
		}
		//�����, ���û������Object���������, ���󽫱��ͷ�.
		void Null(){
			if(!h) return;
			h->icount--;
			if(h->icount==0){
				delete h->obj;
				delete h;
			}
			h = 0;
		}
		//�Ƿ��Ѿ���һ������.
		inline bool IsNull(){
			return h==NULL;
		}
		//���ð󶨵Ķ���, ���ܹ��ð󶨵Ķ����Զ���¶���ⲿ.
		inline T* operator -> (){
			return h->obj;
		}
		//��ʾ���ذ󶨵Ķ���.
		inline T* Handle(){
			return h->obj;
		}
		inline void Zero(){
			if(h) ::ZeroMemory(h->obj,sizeof(*h->obj));
		}
		inline DWORD Size(){
			return sizeof(T);
		}
		inline ~Object(){
			Null();
		}
	};
	//������ຬ��һ���ض����͵ľ��.
	template<typename T> class CONCISETL_API HandleType : public _class{
	public:
		//��ʼ��Ϊ 0 .
		HandleType():_Handle(0){
			_ASSERT(sizeof(T)==sizeof(void*));
		}		
		//ֱ��ת��Ϊ��Ӧ������.
		inline operator T (){
			return _Handle;
		}	
		//��ָ��ķ�ʽ���ýṹ�ĳ�Ա��һ����˵HandleType����ָ��.
		/*��������VAssistX�Ĳ����ݣ�
		inline T operator -> (){
		return _Handle;
		}*/
		//��ȷ��ȡ���.
		inline T Handle(){
			return _Handle;
		}	
		//�Ƿ�Ϊ��(0),��Щ���Ͳ�֧���������,���Բ�Ҫ��ĳЩ�����ϵ����������,��������һ������.
		inline bool IsNull(){
			return _Handle==0;
		}		
		//�󶨵�����������û����д������������ܱ�֤��Դ����ȷ�ͷ�
		inline T Attach(T handle){
			T h = _Handle;
			_Handle = handle;
			return h;
		}
		//�Ѿ�����������İ�װ�����û����д������������ܱ�֤��Դ����ȷ�ͷ�
		inline T Detach(){
			T handle = _Handle;
			_Handle = 0;
			return handle;
		}

		T _Handle;
	};
#define AutoStruct(T,v) T v;::ZeroMemory(&v,sizeof(T))
#define AutoStruct2(T,v) T v;::ZeroMemory(&v,sizeof(T));v.cbSize = sizeof(v)
	//��Ҫ����Windows API���õ�һ��ṹ,�ص�����4�ֽ���Ϊ�ṹ�Ĵ�С(һ�����cbSize��Ա).
	//���������ǰѽṹ��0��ʼ��,Ȼ������cbSize��ԱΪ�ṹ�Ĵ�С.��API�������൱�ظ��Ĺ���.
	/*���������MFC����Ī�������⣬������ʶ���С����4�ֽڣ����¸�д���ⲿ�����ݡ�
	template<typename T,bool size = false> struct CONCISETL_API AutoStruct : public T,public _class{
		AutoStruct(){
			RtlZeroMemory(this,sizeof(T));
			if(size) *((size_t*)this) = sizeof(T);
		}
		void Zero(){
			RtlZeroMemory(this,sizeof(T));
		}
		operator T& (){
			return *((T*)this);
		}
		void operator = (T& t){
			*((T*)this) = t;
		}
	};*/
	//�»��߿�ͷ�ĺ�������, һ���ǿ�Ŀ�����Աʹ��, ���ʹ���߲���ʹ����Щ����.
	CONCISE_API void* _CProcedure(void* p = 0);
	CONCISE_API void _InitCProcdure(void* obj,void* func,void* pointer,int n = 0);
	//�������ʵ������ _InitCProcdure obj = NULL ���������Ρ�
	CONCISE_API void _InitStdProcdure(void* func,void* pointer,int n = 0);
	//_StdClassProc���Կ�������ָ������,���ǿ��Դ����Ա����,���Ҳ�����й����ϸ�����ͼ��,��ʵ��,���������κΰ�ȫ
	//���,����԰��κ����͵ĺ������ݸ���.����C++���﷨,û��һ���򵥿��еķ�ʽ�������ͼ��.
	class CONCISE_API _StdClassProc : public HandleType<void*>{
	public:
		_StdClassProc(){};
		_StdClassProc(_StdClassProc& scp){
			*this = scp;
		}
		~_StdClassProc();
		//�ж�һ����Ա�����Ƿ��Ѿ������ʵ��,���ھ�̬����,obj������NULLֵ.
		template<typename T> bool HasBind(void* obj,T func){
			if(obj!=GetObject()) return false;
			return SuperCast<void*,T>(func)==GetFunction();
		}
		//����ָ���Ӧ��ʵ��ָ��.
		void* GetObject();
		//����ָ���Ӧ�ĺ���ָ��.
		void* GetFunction();
		//�ж�����ʵ���Ƿ��Ӧ��ͬ�ĵ���(�����Ͼ����ж�GetObject()��GetFunction()���ص�ֵ�Ƿ���ͬ).
		bool operator == (_StdClassProc& scp);
		//��һ�����ø�ֵ����һ������.
		void operator = (_StdClassProc& scp);
		//���ص���ָ��,��������__stdcall���͵Ļص�����.
		template<typename P> inline P Procedure(){
			if(_Handle==0)//���ɻָ�����
				CONASSERT(L"����һ���յĺ�������,���򽫱���");
			return (P)(INT_PTR)_Handle;
		}
		//�Ƿ���һ���պ��������Ա����ã�����û�а��κκ�������IsNull ���� true�����󲻿ɵ��á�
		bool IsEmpty();
		//�ͷŶ������Դ��IsNull���� true ��
		void Release();
		//�󶨵���һ����̬������__stdcall��
		bool IsStdcall();
		//�󶨵���һ����ĳ�Ա��������__thiscall��
		bool IsThiscall();
		//��һ����Ա����,obj��ʵ��ָ��,func����ĳ�Ա����.
		template<typename T>void Bind(void* obj,T func,int n = 0){
			if(_Handle==0) _Handle = _CProcedure();
			if(!_Handle) return;
			_InitCProcdure(obj,SuperCast<void*>(func),_Handle,n);
		}
		//��һ����̬����,func�Ǻ���.
		template<typename T>void BindStd(T func,int n = 0){
			if(_Handle==0) _Handle = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),_Handle,n);
		}
	};
	/**	{
	//	"type":"class",
	//	"name":"APIHook",
	//	"text":"APIHook �������õ�ǰ���̵� API �������ع��ܣ�Ҳ���ǵ�ǰ���̶����ص� API �����ĺ���ȫ��ָ���������õĺ�����hook�������������һ�������ͷ���ֵ��ԭ API ����ͬ�ĺ�����",
	//	"remark":[
	//		"APIHook ���������������������ָ�� API �ĵ���"
	//	]**/
	class CONCISE_API APIHook : public _class{
	protected:
		uchar* _orgFunc;
	public:
		APIHook();
		~APIHook();
		/**{
		//	"type":"function",
		//	"name":"SetHook(void* api,void* hook,int pSize)"
		//	"text":"����һ�� API �����ء�",
		//	"param":[
		//		{
		//			"type":"void*",
		//			"name":"api",
		//			"text":"Ҫ���ص� api����Ҫ�� GetProcAddress ����ȡ������ֱ�����ú���������Ϊ������ʵ��������ת��ַ������ API ��������ַ"
		//		},
		//		{
		//			"type":"void*",
		//			"name":"hook",
		//			"text":"���غ�ָ��ĺ�����ַ��Ҳ���������Լ��ĺ�����"
		//		},
		//		{
		//			"type":"void*",
		//			"name":"pSize",
		//			"text":"API �����Ĳ������ȣ����ֵ�����в����Ĵ�С�ܺ͡�ע�⣺��ʹ�� bool��uchar �����Ĳ�����ʵ���ϱ�����Ҳ�ǰ� 4 �ֽڶ���ģ�Ҳ�����κβ����Ĵ�С���� 4 �ı�����"
		//		}
		//	],
		//	"return":{
		//		"type":"bool",
		//		"text":"����ֵ��ʾ�����Ƿ�ɹ�"
		//	}
		}**/
		bool SetHook(void* api,void* hook,int pSize);
		/**,{
		//	"type":"function",
		//	"name":"UnHook()",
		//	"text":"��ԭ���ص� API ��",
		//	"return":{
		//		"type":"bool",
		//		"text":"����ֵ��ʾ�����Ƿ�ɹ�"
		//	}
		}**/
		bool UnHook();
		/**,{
		//	"type":"function",
		//	"name":"OrgFunc()",
		//	"text":"��ȡԭ���� API ��������������൱���ƹ����أ����������Լ��ĺ�������Ҫ�������������",
		//	"return":{
		//		"type":"void*",
		//		"text":"���ֵ��һ��������ַ�����û�������κκ��������ֵ���� NULL ��"
		//	}
		}**/
		inline const void* OrgFunc(){
			return _orgFunc;
		}
	};
	/**}**/
}
