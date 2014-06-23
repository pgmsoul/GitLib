#include "../stdafx.h"
#include "../concise.h"

namespace concise{
	bool checkStrValidName(LPCWSTR name){
		int len = WcsLength(name);
		for(int i=0;i<len;i++){
			if((name[i]==' ')||(name[i]=='\t')||(name[i]=='\r')||(name[i]=='\n')||
				(name[i]=='\"')||(name[i]=='=')||(name[i]=='>')||(name[i]=='<')||(name[i]=='/'))
				return 0;
		}
		return 1;
	}
	bool checkStrValidValue(LPCWSTR var){
		int len = WcsLength(var);
		for(int i=0;i<len;i++){
			if(var[i]=='\"')
				return 0;
		}
		return 1;
	}
	bool checkStrValidValue2(LPCWSTR var){
		int len = WcsLength(var);
		for(int i=0;i<len;i++){
			if((var[i]=='<')||(var[i]=='>'))
				return 0;
		}
		return 1;
	}
	void formatValue(String& val){
		int sp = val.Find(L"\n");	//�Ƴ���ͷ��һ������
		if(sp==0) val.Tail(1);
		sp = val.Find(L"\r\n");		//�Ƴ���ͷ��һ���س�����
		if(sp==0) val.Tail(2);
		int i = 0;
		//�Ƴ���ͷ�Ŀո��tab�ַ���
		for(;i<(int)val.Length();i++){
			if((val[i]=='\t')||(val[i]==' ')) continue;
			break;
		}
		val.Tail(i);
		//�Ƴ���β�Ŀո��tab�ַ�
		for(i=val.Length()-1;i>=0;i--){
			if((val[i]=='\t')||(val[i]==' ')) continue;
			break;
		}
		val.Head(i+1);
		//�Ƴ�β����һ���س�����
		sp = val.Length();
		if((sp>0)&&(val[sp-1]=='\n')){
			val.Head(-1);
			sp--;
		}
		if((sp>0)&&(val[sp-1]=='\r')){
			val.Head(-1);
		}
	}
	bool Xml::SetName(LPCWSTR name)
	{
		if(!checkStrValidName(name)) return 0;
		this->name = name;
		return 1;
	}
	bool Xml::SetValue(LPCWSTR val){
		if(!checkStrValidValue2(val)) return 0;
		value = val;
		formatValue(value);
		return 1;
	}
	void Xml::ClearProperty(){
		atrb.Clear();
	}
	bool Xml::DelProperty(LPCWSTR name){
		if(!checkStrValidName(name)) return 0;
		for(uint i=0;i<atrb.Count();i++){
			if(atrb[i].Name==name){
				atrb.Delete(i);
				return 1;
			}
		}
		return 0;
	}
	LPCWSTR Xml::GetPropertyName(int index){
		prop* p = atrb.GetElement(index);
		if(p==0) return 0;
		return p->Name;
	}
	LPCWSTR Xml::GetProperty(int index){
		prop* p = atrb.Element(index);
		if(p==0) return 0;
		return p->Value;
	}
	LPCWSTR Xml::GetProperty(LPCWSTR pName){
		if(!checkStrValidName(pName)) return 0;
		for(uint i=0;i<atrb.Count();i++){
			if(atrb[i].Name==pName){
				return atrb[i].Value;
			}
		}
		return 0;
	}
	bool Xml::SetProperty(LPCWSTR pName, LPCWSTR pValue){
		if(!checkStrValidName(pName)) return 0;
		if(!checkStrValidValue(pValue)) return 0;
		for(uint i=0;i<atrb.Count();i++){
			if(atrb[i].Name==pName){
				atrb[i].Value = pValue;
				return 1;
			}
		}
		prop* p = atrb.Add();
		p->Name = pName;
		p->Value = pValue;
		return 1;
	}
	Xml* Xml::GetNode(LPCWSTR name, bool always){
		if(!checkStrValidName(name)) return 0;
		for(uint i=0;i<_childs.Count();i++){
			if(_childs[i].name==name)
				return (Xml*)&_childs[i];
		}
		if(!always) return 0;
		Xml* node = (Xml*)_childs.Add();
		node->name = name;
		return node;
	}
	Xml* Xml::AddNode(LPCWSTR name,int index){
		Xml* node = (Xml*)_childs.Add(index);
		node->name = name;
		return node;
	}
	Xml* Xml::AddNode(LPCWSTR name,LPCWSTR val,int index){
		Xml* node = (Xml*)_childs.Add(index);
		node->name = name;
		node->value = val;
		return node;
	}
	bool Xml::DelNode(LPCWSTR name,int start){
		if(!checkStrValidName(name)) return 0;
		for(uint i=start;i<_childs.Count();i++){
			if(_childs[i].name==name){
				_childs.Delete(i);
				return 1;
			}
		}
		return 0;
	}
	void Xml::SetValueInt(int v,int radix){
		value.FromInt32(v,radix,0);
	}
	bool Xml::GetValueInt(int& v,int radix){
		return value.ToInt32(v,radix);
	}
	void Xml::SetValueDouble(double v,int width){
		String fs;
		if(width>=0){
			fs.Format(L"%d",width);
			fs.Insert(L"%.",0);
			fs.Insert(L"f");
		}else{
			fs = L"%f";
		}
		value.Format(fs,v);
	}
	bool Xml::GetValueDouble(double& v){
		wchar_t* os = 0;
		v = wcstod(value,&os);
		return os==0||os[0]==0;
	}
	void Xml::SetValueHex(const void* data,int len){
		value.HexBinary(data,len);
	}
	int Xml::GetValueHex(void* data){
		Memory<byte> buf;
		int len = value.ToBinary(buf);
		if(data==0) return len;
		buf.CopyTo((byte*)data,len);
		return len;
	}
	bool Xml::SetPropertyInt(LPCWSTR name,int v,int radix){
		String str;
		str.FromInt32(v,radix,0);
		return SetProperty(name,str);
	}
	bool Xml::GetPropertyInt(LPCWSTR name,int& v,int radix){
		LPCWSTR str = GetProperty(name);
		if(str==0) return 0;
		wchar_t* os = 0;
		v = wcstoul(str,&os,radix);
		return os==0||os[0]==0;
	}
	bool Xml::SetPropertyDouble(LPCWSTR name,double v,int width){
		String str,fs;
		if(width>=0){
			fs.Format(L"%d",width);
			fs.Insert(L"%.",0);
			fs.Insert(L"f");
		}else{
			fs = L"%f";
		}
		str.Format(fs,v);
		return SetProperty(name,str);
	}
	bool Xml::GetPropertyDouble(LPCWSTR name,double& v){
		String str = GetProperty(name);
		if(str.Length()==0) return 0;
		v = str.ToFloat();
		return 1;
	}
	bool Xml::SetPropertyHex(LPCWSTR name,const void* data,int len){
		String str;
		str.HexBinary(data,len);
		return SetProperty(name,str);
	}
	int Xml::GetPropertyHex(LPCWSTR name,void* data){
		LPCWSTR pv = GetProperty(name);
		if(pv==0) return 0;
		String str = pv;
		Memory<byte> buf;
		int len = str.ToBinary(buf);
		if(data==0) return len;
		buf.CopyTo((byte*)data,len);
		return len;
	}

	void Xml::_ToString(String& str,int Indent)
	{
		String sp;
		for(int i=0;i<Indent;i++)
		{
			sp += L"  ";
		}
		Indent++;
		str = sp;
		str += L"<";
		str += name;
		for(uint i=0;i<atrb.Count();i++){
			prop* p = &atrb[i];
			str += L"\r\n";
			str += sp;
			str += L"  ";
			str += p->Name;
			str += L"=";
			str += L"\"";
			str += p->Value;
			str += L"\"";
		}
		str += L">";

		if(value.Length()!=0){
			str += L"\r\n";
			str += sp;
			str += L"  ";
			str += value;
		}

		String ns;
		for(int i=0;i<(int)_childs.Count();i++)
		{
			str += L"\r\n";
			((Xml*)_childs.Element(i))->_ToString(ns,Indent);
			str += ns;
		}

		str += L"\r\n";
		str += sp;
		str += L"</";
		str += name;
		str += L">";
	}
	void Xml::ToString(String& ns)
	{
		_ToString(ns,0);
	}
	inline void checkSpaceStr(LPCWSTR str,uint& right){
		while((str[right]==' ')||(str[right]=='\t')||(str[right]=='\r')||(str[right]=='\n')){
			right++;
		}
	}
	int isCloseFlag(LPCWSTR str){
		if(str[0]==0) return 1;
		if(str[0]=='>') return 1;
		if(str[0]=='/'){
			if(str[1]=='>') return 2;
			else return 0;
		}
		return 0;
	}
	uint getSplitChar(LPCWSTR str,uint len){
		static wchar_t* spc[5] ={L"/>",L">",L" ",L"\r",L"\n"};
		uint right = -1,pos;
		for(int i=0;i<5;i++){
			pos = WcsFind(str,spc[i]);
			if(pos<right) right = pos;
		}
		return right;
	}
	uint Xml::_FromString(LPCWSTR str,uint len){
		//�ѵ�ǰ Node �ÿա�
		_childs.Clear();
		name = L"";
		atrb.Clear();

		//Ѱ�ҵ�һ��������ţ�֮ǰ�����ݶ�����
		uint slen = 0;
		uint left = WcsFind(str,L"<");
		if(left>=len){
			Print(L"not find <");
			return 0;
		}
		left += 1;

		//��������
		//checkSpaceStr(str,left);
		//�������ƽ�β��
		slen = getSplitChar(str+left,len-left);
		if(slen>=(len-left)){
			Print(L"not find name tail,pos:%d",left);
			return 0;
		}
		//�����ڵ�����
		name.CopyFrom(str+left,slen);
		//Print(L"<%s",(LPCWSTR)name);
		left += slen;

		//��������
		String pn,pv;
		while(1){
			pn = L"";
			pv = L"";
			checkSpaceStr(str,left);
			int closeLen = isCloseFlag(str+left);
			left += closeLen;
			if(closeLen==1){
				//Print(L">");
				break;
			}
			if(closeLen==2){
				//Print(L"/>");
				return left;
			}
			//���ҵ�һ����=���š�
			slen = WcsFind(str+left,L"=");
			if(slen>=(len-left)){
				Print(L"not find =,pos:%d",left);
				return 0;
			}
			pn.CopyFrom(str+left,slen);
			left += slen + 1;
			//��������ֵ������
			slen = WcsFind(str+left,L"\"");
			if(slen>=(len-left)){
				Print(L"not find left \",pos:%d",left);
				return 0;
			}
			uint pLeft = left + slen + 1;
			left = pLeft;
			//����������
			slen = WcsFind(str+left,L"\"");
			if(slen>=(len-left)){
				Print(L"not find right \",pos:%d",left);
				return 0;
			}
			pv.CopyFrom(str+pLeft,slen);
			left += slen + 1;
			prop* p = atrb.Add();
			//Print(L" %s=\"%s\"",(LPCWSTR)pn,(LPCWSTR)pv);
			p->Name = pn;
			p->Value = pv;
		}
		//�����պϴ�
		String back = L"</";
		back += name;
		back += L">";
		bool setValue = 1;
		//�����ӽڵ�
		while(left<len){
			//������һ����ʶ�������ַ�ȫ�������ԡ�
			slen = WcsFind(str+left,L"<");
			if(slen>(len-left)){
				Print(L"not find <,pos:%d",left);
				return 0;
			}
			if(setValue){
				value.CopyFrom(str+left,slen);
				formatValue(value);
				setValue = false;
			}
			left += slen;
			//�Ƿ�պϴ�
			if(WcsEqual(back,str+left,back.Length())){
				left += back.Length();
				//Print(back);
				break;
			}
			//���պϣ������ӽڵ㡣
			Xml* node = (Xml*)_childs.Add();
			slen = node->_FromString(str+left,len-left);
			if(slen==0){ 
				Print(L"parse sub node failed,pos:%d",slen);
				return 0;
			}
			left += slen;
		}
		return left;
	}
	bool Xml::Save(LPCWSTR file,LPCWSTR cp){
		DWORD code = GetCodePage(cp);
		if(code==CP_THREAD_ACP) cp = L"ANSI";
		File sf;
		if(!sf.Create(file,CREATE_ALWAYS)) return 0;
		String xs,str;
		ToString(xs);
		str = L"<?xml version=\"1.0\" encoding=\"";
		str += cp;
		str += L"\"?>\r\n";
		str += xs;
		Memory<char> buf;
		int len = str.ToMultiByte(&buf,code);
		sf.SetLength(0);
		return len==sf.Write(buf.Handle(),len);
	}
	bool Xml::FromString(LPCWSTR str)
	{
		LPWSTR ns = (LPWSTR)str;
		uint start = 0;
		checkSpaceStr(ns,start);
		ns += start;
		uint fpos = WcsFind(ns,L"<?");
		if(fpos==0){
			ns += 2;
			fpos = WcsFind(ns,L"?>");
			if(fpos!=-1)
				ns += fpos+2;
			else 
				return false;
		}
		int len =_FromString(ns,WcsLength(ns));
		return 1;
	}
	bool Xml::Load(LPCWSTR file,DWORD code){
		File sf;
		if(!sf.Create(file,OPEN_EXISTING)) return 0;
		Memory<char> buf;
		int len = sf.Read(&buf);

		String str;
		str.FromMultiByte(buf,len,code);
		return FromString(str);
	}
	bool Xml::Load(ResID res,DWORD code){
		DWORD size = 0;
		void* pm = GetResource(res,L"LAYOUT",&size);
		if(pm==0) return 0;

		String str;
		str.FromMultiByte((LPCSTR)pm,size,code);
		return FromString(str);
	}
	Xml* Xml::OpenMultiLevelNode(LPCWSTR name,bool create){
		StringMemList sl;
		sl.SplitString(name,L"/");
		Xml* pt = this;
		for(uint i=0;i<sl.Count();i++){
			bool nFound = 1;
			for(uint j=0;j<pt->Count();j++){
				if(WcsEqual(pt->Element(j)->GetName(),sl[i])){
					pt = pt->Element(j);
					nFound = 0;
					break;
				}
			}
			if(nFound){
				if(create) pt = pt->Add();
				else return 0;
				if(pt==0) return 0;
				if(!pt->SetName(sl[i])) return 0;
			}
		}
		return pt;
	}
	bool Xml::DeleteMultiLevelNode(LPCWSTR ns)
	{
		StringMemList sl;
		sl.SplitString(ns,L"/");
		Xml* pt = this;
		for(uint i=0;i<sl.Count();i++){
			bool nFound = 1;
			for(uint j=0;j<pt->Count();j++){
				if(WcsEqual(pt->Element(j)->GetName(),sl[i])){
					if((sl.Count()-1)==i){
						return pt->Delete(j)==1;
					}
					pt = pt->Element(j);
					nFound = 0;
					break;
				}
			}
			if(nFound) return 0;
		}
		return 0;
	}

	class register_struct : public _class{
	public:
		Xml Tree;
		String FileName;
		CriticalSection Lock;
		int icount;
	};
	//��һ��Xml������ͬһ��������ͬ�� fn ������ͬ�Ķ���.
	//��Щ�ڴ�ֱ����������Ż��ͷ�
	register_struct* getRegisterObject(LPCWSTR name,register_struct* obj = 0){
		static ObjectList<register_struct> dbList;

		CriticalSection cs;
		LocalCriticalSection lcs(cs);

		if(obj!=NULL){
			obj->icount--;
			if(obj->icount==0) dbList.Delete(obj);
			return 0;
		}
		String fn,title;
		if(WcsLength(name)==0){
			fn = App::GetAppDirectory();
			title = App::GetAppName();
			FPLinkPath(fn,title);
			FPLinkExt(fn,L"xml");
		}else{
			fn = name;
		}

		register_struct* rs = 0;
		for(uint i=0;i<dbList.Count();i++){
			rs = &dbList[i];
			if(FPIsSame(fn,rs->FileName)){
				rs->icount++;
				return rs;
			}
		}
		rs = new register_struct;
		rs->icount = 1;
		if(1!=GetFileType(fn)){
			File f;
			f.Create(fn);
		}
		if(!rs->Tree.Load(fn)){
			delete rs;
			return 0;
		}
		rs->FileName = fn;
		title = fn;
		FPToTitle(title);
		rs->Tree.SetName(title);
		dbList.AddIn(rs);
		return rs;
	}

	RegisterTree::RegisterTree():_innerobj(0){}
	RegisterTree::RegisterTree(LPCWSTR fname):_innerobj(0){
		_innerobj = getRegisterObject(fname);
	}

	bool RegisterTree::Lock(){
		if(!_innerobj) return 0;
		((register_struct*)_innerobj)->Lock.Lock();
		return true;
	}
	void RegisterTree::Unlock(){
		if(_innerobj==0) return;
		Flush();
		((register_struct*)_innerobj)->Lock.Unlock();
	}
	void RegisterTree::Close(){
		if(_innerobj==0) return;
		Flush();
		register_struct* rs = (register_struct*)_innerobj;
		_innerobj = 0;
		rs->Lock.Unlock();
		getRegisterObject(0,rs);
	}
	bool RegisterTree::Create(LPCWSTR fname){
		if(_innerobj) return 1;
		_innerobj = getRegisterObject(fname);
		return _innerobj!=NULL;
	}
	Xml* RegisterTree::GetXml(){
		if(_innerobj==0) return 0;
		return &((register_struct*)_innerobj)->Tree;
	}
	RegisterTree::~RegisterTree(){
		if(_innerobj==0) return;
		register_struct* rs = (register_struct*)_innerobj;
		rs->Tree.Save(rs->FileName);
		rs->Lock.Unlock();
		getRegisterObject(0,rs);
	}
	void RegisterTree::Flush(){
		if(_innerobj==0) return;
		((register_struct*)_innerobj)->Tree.Save(((register_struct*)_innerobj)->FileName);
	}
}
