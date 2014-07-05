namespace cs{
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
	//_delegate��Delegate�Ļ���,��ΪDelegate��û�����ӳ�Ա����,���Դ�С��_delegate����ͬ��,���Ҫ����һ���б�(ObjectList)
	//�����Ա�ǲ�ͬ��Delegate,��������Ϊ_delegate����,����ʹ�õ�ʱ����ת��Ϊ��Ӧ���͵�Delegate,�����õ��ڴ������������.
	class CONCISE_API _delegate : public _class
	{
	public:
		//�Ӵ�����ɾ��ָ�������ĵ���,����������緵��false.
		bool Delete(uint index);
		//����ָ�������ĵ��ö�Ӧ�Ķ���ָ��.�������緵��0
		void* GetObject(uint index);
		//����ָ��������Ӧ�ĺ���ָ��.�������緵��0
		void* GetFunction(uint index);
		//��ֵ
		void operator = (_delegate& dt);
		//���ذ󶨵ĵ�������.
		inline uint Count(){return _Ptrs.Count();}
		~_delegate();
	protected:
		IntegerList<void*> _Ptrs;
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
	/*@function GetThis(T,var)
	@text ��ȡ��̬(__stdcall)������ Function �ص���ʱ�򣬳�ʼ�� Function ʱ���õ� object ָ�롣
	�������������һ���꣬��Ƕ��һ�λ������ں����Ŀ�ͷ��<span class="red_bold">ע�⣺
	���������ں����Ŀ�ͷʹ��</span>����Ϊ��ʵ����ֻ��2�У��������� var ��һ��Ƕ��ʽ��� mov var,edx ��
	����������ƣ�����ȷ�� edx �Ĵ�����֮ǰ����ʹ�ã�����ͬ�ı���������ķ�ʽ�ǲ�ͬ�ġ�
	@text ����̬�������� this ָ����ʲô�ã��ܶ�ʱ�������һ�����ʱ���������Ļص�����������û���Լ���
	���򣬾���Ҫ��ÿһ���ص���������������һ��������ָʾ����ʵ��������ĳЩ���ϣ��ص���������һ����̬������
	�������Լ��ĳ�Ա��������������̬���������ʵ���ص������Ǿ���Ҫʹ���������֪���ĸ�ʵ�������˻ص�������
	@param T
	@text ��Ҫ�����Ķ������͡�
	@param var -- T
	@text �����ı�������
	@return ��
	@code 
	class A{
	public:
	int call(int i){
	return i+3;
	}
	};
	int __stdcall cbfunc(int i){
	GetThis(A,pa);
	return 2*pa->call(i);
	}
	int main(){
	//����һ����������
	Function<int,int> func;
	//����һ�� A ����
	A a;
	//�� a �ĵ�ַ��Ϊ this ָ�봫�ݸ���̬���� cbfunc��
	func.BindStd(&cbfunc,&a);
	//���к�����cbfunc���ᱻ�ص����� pa ������ a �ĵ�ַ��
	int n = func(2);
	//n Ӧ�÷��� 10 .
	return n;
	}
	*/
#define GetThis(T,var) T* var;__asm mov var,edx
	//һ�����ᱻʹ�õĿ�����,ʵ�ʵ�������Զ�����õ���,��������������.
	CONCISE_API void* GetNullProc(uint n);
	class CONCISE_API NullType{private:NullType(){}};
	template<typename T> struct CONCISETL_API is_reference{
		enum{value = false};
	};
	template<typename T> struct CONCISETL_API is_reference<T&>{
		enum{value = true};
	};
	template<typename T> uint GetAlignment(){
		if(is_reference<T>::value) return 4;
		else{
			size_t dsp = sizeof(T);
			if(dsp%4){
				dsp /= 4;
				dsp += 1;
				dsp *= 4;
			}
			return (uint)dsp;
		}
	}
	//Function���ṩ����ָ�������,�������ϸ�����ͼ��,����,���԰���ĳ�Ա���������ص�(__stdcall)����.��Delegate��������,
	//Functionֻ������һ������,��Delegate���Ե��ö������,����Ҳ���ܻص�.
	//Function��һ����Ҫ���ԣ����ǵ������Ѿ�ִ�е� Function ʵ��ָ��ĺ�������ʱ��ʵ�������ǿ��԰�ȫɾ���ģ���Ϊ������Ҳ��
	//��ص�ʵ�������ˣ�ʵ��ʵ�����ṩ���Ǵ������ת���ܣ�һ���Ѿ���ת���ⲿ���ڴ�Ͳ�����Ҫ�ˡ�
	//�պ�����һ��Function�����ǿ��Բ�����ʼ����Bindһ�������������õģ����������������ʹ�����ķ���ֵ��Function����ֻ�����ת���ܣ�
	//�պ����������κ���ת�൱��һ���޷���ֵ�Ŀպ������ã���������������²��ȼ���һ����Ӧ���͵ĺ�����
}