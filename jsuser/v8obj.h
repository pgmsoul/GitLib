#pragma once
/**	C++编程基本知识
C++的核心思想是对象，方法是绑定到对象上的，对对象的操作就是对成员函数的调用。操作系统的很
多系统资源是通过 C 的方式而不是 C++ 的方式来调用的。也就是通过函数，返回一个句柄，通过对应的
函数对这个句柄进行操作。JavaScript 的语法基本上是对象的方式，但是 JavaScript 只有构造函数，没
有析构函数的概念，因为 JavaScript 压根就没有用户主动销毁对象的机制，所以不能完全照搬 C++ 的方
式。但是另一方面，析构函数往往用来做业务流程的清理工作，而业务清理是一个与语言无关的事务，自
动化清理的机制是非常必要的。但是 JavaScript 的自动清理对象导致的后果就是，用户必须手动处理业务
的收尾工作，因为 JavaScript 没有局部变量的概念，函数执行完后无法像 C++ 那样自动调用局部对象的
析构函数来完成清理工作。
为了弥补 JavaScript 语法层面的这个缺陷，我们设计了 Class 函数，
**/
namespace v8{
	template<typename R> class JsObject{
	public:
		static Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			R::on_create(self,args);
			return store.Close(self);
		}
		static void on_create(Handle<Object>& self,const Arguments& args){}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			cs::String name;
			GetString(property,name);
			R::set(name,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,self));
		}
		static inline void set(cs::String& name,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static void Load(Handle<Object>& glb,LPCWSTR name,DWORD tid = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);

			R::init(ft);

			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
	//返回绑定的C++对象制作, 如果已经 dispose 过, 这个指针为 0 .
	template<typename T> inline bool GetCHandle(T*& handle,Handle<Object>& self){
		handle = (T*)getInternelPointer(self,0);
		return handle!=NULL;
	}
	template<typename T> inline void SetCHandle(T* handle,Handle<Object>& self){
		setInternelPointer(self,0,handle);
	}
	//C++环境调用
	template<typename T> bool Dispose(Handle<Object>& self){
		T* cobj = (T*)getInternelPointer(self,0);
		if(!cobj) return false;
		setInternelPointer(self,0,0);
		Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
		if(_self){
			setInternelPointer(self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
			delete cobj;
		}
		return true;
	}
	//解除这个对象绑定的C++对象，和dispose不同的是，这个函数不销毁C++对象，而是返回它，JavaScript对象被置空。
	template<typename T> T* Detach(Handle<Object>& self){
		T* cobj = (T*)getInternelPointer(self,0);
		setInternelPointer(self,0,0);
		Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
		if(_self){
			setInternelPointer(self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
		}
		return cobj;
	}
	/*@class CObject
	@text 除非特别指明, 凡是C开头的类(构造函数)都绑定了一个内部 C++ 对象, CObject是它们的基类。
	@text 所有CObject继承的对象 new 的时候都可以带一个Boolean类型参数, 用来指明是否生成这个C++对象, 
	缺省和 false 时，创建JavaScript对象的时候也会生成对应的C++对象。如果这个参数设为 true, 
	将不会生成内部的C++对象, 而是绑定一个已存在的C++对象. 
	@text 带参数的new CObject一般只由库内部来调用, 如果用户在JavaScript代码里使用 new CObject(true)生成了一
	个对象, 这个对象会永远是一个空对象(isDisposed()返回true).
	@text 一个CObject对象，通过执行 dispose 来释放它绑定的资源，一旦 dispose 被调用，此对象就
	成为一个普通的JavaScript对象，内部的C++对象无法被再次赋值。
	@text 可以通过isOwned()函数来判断是那种类型的CObject对象, 此函数取回的值和生成时的参数相同, 是一个bool类型.
	@text 一个CObject实际上是在堆栈管理之下的，JavaScript函数的堆栈是通过Auto声明函数来实现的。一个
	CObject类对象如果在一个Auto声明的函数里生成（通过调用 new ），那么函数结束后，此对象会被自动
	dispose。但是绑定类型的对象不受此限制，实际上一个绑定类型的CObject对象和一个普通的JavaScript对象
	没有任何区别。*/
	/*@range_begin*/
	template<typename T,typename R> class JsCObject{
	public:
		//如果这个函数被呼叫，对象一定是一个owned对象，而且一定还没有被 dispose。
		static void weakCallback (Persistent<Value> value, void *data) {
			Persistent<Object>* _self = (Persistent<Object>*)data;
			T* cobj = (T*)getInternelPointer(*_self,0);
			setInternelPointer(*_self,0,0);
			setInternelPointer(*_self,1,0);
			_self->ClearWeak();
			_self->Dispose();
			_self->Clear();
			delete _self;
			delete cobj;
		}
		//类的构造函数。
		static v8::Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			if(self==GetGlobal()){
				//不允许直接调用构造函数，而必须使用 new 生成一个对象。
				ThrowException(String::New("use new to create object, do not call constructor direct."));
				return Undefined();
			}
			//如果第一个参数是 true，生成空对象。 
			/*if(args.Length()>0&&args[0]->IsTrue()){
				setInternelPointer(self,0,0);
				setInternelPointer(self,1,0);
				return store.Close(self);
			}*/
			T* cobj;
			//如果第一个参数是一个整数，表示一个对象指针，不再重新生成C++对象。
			if(args.Length()>0&&args[0]->IsUint32()){
				uint val = args[0]->Uint32Value();
				if(!(0xFFFF0000&val)&&val!=0){
					ThrowException(String::New("bind pointer is not a valid pointer, high word is zero."));
					return Undefined();
				}
				cobj = (T*)(UINT_PTR)val;
				setInternelPointer(self,0,cobj);
				if((args.Length()<=1||args[1]->IsFalse())&&cobj!=0){
					Persistent<Object>* _self = new Persistent<Object>();
					*_self = Persistent<Object>::New(self);
					setInternelPointer(self,1,_self);
					_self->MakeWeak(_self, weakCallback);
					_self->MarkIndependent();
				}else{
					//当第一个参数是一个指针，第二个参数是 true 的时候，生成一个包装类型的对象，这类对象不销毁内部cpp对象。
					//指针自己不知道是否已经被包装，一个指针一般只能被包装一次，除非是简单的包装，对象本身没有特殊处理。
					setInternelPointer(self,1,0);
				}
			}else{//其它情况，参数无效，生成新的C++对象。
				cobj = new T();
				Persistent<Object>* _self = new Persistent<Object>();
				*_self = Persistent<Object>::New(self);
				setInternelPointer(self,0,cobj);
				setInternelPointer(self,1,_self);
				_self->MakeWeak(_self, weakCallback);
				_self->MarkIndependent();
			}
			if(cobj!=0){
				Handle<Function> push = GetEnv()->GetFunc(FUNC_ID_STACK_PUSH);
				Handle<Value> argv[1];
				argv[0] = self;
				CallFunc(GetGlobal(),push,1,argv);
				R::on_create(self,cobj);
			}

			return store.Close(self);
		}
		/*@function void dispose()
		@text 销毁一个CObjec对象的内部C++对象, 一旦执行了这个函数, 这个CObject对象将会变成一个普通的
		JavaScript对象, 从而不能使用它的核心功能. 一般只有丢弃它的时候才需要调用这个函数.
		@text dispose函数的调用是非常重要的, 对于一个不在堆栈管理下的CObject对象, JavaScript引擎无法
		知道这些C++对象使用了多少系统资源, 因为JavaScript的垃圾收集器是不确定何时运作的, 如果创建了大
		量的CObject对象, 系统资源很可能会迅速耗尽, 比如GDI资源是非常有限的, 它会在远远不能触发垃圾收集器
		的情况下被耗尽, 所以必须及时的手动清理这些资源. 配合Auto函数的堆栈管理, 可以保证CObject对象
		在函数退出后, 被自动调用dispose, 从而省去手动清理的麻烦. 但是如果一个函数内部在运行期使用了大量的
		系统资源, 必须手动调用dispose及时清理不用的资源.*/
		static Handle<Value> dispose(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();

			T* cobj = (T*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			if(cobj){
				setInternelPointer(self,0,0);
			}

			Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
			if(_self){
				setInternelPointer(self,1,0);
				//如果cobj有回调函数，并且里面保存了 _self 此处无法通知 cobj 设置 _self 无效，所以先销毁 cobj，再清理 _self 。
				delete cobj;//非绑定型
				if(!_self->IsEmpty()){
					_self->ClearWeak();
					_self->Dispose();
					_self->Clear();
				}
				delete _self;
			}
			return True();
		}
		/*@function string toString()
		@text 取得对象的名称
		@return string 返回一个代表对象名称的字串.*/

		/*@function bool isDisposed()
		@text 一个CObject对象的内部C++指针是否为空。一个CObject对象一旦调用了 dispose
		它内部的C++对象会被销毁或释放。
		@return bool 指示CObject对象内部的C++对象是否有效*/
		static Handle<Value> isDisposed(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* cobj;
			return Bool(!GetCHandle<T>(cobj,self));
		}
		/*@function bool isOwned()
		@text 判断一个CObject对象是否拥有它绑定的内部C++指针, 它的意义是对象调用dispose
		的时候，是否删除这个指针。
		@return 如果此函数返回 true，表示它负责自己管理内部C++对象资源，执行dispose
		函数时，C++对象被回收，绑定被置为空；如果返回false，表示它不负责管理它绑
		定的C++资源，执行 dispose 函数，仅仅是把内部绑定的C++对象置为空，这就是说
		那个C++资源实际上是被别的JavaScript对象管理的。如果返回 undefined，表示此
		对象已经被 dispose 过了，不管是哪种类型。*/
		static Handle<Value> isOwned(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			return Bool(0!=getInternelPointer(self,1));
		}
		/*@function integer obj()
		@text 获取内部C++对象的句柄。
		@return integer 返回内部对象的指针, 如果对象已经 dispose，返回 0 .*/
		static Handle<Value> obj(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* cobj;
			GetCHandle(cobj,self);
			return store.Close(Uint32::New((UINT_PTR)cobj));
		}
		/*@function bool attach(handle)
		@text 把一个资源句柄绑定到这个对象上, 从而可以使用这个对象的方法对它进行操作.
		@param handle -- integer
		@text 要绑定的句柄, 每种对象对应的句柄必须正确.
		@return bool 指示操作是否成功*/
		static Handle<Value> bind(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* cobj;
				if(GetCHandle(cobj,self)){
					R::on_dispose(self,cobj);
					delete cobj;
				}
				cobj = GET_ARGS_INT2(0,0,T*);
				SetCHandle(cobj,self);

				if(GET_ARGS_VALUE(1,false,Boolean)||cobj==0){
					Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
					if(_self){
						setInternelPointer(self,1,0);
						if(!_self->IsEmpty()){
							_self->ClearWeak();
							_self->Dispose();
							_self->Clear();
						}
						delete _self;
					}
				}
				return True();
			}
			return Undefined();
		}
		/*@function integer detach()
		@text 把一个对象的句柄和这个对象解除绑定.
		@return integer 返回资源句柄, 如果这是一个系统资源, 需要手动处理它.*/
		static Handle<Value> debind(const Arguments& args){
			HandleScope store;
			while(true){
				Local<Object> self = args.This();
				T* cobj = (T*)getInternelPointer(self,0);
				if(cobj){
					setInternelPointer(self,0,0);
				}

				Persistent<Object>* _self = (Persistent<Object>*)getInternelPointer(self,1);
				if(_self){
					setInternelPointer(self,1,0);
					if(!_self->IsEmpty()){
						_self->ClearWeak();
						_self->Dispose();
						_self->Clear();
					}
					delete _self;
				}
				return store.Close(Uint32::New((UINT_PTR)cobj));
			}
			return Undefined();
		}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			T* cobj;
			if(!GetCHandle(cobj,self)) return;
			cs::String name;
			GetString(property,name);
			R::set(name,cobj,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			T* cobj;
			if(!GetCHandle(cobj,self)) return Undefined();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,cobj,self));
		}
		static inline void set(cs::String& name,T* cobj,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,T* cobj,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static inline void on_create(Handle<Object>& self,T* cobj){}
		static inline void on_dispose(Handle<Object>& self,T* cobj){}
		//初始化这个对象.
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//不要在inst上添加属性，否则，初始化一个对象（new）的时候，内部会在调用Create之前，
			//调用属性设置方法。
			inst->SetInternalFieldCount(2);

			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(isOwned);
			SET_CLA_FUNC(obj);
			SET_CLA_FUNC(debind);
			SET_CLA_FUNC(bind);

			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
	/*@range_end*/
	/*@class CHandleObject
	@text 这个类继承自CObject, 它绑定的C++有一个共同点，就是C++对象都含有一个内部资源指针。
	添加了四个函数：
	@text isNull()：内部资源指针是否为空。
	@text handle()：内部资源指针句柄。
	@text attach(handle)：绑定一个资源指针。
	@text detach()：解除它绑定的资源指针的。
	@text 如果一个CHandleObject对象的内部C++对象为空，上述函数全部返回undefined。*/
	/*@range_begin*/
	template<typename T,typename H,typename R> class JsHandleObject : public JsCObject<T,R>{
	public:
		/*@function bool isNull()
		@text 资源句柄是一个整数，一个空的资源句柄一般是 0 ，但是并不总是如此，比如有的时候是-1等等，所以需要用这个函数来判断句柄是否有效。
		@return bool 指示内部C++对象绑定的资源句柄是否有效，如果C++对象本身为空，返回 undefined。*/
		static Handle<Value> isNull(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* handle;
			if(!GetCHandle(handle,self)){
				return Undefined();
			}
			return Bool(handle->IsNull());
		}
		/*@function integer handle()
		@return integer 内部对象的句柄*/
		static Handle<Value> handle(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			T* handle;
			if(!GetCHandle(handle,self)){
				return Undefined();
			}
			return store.Close(Uint32::New((UINT_PTR)handle->Handle()));
		}
		/*@function bool attach(handle)
		@text 把一个资源句柄绑定到这个对象上, 从而可以使用这个对象的方法对它进行操作.
		@param handle -- integer
		@text 要绑定的句柄, 每种对象对应的句柄必须正确.
		@return bool 指示操作是否成功*/
		static Handle<Value> attach(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* handle;
				if(!GetCHandle(handle,self)) break;
				handle->Attach((H)(UINT_PTR)args[0]->Uint32Value());
				return True();
			}
			return Undefined();
		}
		/*@function integer detach()
		@text 把一个对象的句柄和这个对象解除绑定.
		@return integer 返回资源句柄, 如果这是一个系统资源, 需要手动处理它.*/
		static Handle<Value> detach(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1||!args[0]->IsUint32()) break;
				Local<Object> self = args.This();
				T* handle;
				if(!GetCHandle(handle,self)) break;
				UINT_PTR h = (UINT_PTR)handle->Detach();
				return store.Close(Uint32::New(h));
			}
			return Undefined();
		}
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(isOwned);
			SET_CLA_FUNC(obj);
			SET_CLA_FUNC(debind);
			SET_CLA_FUNC(bind);
			SET_CLA_FUNC(handle);
			SET_CLA_FUNC(isNull);
			SET_CLA_FUNC(attach);
			SET_CLA_FUNC(detach);

			inst->SetInternalFieldCount(2);
			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			GetEnv()->SetTemplate(ft,tid);
		}
	};
	/*@range_end*/
	//JsWrapObject 不能继承，它只能包装单独的类。
	template<typename T,typename R> class JsWrapObject{
	public:
		class WrapObject : public T{
		public:
			Persistent<Object> self;
			~WrapObject(){
				if(self.IsEmpty()) return;
				setInternelPointer(self,0,0);
				self.ClearWeak();
				self.Dispose();
				self.Clear();
			}
		};
		static void weakCallback (Persistent<Value> value, void *data) {
			delete (WrapObject*)data;
		}
		static v8::Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			if(self==GetGlobal()){
				//不允许直接调用构造函数，而必须使用 new 生成一个对象。
				ThrowException(String::New("use new to create object, do not call constructor direct."));
				return Undefined();
			}
			WrapObject* cobj = new WrapObject();
			cobj->self = Persistent<Object>::New(self);
			setInternelPointer(self,0,cobj);
			cobj->self.MakeWeak(cobj,weakCallback);
			cobj->self.MarkIndependent();

			Handle<Function> push = GetEnv()->GetFunc(FUNC_ID_STACK_PUSH);
			Handle<Value> argv[1];
			argv[0] = self;
			CallFunc(GetGlobal(),push,1,argv);
			R::on_create(self,cobj);
			return store.Close(self);
		}
		static Handle<Value> dispose(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();

			WrapObject* cobj = (WrapObject*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			delete cobj;
			return True();
		}
		static void Dispose(Handle<Object>& self){
			WrapObject* cobj = (WrapObject*)getInternelPointer(self,0);
			R::on_dispose(self,cobj);
			delete cobj;
		}
		static Handle<Value> isDisposed(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			WrapObject* cobj;
			return Bool(!GetCHandle(cobj,self));
		}
		static Handle<Value> obj(const Arguments& args){
			HandleScope store;
			Local<Object> self = args.This();
			WrapObject* cobj;
			GetCHandle(cobj,self);
			return store.Close(Uint32::New((UINT_PTR)cobj));
		}
		static void _set(Local<String> property, Local<Value> value,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			WrapObject* cobj;
			if(!GetCHandle(cobj,self)) return;
			cs::String name;
			GetString(property,name);
			R::set(name,cobj,value,self);
		}
		static Handle<Value> _get(Local<String> property,const AccessorInfo& info){
			HandleScope store;
			Local<Object> self = info.This();
			WrapObject* cobj;
			if(!GetCHandle(cobj,self)) return Undefined();
			cs::String name;
			GetString(property,name);
			return store.Close(R::get(name,cobj,self));
		}
		static inline void set(cs::String& name,WrapObject* cobj,Local<Value>& value,Local<Object>& self){}
		static inline Handle<Value> get(cs::String& name,WrapObject* cobj,Local<Object>& self){}
		static inline void init(Handle<FunctionTemplate>& ft){}
		static inline void on_create(Handle<Object>& self,WrapObject* cobj){}
		static inline void on_dispose(Handle<Object>& self,WrapObject* cobj){}
		//初始化这个对象.
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid,Persistent<FunctionTemplate>* inh = 0){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&R::Create);
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//不要在inst上添加属性，否则，初始化一个对象（new）的时候，内部会在调用Create之前，
			//调用属性设置方法。
			inst->SetInternalFieldCount(1);

			R::init(ft);

			func->Set(String::New("dispose"),FunctionTemplate::New(&R::dispose),ReadOnly);
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			func->Set(String::New("toString"),ts,ReadOnly);
			func->Set(String::New("isCObject"),True(),ReadOnly);
			SET_CLA_FUNC(isDisposed);
			SET_CLA_FUNC(obj);

			if(inh)
				ft->Inherit(*inh);
			glb->Set(String::New((uint16_t*)name),ft->GetFunction());
			if(tid!=0)
				GetEnv()->SetTemplate(ft,tid);
		}
	};
}