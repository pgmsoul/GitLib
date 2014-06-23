#pragma once
namespace cs{
	//�����ڴ棬��Ϊ����MapViewOfFile������ڴ棬���ܲ�������ĳЩ API ����������
	//SetWindowText�ȣ��᷵�ؾ����Ч������Ҫʹ��ϵͳ�ڴ档������������ĳ���������ġ�
	//�ڴ����,������ṩ�ڴ�ķ�����ͷŲ���,�����в����ĵ�λ����Ӧ���͵�����.
	//���������ĳ��Ȳ��ܴ�����������������ʹ�С����Ϊ������������ֵ����Խ�硣
	template<typename T> class CONCISETL_API Memory : public HandleType<T*>{
	protected:
		uint _Size;
		bool _sys,_bind;
	public:
		Memory():_Size(0),_sys(0),_bind(0){}	//���캯��
		//���캯��,length�ǳ�ʼ�ռ�Ĵ�С,ע��,�ڴ�ռ�Ĵ�С�����ֵ����Ԫ�صĴ�С.
		Memory(bool sys):_Size(0),_bind(0){
			_sys = sys;
		}
		~Memory(){			//��������
			if(_Handle&&!_bind) MemoryFree(_Handle,_sys);
		}
		//����ʹ�õ��ڴ���ϵͳ�ڴ滹�Ǹ����ڴ�, Ĭ���Ǹ����ڴ�. ������ʵ��������ڴ�Ϊ�յ�״̬��, ��������.
		//��Ϊ�����ڴ��С����, �����������ڴ�, ���Ҳ���Ƶ���ķ�����ͷ�, ������ʹ�ø��ٷ����ڴ�.
		inline void SetUseSysMemory(bool sys = 0){_sys = sys;}
		//�����ڴ�����.
		inline bool GetUseSysMemory(){return _sys;}
		void Bind(T* pointer,uint size = 0){
			if(_Handle&&!_bind){
				MemoryFree(_Handle,_sys);
			}
			_Handle = pointer;
			_Size = size;
			_bind = true;
		}
		//��Ϊ����ʹ��.
		inline T& operator () (uint index){
			_ASSERT((index>=0)&&(index<_Size));
			return _Handle[index];
		}
		//���ÿռ�Ĵ�С.���lengthΪ��,�ڴ汻�ͷ�.
		bool SetLength(uint length){
			if(_bind) return false;
			if(length==_Size) return true;
			if(length==0){
				bool r = MemoryFree(_Handle,_sys);
				if(r){
					_Size = 0;
					_Handle = 0;
				}
				return r;
			}else{
				T* handle = (T*)MemoryRealloc(_Handle,length*sizeof(T),_sys);
				if(handle==0) return 0;
				_Size = length;
				_Handle = handle;
				return true;
			}
		}
		//���ÿռ��ֵΪ0
		void Zero(){
			::RtlZeroMemory(_Handle,_Size*sizeof(T));
		}

		/*@function uint Move(uint dst,uint src,int len)
		@text ��������ڱ��ռ����ƶ�ָ�����ȵ��ڴ�
		@param dst -- uint
		@text Ŀ��λ��ƫ�ƣ����ڷǰ�ģʽ�����ֵ���Դ��ڴ������еĿռ�λ�ã��ڴ���Զ����䡣���ڰ�ģʽ�����������Χ���������� 0 .
		@param src -- uint
		@text Դλ��ƫ�ƣ����ֵ���������е��ڴ淶Χ�ڡ�
		@param len -- int
		@text �ƶ��ڴ�ĳ��ȣ������һ�� int ֵ����� len С�� 0����ʶ��Դ��ַ����ǰĩβ������
		@return uint
		@text ����ʵ���ƶ������ݳ��ȡ�*/
		uint Move(uint dst,uint src,int len){
			if(src>_Size) return 0;	//Դ��ַ���ܳ������з�Χ
			if(len<0) len = _Size - src;	//�������С�����ʾ��ĩβ
			else if(len>(int)(_Size-src)) return 0;//Դ���Ȳ��ܳ����ַ�Χ
			uint useLen = (dst+len)*sizeof(T);	//������Ҫ�ĳ���
			if(useLen>_Size){
				if(_bind) return 0; //bind mode can't expand memory
				if(!SetLength(useLen)) return 0;	//��������޷����䷵�� 0
			}
			::RtlMoveMemory(_Handle+dst,_Handle+src,len*sizeof(T));
			return len;
		}
		//����(������)�ռ�,���length<0,��ռ��С.index ������(������)��λ��(��λ�ú�������ݻᱻ�ƶ�),С��0����ڵ���_Size���ĩβ���.
		bool Expand(int length,uint index = -1){
			if(_bind) return false;
			if(length==0) return 1;
			T* handle;
			if(length>0){
				handle = (T*)MemoryRealloc(_Handle,(_Size+length)*sizeof(T),_sys);
				if(handle==0) return 0;
				if((index>=0)&&(index<_Size))
					::RtlMoveMemory(handle+index+length,handle+index,sizeof(T)*(_Size-index));
			}else{
				if((index>=0)&&(index<_Size))
					::RtlMoveMemory(_Handle+index+length,_Handle+index,sizeof(T)*(_Size-index));
				handle = (T*)MemoryRealloc(_Handle,(_Size+length)*sizeof(T),_sys);
				if(handle==0) return 0;
			}
			_Handle = handle;
			_Size += length;
			return 1;
		}
		//�����ӱ�,�����ǰ��_SizeΪ0,������Ϊ0x20, ���ָ����minSize���ڴ�ӱ���ﲻ��nimSize�Ļ����ᱻ��ΪminSize.
		bool Double(int minSize = 0){
			if(_bind) return false;
			int size;
			if(_Size){ 
				size = _Size*2;
			}else{ 
				size = 0x20;
			}
			if(size<minSize) size = minSize;
			void* handle = (T*)MemoryRealloc(_Handle,size*sizeof(T),_sys);
			if(handle){
				_Handle = (T*)handle;
				_Size = size;
			}
			return handle!=0;
		}
		//�ͷ��ڴ�,����ǰ�״̬�������󶨵�ָ�룬�������ð�״̬����������������ı��Ƿ�ʹ��ϵͳ�ڴ�״̬.
		bool Free(){
			if(_bind){
				_Handle = 0;
				_Size = 0;
				_bind = 0;
				return 1;
			}else if(MemoryFree(_Handle,_sys)){
				_Handle = 0;
				_Size = 0;
				return 1;
			}
			return 0;
		}
		//�󶨵��ڴ���뿼����ϵͳ�ڴ滹�Ǹ����ڴ�.
		bool Attach(T* handle,bool sys = 0){
			if(_Handle&&!_bind) MemoryFree(_Handle,_sys);
			_Size = MemorySize(handle)/sizeof(T);
			_Handle = handle;
			_sys = sys;
			_bind = false;
			return 1;
		}
		//����󶨵��ڴ���.
		T* Detach(){
			if(_bind) return 0;
			T* handle = _Handle;
			_Handle = 0;
			_Size = 0;
			return handle;
		}
		/*@function uint CopyFrom(const T* handle,uint length,uint offset = 0)
		@text �����ݴ�ָ�����ڴ濽���������ڷǰ�ģʽ�£������Ҫ�ڴ���ܱ�����
		@param handle -- const T*
		@text Դ�ڴ���
		@param length -- uint
		@text Ҫ�������ڴ泤��
		@param offset -- uint
		@text ��������λ��ƫ��
		@return uint
		@text �ɹ������ĳ���*/
		uint CopyFrom(const T* handle,uint length,uint offset = 0){
			uint requirLen = length + offset;
			if(requirLen<length) return 0; //���������ķ�Χ
			if(requirLen>_Size){
				if(_bind) return 0;	//bindģʽ���ڴ治�����䡣
				if(!SetLength(requirLen)) return 0;
			}
			MoveMemory(_Handle+offset,handle,length*sizeof(T));
			return length;
		}
		//length��Ҫ�����ĳ���,offset�ǿ�������ʼλ��,������õĳ��Ⱥ�λ�ó����߽�,��length�ᱻ��С.����ֵ��ʵ�ʿ����ĳ���.
		uint CopyTo(T* handle,int length,uint offset = 0){
			if(offset>=_Size||length==0) return 0;
			if(length<0) length = _Size - offset;
			if((_Size-offset)<(uint)length) return 0; //ָ���ĳ���̫��
			MoveMemory(handle,_Handle+offset,length*sizeof(T));
			return length;
		}
		//����ʵ���Ĵ�С,Ҳ���ǿ��Դ洢��Ӧ���͵�Ԫ�ظ���.
		inline uint Length(){return _Size;}
		//�Ƿ��ģʽ
		inline bool IsBind(){
			return _bind;
		}
	};
	typedef Memory<char> Mbs;
	//�����ڴ�,�����ڴ���䷽ʽ���ص���,���Է���ܴ���ڴ�,��ֱ�����ʹ��ʱ(д������)�Ż�ռ�������ڴ�.
	struct CONCISE_API VirtualAddress : public HandleType<void*>{
	public:
		VirtualAddress();
		inline ~VirtualAddress(){::VirtualFree(_Handle,0,MEM_RELEASE);}
		//����������ڴ����С��λ�� 64KB,section = 1,�����64KB; section = 2�����128KB...
		VirtualAddress(uint section);
		//���������Ϊ���������Ƿ�����԰�ʾ��������,�������������·��䱣���ĵ�ַ�ռ��С.
		bool Reserve(uint section);
		//������ĵ�ַ�ռ䣬ֻ���ύ֮��,�ڴ�ſ���ʹ��.���lengthΪ0,���ύ���������ĵ�ַ�ռ�.ע���ύ���ڴ�����ڱ����ķ�Χ֮��.�ύ����С��λ��ҳ����XP����4096��4K)�ֽڡ�
		void* Commit(LPVOID p = 0,uint length = 0,DWORD protect = PAGE_READWRITE);
		//���±���һ�����ύ������,���pΪ0,������������.
		bool DeCommit(LPVOID p = 0,uint length = 0);
		//��������,���ڴ��Կ���.
		void* Desert(LPVOID p = 0,uint length = 0);
		//ȡ����Ϣ��
		bool QueryInfo(PMEMORY_BASIC_INFORMATION pbi,void* p = 0);
		//�����ڴ�ı�����ʽ
		DWORD SetProtect(DWORD protect,void* p = 0,DWORD length = 0);
		//���ص������ڴ�,��߷����ٶ�.
		bool Lock(LPVOID p = 0,uint length = 0);
		//�ͷż��ص��ڴ档
		bool Unlock(LPVOID p = 0,uint length = 0);
		//�����ڴ���ֽڳ��ȡ�
		inline uint Length(){return _Size;}
		//�����ڴ����������
		inline uint Section(){return _Size/0x10000;}
		//�����ڴ��ҳ����
		inline uint Page(){return _Size/0x1000;}
		//��ȫ�ͷš�
		bool Free();
	protected:
		uint _Size;
	};
	//�����ڴ����, �����Լ�ʹ����������ɸ����ڴ����, ϵͳ�ĸ����ڴ����һ��FastMemoryʵ��.
	class CONCISE_API IMemory : public _class{
	protected:
		class CONCISE_API CS : public _class{
		protected:
			CRITICAL_SECTION _Handle;
		public:
			CS(){InitializeCriticalSection(&_Handle);}
			~CS(){DeleteCriticalSection(&_Handle);}
			//��ʼ�������룬���ú�������߳�û�е���Unlock֮ǰ�������̶߳�Lock�ĵ��ö������
			void Lock(){EnterCriticalSection(&_Handle);}
			//���������������߳������һ�������������
			void Unlock(){LeaveCriticalSection(&_Handle);}
		};
		//ʹ��LocalCriticalSection������������Lock��Unlock�������Զ��������������Ĵ��롣
		//����ֻ�ں�������Ϊ�ֲ�����ʹ�á�
		class CONCISE_API LCS : public _class{
		protected:
			CS* _handle;
		public:
			LCS(CS& cs){_handle = &cs;_handle->Lock();}
			~LCS(){_handle->Unlock();}
		};
		void*	_handle;
		uint	_length;
		char*	_cur;
		CS		_cs;

		char* _findSpaceForward(uint len);
		char* _findSapceAfter(uint len);
		char* _allocIn(char* pt,uint len);
	public:
		IMemory();
		//�����Ƿ�Ϊ��.
		inline bool IsNull(){return _cur==NULL;}
		//�����ڴ�
		void* Alloc(uint len);
		//ȡ����, ���mem��һ����Ч�ľ��, �˺�����Ȼ�᷵��һ��������ķ���ֵ.
		uint GetLength(void* mem);
		//���·��䳤�ȣ�����³��ȱ��ϳ��ȶ̣�ʵ����λ���ǲ���ģ�Ҳ���追���ڴ档
		void* ReAlloc(void* mem,uint size);
		//��Releaseģʽ��,��������޷��ж�mem����Ч��, �������mem����ȷ, ���ܻ��ƻ������ڴ����. �����ͷŵ��ڴ沢�����������.
		bool Free(void* mem);
		//��ӡ�ڲ�������Ϣ
		void PrintInfo();
	};
	class CONCISE_API ExecMemory : public IMemory{
	protected:
		HANDLE	_map;
	public:
		bool Create(int size);
		bool Close();
	};
	class CONCISE_API FastMemory : public IMemory{
	public:
		//��ʼ���������ڴ��С, ��ֵ���ɸ���, 32λϵͳ��ʹ�õ��ڴ������޵�, ���ǿ�ʹ�õ�����ڴ�, ��һ���������ô�������ڴ�.
		bool Create(uint total);
		//��MB��ʼ��
		inline bool CreateMB(uint n){return Create(n*0x100000);}
		//һ���ڴ�ָ���Ƿ���Ԥ�����ڴ�
		bool IsFastMemory(void* mem);
	};
#define FASTMEM_SIZE_MB	16
#define EXECMEM_SIZE	0x10000
	CONCISE_API FastMemory* GetGlobalFastMemory();
	CONCISE_API ExecMemory* GetGlobalExecMemory();
	CONCISE_API void InitConciseLib(uint fastMemSize = FASTMEM_SIZE_MB,uint execMemSize = EXECMEM_SIZE);
}
