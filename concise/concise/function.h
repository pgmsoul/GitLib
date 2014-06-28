#pragma once
namespace cs{
	/*@function GetThis(T,var)
	@text 获取静态(__stdcall)函数被 Function 回调的时候，初始化 Function 时设置的 object 指针。
	这个函数本质上一个宏，它嵌入一段汇编代码在函数的开头。<span class="red_bold">注意：
	这个宏必须在函数的开头使用</span>。因为它实际上只有2行，声明变量 var 和一行嵌入式汇编 mov var,edx 。
	慎用这个机制，必须确保 edx 寄存器在之前不被使用，而不同的编译器编译的方式是不同的。
	@text 给静态函数增加 this 指针有什么用？很多时候，在设计一个类的时候，往往它的回调函数参数里没有自己，
	否则，就需要给每一个回调函数都额外增加一个参数来指示呼叫实例。而在某些场合，回调函数又是一个静态函数，
	而非它自己的成员函数，如果这个静态函数被多个实例回调，我们就需要使用这个机制知道哪个实例呼叫了回调函数。
	@param T
	@text 想要声明的对象类型。
	@param var -- T
	@text 声明的变量名称
	@return 无
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
		//声明一个函数对象
		Function<int,int> func;
		//声明一个 A 对象。
		A a;
		//把 a 的地址作为 this 指针传递给静态函数 cbfunc。
		func.BindStd(&cbfunc,&a);
		//呼叫函数，cbfunc将会被回调，而 pa 将会是 a 的地址。
		int n = func(2);
		//n 应该返回 10 .
		return n;
	}
	*/
#define GetThis(T,var) T* var;__asm mov var,edx
	//一个不会被使用的空类型,实际当中你永远不会用到它,这正是它的作用.
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
	//Function类提供函数指针的作用,但是有严格的类型检查,并且,可以把类的成员函数用作回调(__stdcall)函数.与Delegate的区别是,
	//Function只能设置一个函数,而Delegate可以调用多个函数,后者也不能回调.
	//Function的一个重要特性，就是当函数已经执行到 Function 实例指向的函数代码时，实例本身是可以安全删除的，因为代码再也不
	//会回到实例本身了，实例实际上提供的是代码的跳转功能，一旦已经跳转，这部分内存就不再需要了。
	//空函数，一个Function对象是可以不经初始化（Bind一个函数）而调用的，但是这种情况不能使用它的返回值，Function本身只完成跳转功能，
	//空函数不进行任何跳转相当于一个无返回值的空函数调用，所以它这种情况下不等价于一个对应类型的函数。
	template<typename R,typename T1 = NullType,typename T2 = NullType,typename T3 = NullType,typename T4 = NullType,typename T5 = NullType> class CONCISETL_API Function : public _StdClassProc
	{
	public:
		//函数对应的静态函数类型.
		typedef R (__stdcall* PROC)(T1,T2,T3,T4,T5);
		//缺省绑定一个空函数,如果函数有返回值，空函数的作用仅仅是调用它不会出错，但返回值是不可控制的。
		Function(){}
		void Empty(){
			uint dsp = GetAlignment<T1>() + GetAlignment<T2>() + GetAlignment<T3>() + GetAlignment<T4>() + GetAlignment<T5>();
			_StdClassProc::Bind<PROC>(0,0,dsp);
		}
		//用一个类的实例和成员函数来初始化函数.
		template<typename C,typename P> void Bind(C* obj,P func)
		{
			typedef R (C::*CP)(T1,T2,T3,T4,T5);
			_StdClassProc::Bind<CP>(obj,func);
		}
		//用一个静态函数来初始化函数.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		//用一个相同类型的函数来初始化函数.
		void operator = (Function<R,T1,T2,T3,T4,T5>& func)
		{
			_StdClassProc::operator = (func);
		}
		//返回静态等效函数的指针.
		inline PROC Procedure()
		{
			return _StdClassProc::Procedure<PROC>();
		}
		//同样功能的自动类型转换.
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
		//用一个静态函数来初始化函数.
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
		//用一个静态函数来初始化函数.
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
		//用一个静态函数来初始化函数.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		template<typename C,typename P> void BindAny(C* obj,P func){
			_StdClassProc::Bind<P>(obj,func);
		}
		//用一个静态函数来初始化函数.
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
		//绑定任何一个函数，有些场合，虽然函数格式正确，但是以为函数所处位置很复杂，不一定符合 CP 的格式，需要使用这个函数，使用这个函数必须保证格式正确，因为完全没有格式检测。
		template<typename C,typename P> void BindAny(C* obj,P func){
			_StdClassProc::Bind<P>(obj,func);
		}
		//用一个静态函数来初始化函数.
		template<typename P> void BindStd(P func){
			_StdClassProc::BindStd<PROC>(func);
		}
		//绑定任何一个函数，有些场合，虽然函数格式正确，但是以为函数所处位置很复杂，不一定符合 PROC 的格式，需要使用这个函数，使用这个函数必须保证格式正确，因为完全没有格式检测。
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
		//用一个静态函数来初始化函数.
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
	//_delegate是Delegate的基类,因为Delegate并没有增加成员变量,所以大小和_delegate是相同的,如果要声明一个列表(ObjectList)
	//而其成员是不同的Delegate,可是声明为_delegate类型,而在使用的时候再转换为相应类型的Delegate,而不用担内存和析构的问题.
	class CONCISE_API _delegate : public _class
	{
	public:
		//从代表中删除指定索引的调用,如果索引超界返回false.
		bool Delete(uint index);
		//返回指定索引的调用对应的对象指针.索引超界返回0
		void* GetObject(uint index);
		//返回指定索引对应的函数指针.索引超界返回0
		void* GetFunction(uint index);
		//赋值
		void operator = (_delegate& dt);
		//返回绑定的调用数量.
		inline uint Count(){return _Ptrs.Count();}
		~_delegate();
	protected:
		IntegerList<void*> _Ptrs;
	};
	template<typename T1 = NullType,typename T2 = NullType,typename T3 = NullType,typename T4 = NullType,typename T5 = NullType> class CONCISETL_API Delegate : public _delegate
	{
	public:
		typedef void (__stdcall* STDCALLTYPE)(T1,T2,T3,T4,T5);
		//添加一个类的实例和成员函数调用.
		template<typename R,typename P> uint Add(R* object,P proc,uint index = -1)
		{
			typedef void (R::*THISCALLTYPE)(T1,T2,T3,T4,T5);
			void* p = _CProcedure();
			_InitCProcdure(object,SuperCast<void*,THISCALLTYPE>(proc),p);
			return _Ptrs.Add(p,index);
		}
		//添加一个函数调用.函数必须是void返回类型.
		uint Add(Function<void,T1,T2,T3,T4,T5> func,uint index = -1){
			void* p = _CProcedure();
			if(func.IsNull()) return 0;
			_InitCProcdure(func.GetObject(),func.Procedure(),p);
			return _Ptrs.Add(p,index);
		}
		//添加一个静态函数调用.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		//删除指定索引的函数.
		bool Delete(uint index)
		{
			return _delegate::Delete(index);
		}
		//删除特定的函数,如果函数没有在实例中,返回false.
		template<typename R,typename P> bool Delete(R* object,P proc)
		{
			return Delete(IndexOf(object,proc));
		}
		//删除静态函数.
		bool Delete(STDCALLTYPE func)
		{
			return Delete(IndexOf(func));
		}
		//删除一个调用.
		bool Delete(Function<void,T1,T2,T3,T4,T5> func)
		{
			return Delete(IndexOf(func));
		}
		//返回指定调用的索引,如果调用不在代表中,则返回-1;
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
		//调用代表.添加的所有函数都会被呼叫.
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
		//添加一个静态函数调用.
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
		//添加一个静态函数调用.
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
		//添加一个静态函数调用.
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
		//添加一个静态函数调用.
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
		//添加一个静态函数调用.
		uint Add(STDCALLTYPE func,uint index = -1){
			void* p = _CProcedure();
			_InitStdProcdure(SuperCast<void*>(func),p);
			return _Ptrs.Add(p,index);
		}
		//从代表中删除指定索引的调用,如果索引超界返回false.
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