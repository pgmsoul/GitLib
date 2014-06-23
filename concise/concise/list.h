#pragma once
#include <new>
namespace cs{
	//List��һ��������,����ͬ��ʵ����ֵ.��������Ǵ�����ڴ���󣬶����������Ļ���Ӧ��ʹ��ObjectList�������档
	template<typename T> class CONCISETL_API List : public _class{
	protected:
		uint _Count,		//Ԫ������.
			_Grow;			//�ռ�����ʱ������,0��ʾ�ӱ������ݷ�ʽ.
		Memory<T> _Handle;	//���ڴ洢Ԫ�ص��ڴ�.
	public:
		//Ĭ�Ϲ��캯�������ݷ�ʽΪ�ӱ�(����Ϊ0).
		List():_Count(0),_Grow(0){
		}
		//grow����������ʱ������,Ϊ0���ʾ����ʱ,�����ӱ�.
		List(uint grow):_Count(0)(0){
			if(grow<0) _Grow = 0;
			else _Grow = grow;
		}
		inline bool SetUseSysMemory(bool sys = 0){return _Handle.SetUseSysMemory(sys);}
		inline bool GetUseSysMemory(){return _Handle.GetUseSysMemory();}
		//��������������.Ϊ0���ʾ����ʱ,�����ӱ�.
		void SetGrow(uint grow){
			if(grow<0) _Grow = 0;
			else _Grow = grow;
		}
		//��������,������õ���ֵСʵ����Ԫ�صĸ���,�������ʲôҲ����.
		void SetCubage(uint cubage){
			if(cubage>=_Count) _Handle.SetLength(cubage);
		}
		//ʹʵ��ʹ�õ��ڴ������ܹ��洢��ǰ��Ԫ��,Ҳ�����ͷŶ���ĵ��ڴ�.
		void Realize(){
			_Handle.SetLength(_Count);
		}
		//���һ��Ԫ��,index ����ӵ�λ��,-1 ��ʾ��ӵ�ĩβ,����Ч����ߵķ�ʽ.�������Ԫ�صĵ�ǰλ������.
		uint Add(T& value,uint index = -1){
			if(_Count==_Handle.Length()){
				if(_Grow) _Handle.Expand(_Grow);
				else _Handle.Double();
			}
			if(index>_Count) index = _Count;
			_Handle.Move(index+1,index,_Count-index);
			_Handle[index] = value;
			_Count++;
			return index;
		}
		//�������һ���������б������ڴ�,���Ǳ�����λ�õ�Ԫ�ز�δ����ֵ.ע��������ڴ���ܴ���������ڴ�,���б�����䷽ʽ�й�.
		//���ص�һ������Ԫ�ص�λ������,Ԫ������Count�����ᱻ���¸�ֵ��
		uint Expand(uint count,uint index = -1){
			if((count+_Count)<=_Count) return -1;
			if(_Handle.Length()<(count+_Count)){
				uint length;
				if(_Grow){
					length = count + _Count - _Handle.Length();
					if(length%_Grow)
						length = (length/_Grow + 1)*_Grow;
				}else{
					length = _Handle.Length();
					if(length==0) length = 8;
					while(length<(count+_Count))
						length *= 2;
					length -= _Handle.Length();
				}
				if(!_Handle.Expand(length,index)) return -1;
			}
			if(index==-1) index = _Count;
			_Handle.Move(index+count,index,_Count-index);
			_Count += count;
			return index;
		}
		//����ɾ����Ԫ�ظ���,��������������ͷ��Ѿ�������ڴ�.
		uint Delete(uint start,uint count = 1){
			if(start>=_Count) return 0;
			if((start+count)<=start) return 0;
			if((start+count)>_Count) count = _Count - start;
			_Handle.Move(start,start+count,_Count-start-count);
			_Count -= count;
			return count;
		}
		//ɾ���������ĵ�һ��ֵΪ value ��Ԫ��,���Ԫ�ز�����,���� 0.
		bool DeleteValue(T& value,uint start = 0){
			for(uint i=start;i<_Count;i++){
				if(memcmp(&value,_Handle+i,sizeof(T))==0)
					return Delete(i,1)!=0;
			}
			return 0;
		}
		//�������Ԫ��.�������ͷ�ռ�õ��ڴ�,��SetCubage�������ڴ��ռ��.��Release���ͷ��ڴ�.��Realize�������ͷŶ�����ڴ�.
		inline void Clear(){
			_Count = 0;
		}
		//����Ԫ�ظ���
		inline uint Count() const {return _Count;
		}
		//Ԫ�������ָ��
		inline const T* GetHandle(){return _Handle;}			
		//�����б������.
		inline uint Cubage(){return _Handle.Length();}	
		//ȡ���൱������Ԫ��.����ַ���ص�Ԫ��,���Դ��ⲿ����Ԫ�ص�ֵ.û�б߽���.
		inline T& Element(uint index){
			_ASSERT(index<_Count);
			return _Handle[index];
		}
		//ȡ���൱������Ԫ��.����ַ���ص�Ԫ��,���Դ��ⲿ����Ԫ�ص�ֵ.û�б߽���.
		inline T& operator [] (uint index){
			_ASSERT(index<_Count);
			return _Handle[index];
		}
		//������Ӧ����Ԫ�ص�ָ��,���index�����߽�,����0.
		T* GetElement(uint index){
			if(index>=_Count) return 0;
			return _Handle + index;
		}
		//�ض�ֵ�������е�����,��start��ʼ����,���û�����ֵ,����-1.
		uint IndexOf(T& value,uint start = 0){
			for(uint i=start;i<_Count;i++)
				if(CompareMemory(_Handle+i,value,sizeof(T))==sizeof(T))
					return i;
			return -1;
		}
		//���Ԫ��,�����ͷ������ڴ�.
		void Release(){
			_Handle.Free();
			_Count = 0;
		}
		//����������������Ӧ��Ԫ�أ�index=0��Ӧ�����б�����һ��Ԫ�ء�
		inline T& RevElement(uint index){
			_ASSERT(index<_Count);
			return _Handle[_Count-index-1];
		}
	};
	//Diction�����List�������������ֻ����ֵ��ͬ��Ԫ��.
	template<typename T> class CONCISETL_API Diction : public List<T>{
	public:
		//������ӵ�ֵ��λ������,�������Ƿ��Ѿ����б���.exist����ȷ����ӵ�ֵ�Ƿ��Ѿ����б���.
		uint Add(T& value,uint index = -1,bool* exist = 0){
			for(uint i=0;i<_Count;i++) if(memcmp(_Handle+i,&value,sizeof(T))==0){
				if(exist) *exist = true;
				return i;
			}
			if(exist) *exist = 0;
			return List<T>::Add(value,index);
		}
	};
	//������List����ͬ��,������Ч�ʵĿ���,��ֻ��������ֳ���Ԫ��.
	template<typename T> class CONCISETL_API IntegerList : public List<T>{
	public:
		IntegerList(){
			_ASSERT(sizeof(T)==sizeof(uint));
		}
		IntegerList(uint grow){
			_ASSERT(sizeof(T)==sizeof(uint));
			if(grow<0) _Grow = 0;
			else _Grow = grow;
		}
		//���һ��Ԫ��,index ����ӵ�λ��,-1 ��ʾ��ӵ�ĩβ,����Ч����ߵķ�ʽ.������Ԫ�ص���ʼλ�ã�-1��ʾ�ڴ治�㵼��ʧ�ܡ�
		uint Add(T value,uint index = -1){
			if(_Count==_Handle.Length()){
				if(_Grow) _Handle.Expand(_Grow);
				else _Handle.Double();
			}
			if(index>_Count) index = _Count;
			_Handle.Move(index+1,index,_Count-index);
			_Handle[index] = value;
			_Count++;
			return index;
		}
		//ɾ���������ĵ�һ��ֵΪ value ��Ԫ��,���Ԫ�ز�����,���� 0.
		bool RemoveValue(T value,uint start = 0){
			for(uint i=start;i<_Count;i++) if(value==_Handle[i]) return Delete(i,1)==1;
			return 0;
		}
		//�б߽����Ԫ��ֵ������������磬���� 0 �� 
		inline T GetElement(uint index){
			if(index>=_Count) return 0;
			return _Handle[index];
		}
		//�ض�ֵ�������е�����,��start��ʼ����,���û�����ֵ,����-1.
		uint IndexOf(T value,uint start = 0){
			for(uint i=start;i<_Count;i++) if(_Handle[i]==value) return i;
			return -1;
		}
	};
	//IntegerList��ӦDictin��.
	template<typename T> class CONCISETL_API IntegerDiction : public IntegerList<T>{
	public:
		//���һ��ֵ,������ֵ�Ѿ������ڼ�����,������Ӧ������.exist�����ж��Ƿ��Ѿ��������ֵ.
		uint Add(T value,uint index = -1,bool* exist = 0){
			for(uint i=0;i<_Count;i++){
				if(_Handle[i]==value){
					if(exist) *exist = true;
					return i;
				}
			}
			if(exist) *exist = false;
			return IntegerList<T>::Add(value,index);
		}
	};
	//ObjectList�������ʵ����ָ��,�����Զ����ɺ�������Щʵ��.һ����˵,��Ϊnew��delete�ͽ�������ص�,���Ծ�����Ҫ��
	//�ⲿ���ɵ�ʵ����ָ����ӽ���,��Ϊ����������Ƴ��Ļ�,�������Զ���delete��ɾ��ʵ��,��Ҫ�Լ���֤���ǿɿ���.
	template<typename T> class CONCISETL_API ObjectList : public _class{
	protected:
		IntegerList<T*> _Ptrs;
	public:
		inline ObjectList(){
		}
		//ָ��������
		inline ObjectList(uint grow):_Ptrs(grow){
		}
		//����ʹ�������ڴ�, ��������ɵĶ���ʵ�����ڴ�, �Լ�ObjectList����ʹ�õ��ڴ�.
		inline bool SetUseSysMemory(bool sys = 0){
			return _Ptrs.SetUseSysMemory(sys);
		}
		inline bool GetUseSysMemory(){
			return _Ptrs.GetUseSysMemory();
		}
		//����������
		inline void SetGrow(uint grow){
			_Ptrs.SetGrow(grow);
		}
		//����������
		inline void SetCubage(uint cubage){
			_Ptrs.SetCubage(cubage);
		}
		//���ز����Ԫ��ָ��
		T* Add(uint index = -1){
			T* obj = new T;
			_Ptrs.Add(obj,index);
			return obj;
		}
		//��Ӷ������,���ز����λ��.-1��ʾû��Ԫ�ر����.
		uint Expand(uint count,uint index = -1){
			_ASSERT(count>0);//���Ǽ���õ�.
			if(count==0) return -1;
			if(index>_Ptrs.Count()) index = _Ptrs.Count();
			if(_Ptrs.Cubage()<(_Ptrs.Count()+count))
				index = _Ptrs.Expand(count,index);
			count += index;
			for(uint i=index;i<count;i++)
				_Ptrs[i] = new T;
			return index;
		}
		/*Delete�������б����Ƴ����ɸ�Ԫ��.����ɾ��ʵ������.index--Ҫ�Ƴ�Ԫ�ص�λ��.count--�Ƴ�Ԫ�صĸ���,��count����index��ĩβ�ĸ���ʱ,ɾ����ĩβ.��������ʵ��ɾ���ĸ���.*/
		uint Delete(uint index,uint count = 1){
			if(index>=_Ptrs.Count()) return 0;
			if((_Ptrs.Count()-index)<count) count = _Ptrs.Count() - index;
			if(count==0) return 0;
			uint index2 = count + index;
			for(uint i=index;i<index2;i++) delete _Ptrs[i];
			return _Ptrs.Delete(index,count);
		}
		//ɾ���ض���Ԫ�ء�
		inline bool Delete(T* object){
			delete object;
			return _Ptrs.RemoveValue(object);
		}
		//��Delete����Ψһ��ͬ����,�������ֻ�Ƴ�Ԫ��,�����ڴ���ɾ��ʵ��.Ҳ����˵,��Ҫ�Լ��ֶ�ɾ��ʵ��.
		uint RemoveOut(uint index,uint count = 1){
			if(index>=_Ptrs.Count()) return 0;
			if((_Ptrs.Count()-index)<count) count = _Ptrs.Count() - index;
			if(count==0) return 0;
			return _Ptrs.Delete(index,count);
		}
		//���ⲿ���ɵ�ʵ����ӵ�������,������ӵ���λ������, ��Ҫ�����ڴ����͵�һ��.
		uint AddIn(T* object,uint index = -1){
			if(index>_Ptrs.Count()) index = _Ptrs.Count();
			_Ptrs.Expand(1,index);
			_Ptrs[index] = object;
			return index;
		}
		//����ָ��������Ԫ�ء�
		inline T* Element(uint index){
			_ASSERT(index<_Ptrs.Count());
			return _Ptrs[index];
		}
		//����ָ��������Ԫ�أ�������T*��Ϊ�˷�ֹ�ⲿ��дԪ�أ��������List��ͬ���������ⲿֱ�Ӹ�дԪ��ֵ��
		inline T& operator [] (uint index){
			_ASSERT(index<_Ptrs.Count());
			return *_Ptrs[index];
		}
		//����������������Ӧ��Ԫ�أ�index=0��Ӧ�����б�����һ��Ԫ�ء�
		inline T* RevElement(uint index){
			_ASSERT(index<_Ptrs.Count());
			return _Ptrs[_Ptrs.Count()-index-1];
		}
		//Ԫ�ظ���
		inline uint Count(){return _Ptrs.Count();}
		//����ָ��������Ԫ�أ�����������磬���� 0 ��
		inline T* GetElement(uint index){
			return _Ptrs.GetElement(index);
		}
		//�������Ԫ�ء�
		void Clear(){
			for(uint i=0;i<_Ptrs.Count();i++) delete _Ptrs[i];
			_Ptrs.Clear();
		}
		//�����ص�Ԫ�ص�������������ڼ����У�����-1.
		inline uint IndexOf(T* pt,uint start = 0){
			return _Ptrs.IndexOf(pt,start);
		}
		//����
		~ObjectList(){
			for(uint i=0;i<_Ptrs.Count();i++) delete _Ptrs[i];
		}
	};
	/*ObjectLink����һϵ�еĶ���ʵ�����������������Զ�ɾ�����г�Ա������Ԫ��ʹ��Move����λ�ã�Element����Ԫ��
	����Ԫ�غ�ɾ��Ԫ�����κ�λ�ö�����ͬ�ģ����追���ڴ棬���Ǻ�ObjectList������𣬵���ÿ��������ռ��8�ֽڵ�
	�ռ�*/
#define LINK_FIRST		0
#define LINK_CURRENT	1
#define LINK_LAST		2
	template<typename T> class CONCISETL_API ObjectLink : public _class{
	protected:
		template<typename T> class CONCISETL_API  _Link : public _class{
		public:
			T* handle;
			_Link* previous,*next;
			_Link(){
				previous = 0;
				handle = 0;
				next = 0;
			}
		};
		uint _count;
		bool _sys;
		_Link<T>* _first,*_current,*_last;
	public:
		ObjectLink(bool sys = 0):_first(0),_current(0),_last(0),_count(0),_sys(sys){
		}
		~ObjectLink(){
			while(_last){
				delete _last->handle;
				_current = _last;
				_last = _last->previous;
				delete _current;
			}
		}
		inline bool GetUseSysMemory(){return _sys;}
		//���Ԫ�أ�0������ڿ�ͷ��1������ڵ�ǰ��2������ڽ�β
		T* Add(int link_ = LINK_LAST){
			return Add(new T,link_);
		}
		//��ָ���Ķ���ָ����ӵ����ϣ����ϸ�������ָ��ָ��Ķ���
		T* Add(T* t,int link_ = LINK_LAST){
			if((link_<0)||(link_>2)) return 0;
			if(_first==0){
				_first = new _Link<T>;
				_first->previous = 0;
				_first->next = 0;
				_first->handle = t;
				_current = _first;
				_last = _first;
				_count++;
				return _first->handle;
			}
			_Link<T>* link = new _Link<T>;
			link->handle = t;
			switch(link_)
			{
			case LINK_FIRST:
				link->previous = 0;
				link->next = _first;
				_first->previous = link;
				_first = link;
				break;
			case LINK_CURRENT:
				_ASSERT(_current!=0);
				if(_current==0){
					delete link;
					return 0;
				}
				link->previous = _current;
				link->next = _current->next;
				if(_current->next){
					_current->next->previous = link;
				}
				_current->next = link;
				break;
			case LINK_LAST:
				link->previous = _last;
				link->next = 0;
				_last = link;
				link->previous->next = link;
				break;
			}
			_count++;
			return link->handle;
		}
		bool Delete(T* obj){
			for(First();;Move(1)){
				T* elm = Element();
				if(elm==0) break;
				if(elm==obj){
					Delete();
					return true;
				}
			}
			return false;
		}
		//ɾ��һ���ڵ㣬0��ɾ����һ��Ԫ�أ�1��ɾ����ǰλ��Ԫ�أ�2��ɾ��ĩβԪ�ء�delObjָʾ�Ƿ�ɾ���󶨶���
		bool Delete(int link_ = LINK_CURRENT,bool delObj = 1){
			if((link_<0)||(link_>2)) return 0;
			if(_first==0) return 0;
			_Link<T>* link;
			switch(link_)
			{
			case LINK_FIRST:
				link = _first->next;
				if(link) link->previous = 0;
				if(_current==_first){
					_current = _first->next;
				}
				if(_last==_first) _last = 0;
				if(delObj) delete _first->handle;
				delete _first;
				_first = link;
				break;
			case LINK_CURRENT:
				_ASSERT(_current!=0);
				if(_current==0){
					return 0;
				}
				if(_current->previous){
					_current->previous->next = _current->next;
					link = _current->previous;
				}
				else link = 0;
				if(_current->next){
					_current->next->previous = _current->previous;
					link = _current->next;
				}
				if(delObj) delete _current->handle;
				delete _current;
				if(_current==_first){
					_first = link;
				}
				if(_current==_last){
					_last = link;
				}
				_current = link;
				break;
			case LINK_LAST:
				link = _last->previous;
				if(link) link->next = _last->next;
				if(_last->next) _last->next->previous = _last->previous;
				if(_current==_last){
					_current = _last->previous;
				}
				if(_first==_last) _first = 0;
				if(delObj) delete _last->handle;
				delete _last;
				_last = link;
				break;
			}
			_count--;
			return 1;
		}
		//�е�ʱ����Ҫ���б��β�����һ��Ԫ�أ�ͬʱ����Ҫ�ڿ�ͷɾ��һ��Ԫ�أ����������������Ԫ�ض����Ӻ�����
		//��һ��λ�ã��ܵ������ǲ���ġ�������������Ĳ�����ʹ��PushBack���������Ա������ɺ�ɾ���κζ��󣬴Ӷ�
		//���Ч�ʡ�����������ı䵱ǰָ���Ԫ�أ�Ҳ���ı䣬overflow��־��������������£���ȫ���ܴ�ָ��ͷ����
		//Ϊָ��β����
		//PushBack�������κ���Ԫ�أ�Ҳ��ɾ���κ�Ԫ�أ����ص�Ԫ����ʵ���� first��
		T* PushBack(){
			if(_first==0) return 0;//���ڿյ������޷�ִ�к��Ʋ�����
			if(_first->next){
				_Link<T>* link = _first;
				_first = _first->next;
				_first->previous = 0;
				_last->next = link;
				link->next = 0;
				link->previous = _last;
				_last = link;
			}
			return _last->handle;
		}
		//��PushBack�෴�����������ͷ����ӣ�β��ɾ��Ԫ�ء�
		T* PushAhead(){
			if(_last==0) return 0;//�յ������޷�ִ��ǰ�Ʋ�����
			if(_last->previous){
				_Link<T>* link = _last;
				_last = _last->previous;
				_last->next = 0;
				_first->previous = link;
				link->previous = 0;
				link->next = _first;
				_first = link;
			}
			return _first->handle;
		}
		//����Ԫ�ص�����
		inline uint Count(){
			return _count;
		}
		//ָ���һ��Ԫ�ء�
		inline bool First(){
			_current = _first;
			return _current!=0;
		}
		//ָ�����һ��Ԫ�ء�
		inline bool Last(){
			_current = _last;
			return _current!=0;
		}
		//���״̬���������״̬��������ǰԪ�ر�ָ�� NULL�����״ֻ̬��ʹ��Move�������ﵽ������ָʾ����Ĳ����Ѿ��ﵽ��β����ǰ��
		//ע�⣬���״ֻ̬��һ����������������ǰMove���µĻ������Move���µģ���������ͬ�ġ�
		//һ�����������״̬���޷�ʹ��Move���������ã�����ʹ��First��Last������ȡ�����״̬��
		//һ���յ��������Ǵ������״̬��
		inline bool OverFlow(){
			return _current==NULL;
		} 
		//���ص�ǰԪ�أ��������Ϊ�գ����� 0 .
		inline T* Element(){
			if(_current!=0)
				return _current->handle;
			else
				return 0;
		}
		//�ڲ��ı䵱ǰָ�������»�ȡ��Ԫ��
		inline T* GetFirstElement(){
			if(_first==0) return 0;
			return _first->handle;
		}
		//�ڲ��ı䵱ǰָ�������£���ȡβԪ�ء�
		inline T* GetLastElement(){
			if(_last==0) return 0;
			return _last->handle;
		}
		//������������󣬸�����ǰ������ʵ�ʲ����Ĳ�����ֱ�������������ֹͣ��
		int Move(int step){
			if(step==0) return 0;
			if(_current==0) return 0;
			int n = 0;
			if(step>0){
				while(step>n){
					_current = _current->next;
					if(_current==0) break;
					n++;
				}
			}else{
				while(step<n){
					_current = _current->previous;
					if(_current==0) break;
					n--;
				}
			}
			return n;
		}
		void Clear(){
			while(_last){
				delete _last->handle;
				_current = _last;
				_last = _last->previous;
				delete _current;
			}
			_first = 0;
			_current = 0;
			_last = 0;
			_count = 0;
		}
	};
	template<typename KEY,typename VALUE> class Map : public _class{
	protected:
		ObjectList<KEY> _keys;
		ObjectList<VALUE> _values;
	public:
		void Add(KEY* key,VALUE* val,uint index = -1){
			_keys.AddIn(key,index);
			_values.AddIn(val,index);
		}
		VALUE* Add(KEY* key,uint index = -1){
			_keys.AddIn(key,index);
			return _values.Add(index);
		}
		uint Add(uint index = -1){
			_keys.Add(index);
			_values.Add(index);
			if(index<=_values.Count()) return index;
			else return _keys.Count()-1;
		}
		VALUE* Value(KEY* key){
			uint index = _keys.IndexOf(key);
			return _values.GetElement(index);
		}
		VALUE* Value(uint index){
			return _values.GetElement(index);
		}
		KEY* Key(uint index){
			return _keys.GetElement(index);
		}
		bool Delete(KEY* key){
			uint index = _keys.IndexOf(key);
			if(index==-1) return 0;
			_values.Delete(index);
			_keys.Delete(index);
			return 1;
		}
		bool Delete(int index){
			if(index>=_keys.Count()) return 0;
			_keys.Delete(index);
			_values.Delete(index);
			return 1;
		}
		bool RemoveOut(uint index){
			if(index>=_keys.Count()) return 0;
			_keys.RemoveOut(index);
			_values.RemoveOut(index);
			return true;
		}
		VALUE* RemoveOut(KEY* key){
			uint index = _keys.IndexOf(key);
			if(index==-1) return 0;
			VALUE* val = _values.Element(index);
			_values.RemoveOut(index);
			return val;
		}
	};
	//������	class C : public TPTree<C>{
	//			};
	//������ʽʹ��TPTree��
	template<typename T> class TPTree : public _class{
	protected:
		T*				_parent;
		ObjectList<T>	_childs;
	public:
		TPTree(){
			_parent = 0;
		}
		T* GetParent(){
			return _parent;
		}
		T* Add(uint index = -1){
			T* child = _childs.Add(index);
			child->_parent = (T*)this;
			return child;
		}
		uint Delete(T* pt){
			return _childs.Delete(pt);
		}
		uint Delete(uint index,uint count = 1){
			return _childs.Delete(index,count);
		}
		uint IndexOf(T* pt){
			return _childs.IndexOf(pt);
		}
		uint AddIn(T* pt,uint index = -1){
			pt->_parent = (T*)this;
			return _childs.AddIn(pt,index);
		}
		T* RemoveOut(uint index){
			T* child = _childs.GetElement(index);
			if(child==0) return 0;
			child->_parent = 0;
			_childs.RemoveOut(index);
			return child;
		}
		bool RemoveOut(T* pt){
			uint index = _childs.IndexOf(pt);
			if(index==-1) return 0;
			_childs.RemoveOut(index);
			pt->_parent = 0;
			return true;
		}
		T* Element(uint index){
			return _childs.Element(index);
		}
		T* GetElement(uint index){
			return _childs.GetElement(index);
		}
		uint Count(){
			return _childs.Count();
		}
		void Clear(){
			_childs.Clear();
		}
	};
	//����ֱ��ʹ�õ�һ��Tree
	template<typename T> class Tree : public _class{
	protected:
		Tree*				_parent;
		ObjectList<Tree>	_childs;
	public:
		T					Handle;
		Tree(){
			_parent = 0;
		}
		Tree* GetParent(){
			return _parent;
		}
		Tree* Add(uint index = -1){
			Tree* child = _childs.Add(index);
			child->_parent = this;
			return child;
		}
		uint Delete(Tree* pt){
			return _childs.Delete(pt);
		}
		uint Delete(uint index,uint count = 1){
			return _childs.Delete(index,count);
		}
		uint IndexOf(Tree* pt){
			return _childs.IndexOf(pt);
		}
		uint AddIn(Tree* pt,uint index = -1){
			pt->_parent = this;
			return _childs.AddIn(pt,index);
		}
		Tree* RemoveOut(uint index){
			Tree* child = _childs.GetElement(index);
			if(child==0) return 0;
			child->_parent = 0;
			_childs.RemoveOut(index);
			return child;
		}
		bool RemoveOut(Tree* pt){
			uint index = _childs.IndexOf(pt);
			if(index==-1) return 0;
			_childs.RemoveOut(index);
			pt->_parent = 0;
			return true;
		}
		Tree* Element(uint index){
			return _childs.Element(index);
		}
		Tree* GetElement(uint index){
			return _childs.GetElement(index);
		}
		uint Count(){
			return _childs.Count();
		}
		void Clear(){
			_childs.Clear();
		}
	};
}
