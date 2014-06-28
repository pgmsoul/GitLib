#pragma once
namespace cs{
	/*JavaScript ���Եı����� 7 �֣����� 1 �� Null ���ͣ���������ֻ��һ��ֵ����
	������ȱʡ��ʼֵ��4 ��ֵ���ͣ��������͡��������͡��������ͺ��ִ����ͣ�2 ��
	�������ͣ�����Ͷ���������һϵ�еı������ϣ����������� 7 �����͵��κ�һ�֡�
	������һ���ֵ䣬����һϵ�е� key �� value ����ɣ����� key �������ִ����ͣ�
	ֵ������ 7 �����͵��κ����͡�

	Json �� JavaScript �������ݽṹΪԭ�ͣ��� C++ �������Ӧ�Ĺ��ܡ�
	*/
	enum JSON_TYPE{
		json_null = 0,
		json_boolean = 1,
		json_integer = 2,
		json_double = 3,
		json_string = 4,
		json_array = 5,
		json_object = 6,
		json_binary = 7
	};
	class CONCISE_API Json : public _class{
	public:
		typedef struct DICTION : public _struct{
			String key;
			Json* val;
			DICTION(){
				val = new Json;
			}
			~DICTION(){
				delete val;
			}
		}DICTION;
		union{
			bool _boolValue;
			int64 _intValue;
			double _doubleValue;
			String* _strValue;
			ObjectLink<Json>* _arrayValue;
			ObjectLink<DICTION>* _objectValue;
			Memory<char>*	_binaryValue;
		};
		JSON_TYPE _type;
		~Json(){
			SetToNull();
		}
		Json():_type(json_null),_doubleValue(0){
		}
		Json(bool val):_type(json_null){
			*this = val;
		}
		void operator = (bool val){
			SetToNull();
			_boolValue = val;
			_type = json_boolean;
		}
		Json(const wchar_t* str):_type(json_null){
			*this = str;
		}
		void operator = (const wchar_t* str){
			SetString(str);
		}
		Json(const char* str):_type(json_null){
			*this = str;
		}
		void operator = (const char* mbs){
			SetMbStr(mbs);
		}
		Json(int64 val):_type(json_null){
			*this = val;
		}
		Json(int val):_type(json_null){
			*this = val;
		}
		void operator = (int64 val){
			SetToNull();
			_intValue = val;
			_type = json_integer;
		}
		void operator = (int val){
			SetToNull();
			_intValue = val;
			_type = json_integer;
		}
		Json(double val):_type(json_null){
			*this = val;
		}
		void operator = (double val){
			SetToNull();
			_doubleValue = val;
			_type = json_double;
		}
		Json(Json& val):_type(json_null){
			*this = val;
		}
		void operator = (Json& val);
		bool IsNull(){
			return json_null==_type;
		}
		bool IsBoolean(){
			return json_boolean==_type;
		}
		bool IsInteger(){
			return json_integer==_type;
		}
		bool IsDouble(){
			return json_double==_type;
		}
		//�����򸡵�����
		bool IsNumber(){
			return json_double==_type||json_integer==_type;
		}
		bool IsString(){
			return json_string==_type;
		}
		bool IsObject(){
			return json_object==_type;
		}
		bool IsArray(){
			return json_array==_type;
		}
		bool IsBinary(){
			return json_binary==_type;
		}
		bool Boolean(bool def = 0){
			if(json_boolean==_type) return _boolValue;
			else return def;
		}
		bool GetBoolean(bool& val){
			if(json_boolean!=_type) return false;
			val = _boolValue;
			return true;
		}
		operator bool (){
			return Boolean(false);
		}
		double Double(double def = 0){
			if(json_double==_type) return _doubleValue;
			else if(json_integer==_type) return (double)_intValue;
			else return def;
		}
		bool GetDouble(double& val){
			if(json_double==_type||json_integer==_type){
				val = _doubleValue;
				return true;
			}
			return false;
		}
		operator double (){
			return Double(0.0);
		}
		int64 Int64(int64 def = 0){
			if(json_integer==_type) return _intValue;
			else if(json_double==_type) return (int64)_doubleValue;
			else return def;
		}
		int64 GetInt64(int64& val){
			if(json_integer==_type) val = _intValue;
			else if(json_double==_type) val = (int64)_doubleValue;
			else return false;
			return true;
		}
		operator int64 (){
			return Int64(0);
		}
		int Int32(int def = 0){
			if(json_integer==_type) return (int)_intValue;
			else if(json_double==_type) return (int)_doubleValue;
			else return def;
		}
		bool GetInt32(int& val){
			if(json_integer==_type) val = (int)_intValue;
			else if(json_double==_type) val = (int)_doubleValue;
			else return false;
			return true;
		}
		operator int (){
			return Int32(0);
		}
		LPCWSTR Str(LPCWSTR def = 0){
			if(json_string==_type) return _strValue->Handle();
			else return def;
		}
		bool GetString(LPCWSTR& val){
			if(json_string==_type) val = _strValue->Handle();
			else return false;
			return true;
		}
		void SetString(LPCWSTR str,int len = -1);
		void SetMbStr(LPCSTR mbs,int len = -1,DWORD code = CP_THREAD_ACP);
		operator LPCWSTR (){
			return Str(L"");
		}
		bool SetBinary(LPCVOID data,int len);
		LPCVOID GetBinary(int* len = 0);
		//SetTo�����Ѷ�������Ϊָ�����ͣ��������������������ͣ��˺���ʲô�����������򣬻�ǿ����Ϊ�����͵�ȱʡֵ��
		void SetToArray();
		void SetToBoolean();
		void SetToInteger();
		void SetToDouble();
		void SetToString();
		void SetToObject();
		void SetToBinary();
		void SetToNull();
		//���ı��������ͣ�����ֵ����Ϊ��Ӧ���͵�ȱʡֵ��
		void Empty();
		//Property������Ե�ǰ����Ϊobject����ʱ������������ǰ���Ͳ��� object ���� null��createIfNotExist = false,���в�����ʧ�ܡ���� createIfNotExist = true������ᱻǿ��ת��Ϊ object ���͡�
		Json* GetProperty(LPCWSTR prop,bool createIfNotExist = false);
		//��ȡ���Ե��ִ�ֵ��������Բ����ڻ������Բ����ִ����ͣ�����false��
		bool GetPropertyStr(LPCWSTR prop,LPCWSTR& val){
			Json* js = GetProperty(prop);
			if(!js||!js->IsString()) return 0;
			val = js->Str();
			return 1;
		}
		//�������Ե��ִ�ֵ��������Բ����ڻ������Բ����ִ����ͣ��������õ�ȱʡֵ def ��
		LPCWSTR PropertyStr(LPCWSTR prop,LPCWSTR def = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsString()) return def;
			return js->Str();
		}
		//��ȡ���Ե�32λ����ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ����� false��������ֵ��32λ�����󣬻ᱻ�ضϡ�
		bool GetPropertyInt32(LPCWSTR prop,int& val){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return 0;
			val = js->Int32();
			return 1;
		}
		//�������Ե�32λ����ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ��������õ�ȱʡֵ def ��������ֵ��32λ�����󣬻ᱻ�ضϡ�
		int PropertyInt32(LPCWSTR prop,int def = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return def;
			return js->Int32();
		}
		//��ȡ���Ե�64λ����ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ�����false��
		bool GetPropertyInt64(LPCWSTR prop,int64& val){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return 0;
			val = js->Int64();
			return 1;
		}
		//�������Ե�64λ����ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ��������õ�ȱʡֵ def ��
		int64 PropertyInt64(LPCWSTR prop,int64 def = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return def;
			return js->Int64();
		}
		//��ȡ���Ե� bool ֵ��������Բ����ڻ������Բ��ǲ������ͣ�����false��
		bool GetPropertyBool(LPCWSTR prop,bool& val){
			Json* js = GetProperty(prop);
			if(!js||!js->IsBoolean()) return 0;
			val = js->Boolean();
			return 1;
		}
		//�������Ե� bool ֵ��������Բ����ڻ������Բ��ǲ������ͣ��������õ�ȱʡֵ def ��
		bool PropertyBool(LPCWSTR prop,bool def = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsBoolean()) return def;
			return js->Boolean();
		}
		//��ȡ���Եĸ���ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ�����false��
		bool GetPropertyDouble(LPCWSTR prop,double& val){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return 0;
			val = js->Double();
			return 1;
		}
		//�������Եĸ���ֵ��������Բ����ڻ������Բ������֣������򸡵㣩���ͣ��������õ�ȱʡֵ def ��
		double PropertyDouble(LPCWSTR prop,double def = 0.0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsNumber()) return def;
			return js->Double();
		}
		//��ȡ���ԵĶ�����ֵ��������Բ����ڻ������Բ��Ƕ��������ͣ�����false��len �������ݵĳ��ȡ�
		bool GetPropertyBinary(LPCWSTR prop,LPCVOID& val,int* len = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsBinary()) return 0;
			val = js->GetBinary(len);
			return 1;
		}
		//�������ԵĶ�����ֵ��������Բ����ڻ������Բ��Ƕ��������ͣ��������� NULL ��len �������ݵĳ��ȡ�
		LPCVOID PropertyBinary(LPCWSTR prop,int* len = 0){
			Json* js = GetProperty(prop);
			if(!js||!js->IsBinary()) return 0;
			return js->GetBinary(len);
		}
		//��������Ϊָ�����ִ�ֵ
		Json* SetPropertyStr(LPCWSTR prop,LPCWSTR val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ�����ִ�ֵ
		Json* SetPropertyMbs(LPCWSTR prop,LPCSTR val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ����32λ����ֵ
		Json* SetPropertyInt32(LPCWSTR prop,int val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ����64λ����ֵ
		Json* SetPropertyInt64(LPCWSTR prop,int64 val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ���ĸ���ֵ
		Json* SetPropertyDouble(LPCWSTR prop,double val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ���Ĳ���ֵ
		Json* SetPropertyBool(LPCWSTR prop,bool val){
			Json jv = val;
			return SetProperty(prop,jv);
		}
		//��������Ϊָ���Ķ�����ֵ
		Json* SetPropertyBinary(LPCWSTR prop,const void* data,int len){
			Json jv;
			if(!jv.SetBinary(data,len)) return 0;
			return SetProperty(prop,jv);
		}
		//���õ�ǰ���������ֵΪ�գ������ǰ������ Object ���� Null ���ͣ�����ʧ�ܣ����� NULL��
		//Json* SetProperty(LPCWSTR prop);
		//���õ�ǰ���������ֵΪָ����ֵ�������ǰ������ Object ���� Null ���ͣ�����ʧ�ܣ����� NULL��
		Json* SetProperty(LPCWSTR prop,Json& val);
		//PropertyҲ����ͨ�������������������ǽ����ڶ�ȡ��ɾ����һ�㲻Ҫͨ���������������ԡ�
		//GetPropertyByIndex ��ȡ�� key �� pval ��ָ������ڲ���ֵ������Ӧ��ֻ���ڶ�ȡ���ǵ�ֵ����Ҫ�������ǡ�
		Json* GetPropertyByIndex(int index,LPCWSTR* key = 0);
		bool RemoveProperty(LPCWSTR prop);
		bool RemovePropertyByIndex(int index);
		//Array���������Ե�����������ʽ���ڲ�������ʵ�֣������ٶȱ����Կ죬�ر����������Ƚϴ��ʱ��ɾ����Ӳ��Ҷ�Ч�ʸ��ߡ�
		//������ָ���ƶ�����ͷ
		void ArrayFirst(){
			if(_type!=json_array) return;
			_arrayValue->First();
		}
		//������ָ���ƶ���ĩβ
		void ArrayLast(){
			if(_type!=json_array) return;
			_arrayValue->Last();
		}
		//�ƶ�����ָ�룬������󣬸�����ǰ
		int ArrayMove(int step){
			if(_type!=json_array) return 0;
			return _arrayValue->Move(step);
		}
		//��������ĵ�ǰԪ�أ������ǰ����Ϊ�գ����� NULL��
		Json* GetArrayElm(){
			if(_type!=json_array) return 0;
			return _arrayValue->Element();
		}
		//��������ĵ�ǰԪ�ص�ֵ�����ҷ������Ԫ�أ������ǰ���������飬���� NULL��
		Json* SetArrayElm(Json& val){
			if(_type!=json_array) return 0;
			Json* elm = _arrayValue->Element();
			*elm = val;
			return elm;
		}
		//����һ������Ԫ��, pos = LINK_FIRST��0�� ���뵽��ǰ�棬pos = LINK_CURRENT��1����
		//���뵽��ǰλ�õĺ��棻pos = LINK_LAST��2�������뵽����档����������ı䵱ǰԪ�ص�ָ�롣
		//���ز������Ԫ�ء�
		Json* AddArrayElm(int pos = LINK_LAST){
			if(_type==json_null) SetToArray();
			if(_type!=json_array) return 0;
			return _arrayValue->Add(pos);
		}
		//�Ƴ���ǰ����Ԫ�أ���ǰָ��ָ���һ��Ԫ�أ����û�к�һ��Ԫ�أ����������
		void RemoveArrayElm(){
			if(_type!=json_array) return;
			_arrayValue->Delete();
		}
		//�����Ƿ������״̬
		bool ArrayOverFlow(){
			return _arrayValue->OverFlow();
		}
		bool GetArrayStr(LPCWSTR& val){
			Json* js = GetArrayElm();
			if(!js||!js->IsString()) return 0;
			val = js->Str();
			return 1;
		}
		bool GetArrayInt32(int& val){
			Json* js = GetArrayElm();
			if(!js||!js->IsInteger()) return 0;
			val = js->Int32();
			return 1;
		}
		bool GetArrayInt64(int64& val){
			Json* js = GetArrayElm();
			if(!js||!js->IsInteger()) return 0;
			val = js->Int64();
			return 1;
		}
		bool GetArrayBool(bool& val){
			Json* js = GetArrayElm();
			if(!js||!js->IsBoolean()) return 0;
			val = js->Boolean();
			return 1;
		}
		bool GetArrayDouble(double& val){
			Json* js = GetArrayElm();
			if(!js||!js->IsDouble()) return 0;
			val = js->Double();
			return 1;
		}
		Json* SetArrayStr(LPCWSTR val){
			Json jv(val);
			return SetArrayElm(jv);
		}
		Json* SetArrayMbs(LPCSTR val){
			Json jv(val);
			return SetArrayElm(jv);
		}
		Json* SetArrayBool(bool val){
			Json jv(val);
			return SetArrayElm(jv);
		}
		Json* SetArrayInt32(int val){
			Json jv(val);
			return SetArrayElm(jv);
		}
		Json* SetArrayInt64(int64 val){
			Json jv(val);
			return SetArrayElm(jv);
		}
		Json* SetArrayDouble(double val){
			Json jv(val);
			return SetArrayElm(jv);
		}

		//Count���ص�ǰ����Ϊ�����Objectʱ�������������������ͷ��� 0 ��
		uint Count(){
			if(_type==json_array) return _arrayValue->Count();
			else if(_type==json_object) return _objectValue->Count();
			return 0;
		}
		//readStyleָ���ִ��Ƿ����Ķ�����Ķ�����лس����к����������Ҳ�ת�������ַ����ʺ��Ķ���
		//���readStyle = false�������ַ��ᱻ���룬����û�л��к��������ʺϴ���ͱ��档
		void ToString(String& str,bool readStyle = true);
		bool Parse(LPCWSTR jstr,int len = -1);
		bool Parse(LPCSTR jstr,int len,DWORD codepage = CP_THREAD_ACP);
		bool LoadFromFile(LPCWSTR fn,DWORD codepage = CP_THREAD_ACP);
		bool SaveToFile(LPCWSTR fn,DWORD codepage = CP_THREAD_ACP,bool readStyle = true);

		Json& operator [] (int index){
			if(_type==json_array){
				if((int)_arrayValue->Count()>index){
					_arrayValue->First();
					_arrayValue->Move(index);
					return *_arrayValue->Element();
				}else{
					Json* a;
					for(int i=_arrayValue->Count();i<=index;i++)
						a = _arrayValue->Add();
					return *a;
				}
			}else if(_type!=json_null){
				_ASSERT(0);
				return *(Json*)0;
			}
			_arrayValue = new ObjectLink<Json>;
			_type = json_array;
			Json* a;
			for(int i=0;i<=index;i++)
				a = _arrayValue->Add();
			return *a;
		}
		Json& operator [] (LPCWSTR key){
			if(_type!=json_object){
				return *(Json*)0;
			}
			return *GetProperty(key,true);
		}
	};
	/*ʹ�÷���������ʹ�þֲ������ķ�ʽ Config cfg(0); 0 ��ʾĬ�������ļ���Ȼ���� Lock ������ȡ
	Json ����������� Close �� Unlock ��������Ϊ������������ʱ����Զ����á�
	������Ǿֲ�������ͬһ�̶߳�ε��� Lock �������ڶ��λ�ʧ�ܣ�Ҳ���� Lock �� Unlock ����
	�ϸ�ɶԵ��ã�Unlock ����֮ǰ�����������̣߳����ǵ�ǰ�̣߳����޷��ٴγɹ����� Lock ����
	��ͱ�֤���κ�ʱ�򣬴򿪶�ȡ�͹ر�д���м䲻����ͬʱһ�����ϵĲ������������ļ������ݵ�һ�¡�
	*/
	class CONCISE_API Config : public _class{
	protected:
		void* _innerobj; //it's a register_struct pointer.
	public:
		//��ʱ����ʵ�����ڲ��� Xml ����
		Config();
		//ʵ����һ�� Json ����le��ʹ�á�
		Config(LPCWSTR file);
		//��ָ���ļ���д, �ڲ��ᴴ��һ������, ���Ҷ�ȡfile�ļ�����������ʼ���������.
		bool Create(LPCWSTR file);
		//д����ǰ, Ӧ�õ���Lock, ��ռ�ڲ�����, ������Lock���ᱻ����, ֱ������Unlock����.
		//�����Ե��߳���Ч���޷�����������Ϊͬ�ļ���ֻ��һ���ڲ�����ͬ�̲߳���ͬһ������һ�㲻�������ͻ������ǡ�ò�����Json�����ͬһ��
		//λ�ã��������ֳ�ͻ�ǿɿصģ��Ͷ��̵߳ķ�����ȫ����Ԥ֪�ǲ�ͬ�ġ�
		//���ܿ���ͬʱ�ڲ���ͻ������²��������ļ�������Ӧ�ñ�֤�򿪺�д���м�ֻ��һ�������ߣ���Ϊ�����
		//����������ļ����Լ���ȡ��д�룬��û��ʹ��Config���󣬾ͻ��������ҡ�
		//���wait = true����ȴ������򿪲����رգ����wait = false���������̲߳����Ļ�����ʧ�ܡ�
		Json* Lock(bool wait = true);
		//���Lock, ��ΪΪ�˱����ļ����ݵ�һ��, ͬһʱ��ֻ����һ���򿪲���, Unlock���Զ�����Flush��
		void Unlock();
		//�����ͷ�Create�����򿪵Ķ���, ���ⲿ����ı����ļ�������ʱ.
		//��ҪClose��, ������Create��, ���ݲŻ�ˢ��, ��Ϊ�ڲ�����һ�����ɾͲ����ٴ��ļ���ȡ����.
		void Close();
		//д���ļ�, UnlockҲ��ִ���������, ���Գ����ڻ�û����Unlock����������Ҫ����Ϣд���ļ�ʱ, ����Ҫ�������������
		//void Flush();
		//�����ڲ���Xml����, ���ж�д����.
		//Json* GetJson();
		//����
		~Config();
	};
}
