#pragma once
namespace cs{
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
	template<typename R,typename T1 = NullType,typename T2 = NullType,typename T3 = NullType,typename T4 = NullType,typename T5 = NullType> class CONCISETL_API Function : public _StdClassProc
	{
	public:
		//������Ӧ�ľ�̬��������.
		typedef R (__stdcall* PROC)(T1,T2,T3,T4,T5);
		//ȱʡ��һ���պ���,��������з���ֵ���պ��������ý����ǵ������������������ֵ�ǲ��ɿ��Ƶġ�
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>() + GetAlignment<T2>() + GetAlignment<T3>() + GetAlignment<T4>() + GetAlignment<T5>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		//��һ�����ʵ���ͳ�Ա��������ʼ������.
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)(T1,T2,T3,T4,T5);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		//��һ����ͬ���͵ĺ�������ʼ������.
		void operator = (Function<R,T1,T2,T3,T4,T5>& func)
		{
			_StdClassProc::operator = (func);
		}
		//���ؾ�̬��Ч������ָ��.
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		//ͬ�����ܵ��Զ�����ת��.
		operator PROC ()
		{
			return _StdClassProc::Procedure<PROC>();
		}
	};
	template<typename R,typename T1,typename T2,typename T3,typename T4> class CONCISETL_API Function<R,T1,T2,T3,T4> : public _StdClassProc
	{
	public:
		typedef R (__stdcall* PROC)(T1,T2,T3,T4);
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>() + GetAlignment<T2>() + GetAlignment<T3>() + GetAlignment<T4>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)(T1,T2,T3,T4);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		void operator = (Function<R,T1,T2,T3,T4>& func)
		{
			_StdClassProc::operator = (func);
		}
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		operator PROC ()
		{
			return _StdClassProc::Procedure<PROC>();
		}
	};
	template<typename R,typename T1,typename T2,typename T3> class Function<R,T1,T2,T3> : public _StdClassProc
	{
	public:
		typedef R (__stdcall* PROC)(T1,T2,T3);
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>() + GetAlignment<T2>() + GetAlignment<T3>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)(T1,T2,T3);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		void operator = (Function<R,T1,T2,T3>& func)
		{
			_StdClassProc::operator = (func);
		}
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		operator PROC ()
		{
			return _StdClassProc::Procedure<PROC>();
		}
	};
	template<typename R,typename T1,typename T2> class CONCISETL_API Function<R,T1,T2> : public _StdClassProc
	{
	public:
		typedef R (__stdcall* PROC)(T1,T2);
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>() + GetAlignment<T2>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)(T1,T2);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		template<typename C,typename P> void BindAny(C* obj,P func){
			_StdClassProc::Bind<P>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStdAny(P func){
			_StdClassProc::BindStd<P>(func);
		}
		void operator = (Function<R,T1,T2>& func)
		{
			_StdClassProc::operator = (func);
		}
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		operator PROC ()
		{
			return _StdClassProc::Procedure<PROC>();
		}
	};
	template<typename R,typename T1> class CONCISETL_API Function<R,T1> : public _StdClassProc
	{
	public:
		typedef R (__stdcall* PROC)(T1);
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		template<typename C,typename P> void Bind(C* obj,P func){
			typedef R (C::*CP)(T1);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//���κ�һ����������Щ���ϣ���Ȼ������ʽ��ȷ��������Ϊ��������λ�úܸ��ӣ���һ������ CP �ĸ�ʽ����Ҫʹ�����������ʹ������������뱣֤��ʽ��ȷ����Ϊ��ȫû�и�ʽ��⡣
		template<typename C,typename P> void BindAny(C* obj,P func){
			_StdClassProc::Bind<P>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		//���κ�һ����������Щ���ϣ���Ȼ������ʽ��ȷ��������Ϊ��������λ�úܸ��ӣ���һ������ PROC �ĸ�ʽ����Ҫʹ�����������ʹ������������뱣֤��ʽ��ȷ����Ϊ��ȫû�и�ʽ��⡣
		template<typename P> void BindStdAny(P func){
			_StdClassProc::BindStd<P>(func);
		}
		void operator = (Function<R,T1>& func){
			_StdClassProc::operator = (func);
		}
		inline PROC Procedure(){
			return _StdClassProc::Procedure<PROC>();
		}
		operator PROC (){
			return _StdClassProc::Procedure<PROC>();
		}
	};
	template<typename R> class CONCISETL_API Function<R> : public _StdClassProc
	{
	public:
		typedef R (__stdcall* PROC)();
		Function(){}
		void Empty(){
			_StdClassProc::Bind<PROC>(0,0,0);
		}
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)();
			_StdClassProc::Bind<CP>(obj,func);
		}
		//��һ����̬��������ʼ������.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		void operator = (Function<R>& func)
		{
			_StdClassProc::operator = (func);
		}
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		operator PROC ()
		{
			return _StdClassProc::Procedure<PROC>();
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
	template<typename T1 = NullType,typename T2 = NullType,typename T3 = NullType,typename T4 = NullType,typename T5 = NullType> class CONCISETL_API Delegate : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1,T2,T3,T4,T5);
		//���һ�����ʵ���ͳ�Ա��������.
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3,T4,T5);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����������.����������void��������.
		uint Add(Function<void,T1,T2,T3,T4,T5> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		//ɾ��ָ�������ĺ���.
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		//ɾ���ض��ĺ���,�������û����ʵ����,����false.
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		//ɾ����̬����.
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		//ɾ��һ������.
		bool Delete(Function<void,T1,T2,T3,T4,T5> func)
		{
			return Delete(IndexOf(func));
		}
		//����ָ�����õ�����,������ò��ڴ�����,�򷵻�-1;
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3,T4,T5);
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(SuperCast<void*>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void,T1,T2,T3,T4,T5> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.Procedure()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		//���ô���.��ӵ����к������ᱻ����.
		void Call(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])(t1,t2,t3,t4,t5);
			}
		}
	};
	template<typename T1,typename T2,typename T3,typename T4> class CONCISETL_API Delegate<T1,T2,T3,T4> : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1,T2,T3,T4);
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3,T4);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		uint Add(Function<void,T1,T2,T3,T4> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		bool Delete(Function<void,T1,T2,T3,T4> func)
		{
			return Delete(IndexOf(func));
		}
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3,T4);
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(SuperCast<void*>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void,T1,T2,T3,T4> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.Procedure()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		void Call(T1 t1,T2 t2,T3 t3,T4 t4)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])(t1,t2,t3,t4);
			}
		}
	};
	template<typename T1,typename T2,typename T3> class CONCISETL_API Delegate<T1,T2,T3> : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1,T2,T3);
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		uint Add(Function<void,T1,T2,T3> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		bool Delete(Function<void,T1,T2,T3> func)
		{
			return Delete(IndexOf(func));
		}
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3);
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc){
			for(uint i=0;i<_Ptrs.Count();i++){
				void* p = SuperCast<void*>(proc);
				void* p2 = GetFunction(i);
				if(p!=p2) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void,T1,T2,T3> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.Procedure()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		void Call(T1 t1,T2 t2,T3 t3)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])(t1,t2,t3);
			}
		}
	};
	template<typename T1,typename T2> class CONCISETL_API Delegate<T1,T2> : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1,T2);
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		uint Add(Function<void,T1,T2> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		bool Delete(Function<void,T1,T2> func)
		{
			return Delete(IndexOf(func));
		}
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2);
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(SuperCast<void*>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void,T1,T2> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.Procedure()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		void Call(T1 t1,T2 t2)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])(t1,t2);
			}
		}
	};
	template<typename T1> class CONCISETL_API Delegate<T1> : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1);
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		uint Add(Function<void,T1> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		bool Delete(Function<void,T1> func)
		{
			return Delete(IndexOf(func));
		}
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)(T1);
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(SuperCast<void*>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void,T1> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.Procedure()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		void Call(T1 t1)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])(t1);
			}
		}
	};
	template<> class CONCISETL_API Delegate<> : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)();
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)();
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		uint Add(Function<void> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//���һ����̬��������.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		//�Ӵ�����ɾ��ָ�������ĵ���,����������緵��false.
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		bool Delete(STDCALLTYPE func)
		{
			return _delegate::Delete(IndexOf(func));
		}
		bool Delete(Function<void> func)
		{
			return _delegate::Delete(IndexOf(func));
		}
		template<typename R,typename P> uint IndexOf(R* object,P proc)
		{
			typedef void (R::*THISCALLTYPE)();
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if((void*)object!=GetObject(i)) continue;
				if(SuperCast<void*,THISCALLTYPE>(proc)!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(STDCALLTYPE proc)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(SuperCast<void*>(proc)!=this->GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		uint IndexOf(Function<void> func)
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				if(func.GetObject()!=GetObject(i)) continue;
				if(func.GetFunction()!=GetFunction(i)) continue;
				return i;
			}
			return -1;
		}
		void Call()
		{
			for(uint i=0;i<_Ptrs.Count();i++)
			{
				((STDCALLTYPE)_Ptrs[i])();
			}
		}
	};
}