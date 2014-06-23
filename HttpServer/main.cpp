#include "stdafx.h"
#pragma usexpstyle
#define file_buf_size	0x100000
char* html400 = "<HTML><HEAD><TITLE>400-Bad Request</TITLE></HEAD><BODY><H1>Error occurred: 400 - Bad Request</H1></body></HTML>";
char* html403 = "<HTML><HEAD><TITLE>403-Forbidden</TITLE></HEAD><BODY><H1>Error occurred: 404 - Forbidden</H1></body></HTML>";
char* html404 = "<HTML><HEAD><TITLE>404-Not Found</TITLE></HEAD><BODY><H1>Error occurred: 404 - Not Found</H1></body></HTML>";
char* html200 = "<html><head><title>hello</title></head><body><center><h1>Jsuser Server, Hello !!</h1></center></body></html>";
cs::Edit* outputEdit = 0;
bool debugInfo;
void log(LPCWSTR msg){
	cs::String text;
	outputEdit->GetText(text);
	text += L"\r\n";
	text += msg;
	if(text.Length()>10000)
		text.Tail(-10000);
	outputEdit->SetText(text);
	outputEdit->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}
void log(LPCSTR msg,int len){
	cs::String text;
	text.FromMultiByte(msg,len);
	log(text);
}
inline bool sendRetry(cs::Socket& s,char* data,int len){
	while(true){
		int slen = s.Send(data,len);
		if(slen==0){
			log(L"sendRetry socket error，return 0");
			return 0;
		}else if(slen==-1){
			log(L"sendRetry socket error，return -1");
			return 0;
		}
		if(slen<len){
			len -= slen;
			data += slen;
		}else return true;
	}
}
void InitContentType(cs::Json* contType){
	contType->SetPropertyStr(L"jps",L"image/jpeg");
	contType->SetPropertyStr(L"bmp",L"image/x-xbitmap");
	contType->SetPropertyStr(L"png",L"image/png");
	contType->SetPropertyStr(L"gif",L"image/gif");
	contType->SetPropertyStr(L"html",L"text/html");
	contType->SetPropertyStr(L"htm",L"text/html");
	contType->SetPropertyStr(L"txt",L"text/plain");
	contType->SetPropertyStr(L"h",L"text/plain");
	contType->SetPropertyStr(L"c",L"text/plain");
	contType->SetPropertyStr(L"cpp",L"text/plain");
	contType->SetPropertyStr(L"wav",L"audio/x-wav");
	contType->SetPropertyStr(L"m3u",L"audio/x-mpegurl");
	contType->SetPropertyStr(L"mp3",L"audio/mpeg");
	contType->SetPropertyStr(L"swf",L"application/x-shockwave-flash");
	contType->SetPropertyStr(L"php",L"text/html");
	contType->SetPropertyStr(L"css",L"text/css");
}
class ResponseHeader : cs::_class{
	cs::ObjectList<cs::String> sl;
public:
	ResponseHeader(){
		sl.Add();
		Set(L"Server",L"jsuser 1.2");
	}
	void SetCode(LPCWSTR code){
		cs::String& first = sl[0];
		first.Format(L"HTTP/1.1 %s",code);
	}
	int Get(LPCWSTR key){
		int len = cs::WcsLength(key);
		for(uint i=1;i<sl.Count();i++){
			cs::String& str = sl[i];
			if(cs::WcsEqual(key,str,len)){
				return i;
			}
		}
		return -1;
	}
	void Set(LPCWSTR key,LPCWSTR val){
		int index = Get(key);
		cs::String* add;
		if(index==-1)
			add = sl.Add();
		else
			add = &sl[index];
		*add = key;
		*add += L": ";
		*add += val;
	}
	bool Send(cs::Socket& client){
		char buf[10240],tail[3] = {0};//external 3 bytes for safe;
		uint offset = 0;
		for(uint i=0;i<sl.Count();i++){
			if(sizeof(buf)<=offset){
				log(L"response header is too long");
				return 0;
			}
			uint len = sl[i].ToMultiByte(buf+offset,sizeof(buf)-offset);
			offset += len + 2;
			buf[offset-1] = '\n';
			buf[offset-2] = '\r';
		}
		offset += 2;
		buf[offset-1] = '\n';
		buf[offset-2] = '\r';
		buf[offset] = 0;
		if(debugInfo) log(buf,offset);

		return sendRetry(client,buf,offset);
	}
};
class RequestHeader : cs::_class{
public:
	bool Get;
	cs::String RequestFile;
	int64 RangeX,RangeY;
	RequestHeader(){
		Reset();
	};
	bool IsRange(){
		return RangeX!=-1||RangeY!=-1;
	}
	void Reset(){
		RangeX = RangeY = -1;
	}
};
class Main : public cs::Frame{
	cs::Edit output;
	void onSize(cs::Twin16 cs,cs::IWnd*){
		output.SetSize(cs.x,cs.y);
	}
	cs::Json* contType;
	inline const wchar_t* GetContentType(LPCWSTR ext){
		LPCWSTR ct = contType->PropertyStr(ext);
		if(!ct) ct = L"application/octet-stream";
		return ct;
	}
	cs::StringList defPage;
	cs::String wwwroot;
	cs::TCPServer	serv;
	cs::ObjectLink<cs::Socket> clientList;
	cs::TaskPool<void,SOCKET> task;
	void onCreate(cs::IWnd*){
		cs::SetWndIcon(_Handle,1);
		output.Create(_Handle);
		bool r = serv.StartupAsyn(port);
		if(r){
			cs::String str;
			cs::Host host;
			host.Create();
			str.FromMultiByte(host.GetIp());
			str.Insert(L"服务器启动成功, 地址: ",0);
			str += L":";
			str.InsertNumber(port);
			log(str);
		}else{
			log(L"服务器启动失败，80 端口可能被占用。注意：不要启动多个实例，除非在不同的目录运行且在 httpd.json 文件里设置不同的端口。");
		}
	}
	//void outputHeader(const char* header,int len){
	//}
	// seek header wether has recv completed.
	int seekHeader(char* buf,int len){
		static int rn = 0x0A0D0A0D;
		len -= 4;
		for(int i=0;i<=len;i++){
			if(*(int*)(buf+i)==rn){
				if(debugInfo) log(buf,i+4);
				return i+4;
			}
		}
		return 0;
	}
	void errorHead(char* buf,int ofs){
		cs::String str;
		str.Format(L"头信息太长：len = %d",ofs);
		log(str);
		buf[1024] = 0;
		str.FromMultiByte(buf);
		if(debugInfo) log(str);
	}
	//分析头信息的一行，返回值的起始位置，没有找到返回 0 。
	int parseHeaderLine(const char* key,const char* line,int len){
		if(cs::MbsFindNoCase(line,key)!=0) return 0;
		int offset = cs::MbsByteLen(key);
		while(line[offset]==' ') offset++;
		return offset;
	}
	bool parseHeader(RequestHeader& head,const char* buf,int len){
		if(len<4) return 0;
		int ls[100];
		int offset = 0,count = 0;
		while(true){
			int pos = cs::MbsFind(buf+offset,"\r\n");
			if(pos==0||pos==-1) break;
			offset += pos + 2;
			ls[count] = offset;
			count++;
		}
		if(cs::MbsFindNoCase(buf,"GET",3)==0){
			head.Get = true;
			offset = 4;
		}else if(cs::MbsFindNoCase(buf,"POST",4)==0){
			head.Get = false;
			offset = 5;
		}else return 0;
		//parse request file
		len = ls[0]-offset-2;
		int pos = cs::MbsFind(buf+offset," ");
		if(pos==-1) return 0;
		head.RequestFile.FromMultiByte(buf+offset,pos);
		offset = head.RequestFile.Find('?');
		if(offset!=-1) head.RequestFile.Head(offset);
		cs::DecodeURI(head.RequestFile);
		log(head.RequestFile.Handle());
		////////////////////////
		count--;	//最后一个是尾部位置。
		for(int i=0;i<count;i++){
			int len = ls[i+1] - ls[i] - 2;
			const char* line = buf + ls[i];
			int offset = parseHeaderLine("Range:",line,len);
			if(offset){
				int pos = cs::MbsFind(line+offset,"=",len-offset);
				if(pos==-1) return false;
				cs::String str;
				offset += pos + 1;
				str.FromMultiByte(line+offset,len-offset);
				pos = str.Find(L",");
				if(pos!=-1) str.Head(pos);
				pos = str.Find(L"-");
				if(pos==-1) return false;
				cs::String sx = str;
				sx.Head(pos);
				str.Tail(pos+1);
				if(sx.IsNull()){//请求是一个负数
					head.RangeX = -1;
					head.RangeY = -str.ToInt64(10);
					if(head.RangeY<=0) return 0;
				}else{
					head.RangeX = sx.ToInt64(10);
					if(head.RangeX<0) return 0;
					if(str.IsNull()) head.RangeY = -1;
					else{
						head.RangeY = str.ToInt64(10);
						if(head.RangeY<0) return 0;
					}
				}
			}
		}
		return true;
	}
	void sendResonseForError(ResponseHeader& rh,cs::Socket& client,LPCWSTR errorcode,LPCSTR errorhtml){
		log(errorcode);
		rh.SetCode(errorcode);
		rh.Set(L"Connection",L"close");
		rh.Send(client);
		sendRetry(client,(LPSTR)errorhtml,strlen(errorhtml));
	}
	inline void sendResponse400(ResponseHeader& rh,cs::Socket& client){
		sendResonseForError(rh,client,L"400 Bad Request",html400);
	}
	inline void sendResponse403(ResponseHeader& rh,cs::Socket& client){
		sendResonseForError(rh,client,L"400 Forbidden",html403);
	}
	inline void sendResponse404(ResponseHeader& rh,cs::Socket& client){
		sendResonseForError(rh,client,L"404 Not Find",html404);
	}
	//处理请求文件，成功 fn 返回文件名，失败返回 false。
	bool parseRequestFile(RequestHeader& head,ResponseHeader& rh,cs::Socket& client,cs::String& fn){
		cs::FPLinkPath(fn,head.RequestFile);
		cs::FPToBackSlash(fn);
		int pos = fn.Find(L"\\..");
		if(pos!=-1){
			wchar_t ch = fn[pos+3];
			if(ch==0||ch=='\\'){
				sendResponse403(rh,client);
				return 0;
			} 
		}
		int ftype = cs::GetFileType(fn);
		if(ftype==0){
			sendResponse404(rh,client);
			return 0;
		}else if(ftype==2){
			cs::String path = fn;
			for(uint i=0;i<defPage.Count();i++){
				fn = path;
				cs::FPLinkPath(fn,defPage[i]);
				if(cs::GetFileType(fn)==1){
					ftype = 1;
					return 1;
				}
			}
			if(ftype==2){
				sendResponse404(rh,client);
				return 0;
			}
		}
		return 1;
	}
	bool parseRequest(RequestHeader& head,ResponseHeader& rh,cs::Socket& client,char* file_buf,bool& rqph){
		rqph = true;
		cs::String fn = wwwroot;
		if(!parseRequestFile(head,rh,client,fn)) return 0;
		cs::File file;
		bool r = file.Create(fn,OPEN_EXISTING,FILE_SHARE_READ|FILE_SHARE_WRITE,FILE_GENERIC_READ);
		if(!r){
			sendResponse403(rh,client);
			return 0;
		}
		cs::String str,num;
		int64 flen = file.GetLength(),clen;
		if(head.IsRange()){
			rh.SetCode(L"206 Partial Content");
			if(head.RangeX==-1){//RangeY 是从尾部开始的长度
				if(head.RangeY>flen) head.RangeY = flen;
				head.RangeX = flen - head.RangeY;
				head.RangeY -= 1;
			}else{
				if(head.RangeY==-1) head.RangeY = flen - 1;//没有指明范围的最大值
				if(head.RangeX>head.RangeY) head.RangeX = head.RangeY;//正确的头不应出现这种情况
			}
			clen = head.RangeY - head.RangeX + 1;
			str.FromInt64(clen,10);
			rh.Set(L"Content-Length",str);
			str = L"bytes ";
			num.FromInt64(head.RangeX,10);
			str += num;
			str += L"-";
			num.FromInt64(head.RangeY,10);
			str += num;
			str += L"/";
			num.FromInt64(flen,10);
			str += num;
			rh.Set(L"Content-Range",str);
		}else{
			rh.SetCode(L"200 OK");
			str.FromInt64(flen,10);
			rh.Set(L"Content-Length",str);
			head.RangeX = 0;
			head.RangeY = flen - 1;
			clen = flen;
		}
		cs::String ext = fn;
		cs::FPToExt(ext);
		ext.Lower();
		rh.Set(L"Content-Type",GetContentType(ext));
		rh.Send(client);
		int64 reserv = clen;
		file.SetPointer(head.RangeX);
		while(true){
			uint rlen = (uint)cs::Minimum<int64>(file_buf_size,reserv);
			int read_len = file.Read(file_buf,rlen);
			reserv -= read_len;
			if(!sendRetry(client,file_buf,read_len)){
				log(L"send data error");
				return 0;
			}
			num.FromInt64(clen-reserv,10);
			log(num);
			if(reserv<=0) break;
		}
		return 1;
	}
	void parseClientInfo(cs::Socket& client){
		cs::SOCKADDR si;
		client.GetPeerAddr(si);
		cs::String str = si.GetIp();
		str += L" 发送了新请求";
		log(str);
	}
	void onRecv(SOCKET sock){
		cs::Socket client;
		client.Attach(sock);
		parseClientInfo(client);
		cs::Memory<char> file_buf(1);
		file_buf.SetLength(file_buf_size);
		char buf[10240];
		int ofs = 0;
		ResponseHeader rh;
		RequestHeader head;
		bool requestParseHeader = 1;
		while(true){
			//recv data
			int len = client.Receive(buf+ofs,sizeof(buf)-ofs);
			if(len<0)			{
				log(L"连接被意外中断");
				break;
			}else if(len==0){
				log(L"客户端关闭了连接");
				break;
			}
			//check header recv complete
			int headLen = 0;
			ofs += len;
			if(requestParseHeader){
				headLen = seekHeader(buf,ofs);
				if(!headLen){
					if(ofs>=sizeof(buf)){
						//header data is too long,normal is error request.
						errorHead(buf,ofs);
						sendResponse400(rh,client);
						return;
					}
					//header not recv complete
					continue;
				}
				//parse header info
				head.Reset();
				if(!parseHeader(head,buf,headLen)){
					sendResponse400(rh,client);
					return;
				}
				//parse request file
				if(!parseRequest(head,rh,client,file_buf.Handle(),requestParseHeader)) break;
			}
			if(!parseRecv(buf+headLen,ofs-headLen)) break;
			ofs = 0;
		}
		log(L"连接结束");
	}
	bool parseRecv(char* data,int datalen){
		return true;
	}
	void onAccept(SOCKET sock){
		task.StartTask(this,&Main::onRecv,sock);
	}
	void onClose(bool& cancel,cs::IWnd*){
		serv.Close();
		//强制终止所有客户线程
		task.Close(100);
	}
	cs::Json httpCfg;
	WORD port;
	void loadConfig(){
		if(1!=cs::GetFileType(L"index.html")){
			cs::File file;
			if(file.Create(L"index.html")){
				file.Write(html200,strlen(html200));
			};
		}
		bool saveDef = 0;
		httpCfg.LoadFromFile(L"httpd.json");
		port = (WORD)httpCfg.PropertyInt32(L"port",0);
		if(!port){
			httpCfg.SetPropertyInt32(L"port",80);
			port = 80;
			saveDef = true;
		}

		wwwroot = httpCfg.PropertyStr(L"wwwroot");
		if(!wwwroot){
			httpCfg.SetPropertyStr(L"wwwroot",L"");
			wwwroot = L"";
			saveDef = true;
		}
		cs::FPToFullPath(wwwroot);
		LPCWSTR defpage = httpCfg.PropertyStr(L"default_page");
		if(!defpage){
			httpCfg.SetPropertyStr(L"default_page",L"index.html,index.htm");
			defpage = L"index.html,index.htm";
			saveDef = true;
		}
		defPage.SplitString(defpage,L",");

		if(!httpCfg.GetPropertyBool(L"debug_info",debugInfo)){
			httpCfg.SetPropertyBool(L"debug_info",false);
			debugInfo = false;
			saveDef = true;
		}
		contType = httpCfg.GetProperty(L"content_type",true);
		if(contType->IsNull()){
			InitContentType(contType);
			saveDef = true;
		}
		if(saveDef) httpCfg.SaveToFile(L"httpd.json");
	}
public:
	Main(){
		loadConfig();
		OnCreate.Add(this,&Main::onCreate);
		OnClose.Add(this,&Main::onClose);
		serv.OnAccept.Bind(this,&Main::onAccept);
		cs::SetWndRememberPos(this,L"main_wnd");
		OnSize.Add(this,&Main::onSize);
		Param->Text = L"Jsuser Http Server 1.2";
		output.Param->AddStyle(WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY);
		output.Param->RemoveStyle(ES_AUTOHSCROLL);
		output.Param->Location.SetValue(0,0);
		outputEdit = &output;
	}
	~Main(){
	}
};
WINMAIN{
	Main mf;
	mf.Create();
	return cs::MsgLoop::Start();
};