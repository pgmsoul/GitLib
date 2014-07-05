#pragma once
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Urlmon.lib")
namespace cs{
	//���ִ������UTF8���룬���ִ���ʾ��
	CONCISE_API void EncodeURI(String& str,DWORD codePage = CP_UTF8);
	//��UTF8������ִ���ԭ��
	CONCISE_API void DecodeURI(String& str,DWORD codePage = CP_UTF8);
	//IP�������ַ�Ľṹin_addr��
	typedef struct CONCISE_API IP : public _struct
	{
		union S_UN{
			struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
			struct { USHORT s_w1,s_w2; } S_un_w;
			ULONG S_addr;
		}S_un;
		inline bool IsNull()//-1
		{
			return S_un.S_addr==INADDR_NONE;
		}
		inline IP()
		{
			S_un.S_addr = INADDR_NONE;
		}
		inline IP(const IP& ia)
		{
			S_un.S_addr = ia.S_un.S_addr;
		}
		inline IP(in_addr ia)
		{
			S_un.S_addr = ia.S_un.S_addr;
		}
		inline IP(ulong ip)
		{
			S_un.S_addr = ip;
		}
		inline IP(const char* ip)
		{
			S_un.S_addr = inet_addr(ip);
		}
		inline void operator = (const IP& ia)
		{
			S_un.S_addr = ia.S_un.S_addr;
		}
		inline void operator = (in_addr ia)
		{
			S_un.S_addr = ia.S_un.S_addr;
		}
		inline void operator = (ulong ip)
		{
			S_un.S_addr = ip;
		}
		inline void operator = (const char* ip)
		{
			S_un.S_addr = inet_addr(ip);
		}
		void operator = (const wchar_t* ip);
		inline IP(const wchar_t* ip)
		{
			*this = ip;
		}
		inline operator in_addr ()
		{
			return Handle();
		}
		inline in_addr Handle()
		{
			return *(in_addr*)this;
		}
		inline operator ulong ()
		{
			return S_un.S_addr;
		}
		inline ulong ToLong()
		{
			return S_un.S_addr;
		}
		inline operator char* ()
		{
			return inet_ntoa(Handle());
		}
		inline bool operator == (IP& ip)
		{
			return ip.S_un.S_addr==S_un.S_addr;
		}
		inline char* ToString()
		{
			return inet_ntoa(Handle());
		}
	}IP;
	/*sockaddr_in ��ʾһ������ͨ�Žṹ
	struct sockaddr_in {
	short   sin_family;			��ַ���ͣ�ʵ���ϼ���ֻ��AF_INETһ�ֱ�ʹ�á�
	u_short sin_port;			ʹ�õĶ˿ڣ�ÿ��ͨ�Ŷ�Ҫʹ��һ���˿ڡ�
	struct  in_addr sin_addr;	��һ��ռ��4���ֽڵ�IP��ַ��
	char    sin_zero[8];		δʹ�ã����㡣
	};*/
	//SOCKADDR�ṹ����sockeaddr_in�ṹ�ĳ��ú�����������һ��sockaddr_in�ṹ��
	class CONCISE_API  SOCKADDR : public _struct
	{
		sockaddr_in _Handle;
	public:
		SOCKADDR()
		{
			RtlZeroMemory(this,sizeof(SOCKADDR));
			_Handle.sin_family = AF_INET;
		}
		//�������ʵ���ĵ�ַָ�롣
		inline operator struct sockaddr_in* ()
		{
			return (struct sockaddr_in*)&_Handle;
		}
		//�������ʵ���ĵ�ַָ�롣
		inline operator struct sockaddr* ()
		{
			return (struct sockaddr*)&_Handle;
		}
		//����ʹ�õĶ˿ڡ�һ����˵Ҫ��һ��ת���������Լ��ߵ�λ��һ�¡�����ת���Ĵ�������Ϊʹ�õı�׼��ͬ��ɵ��������⡣
		inline void SetPort(unsigned short port)
		{
			_Handle.sin_port = htons(port);
		}
		//���ض˿ں�,ͬ��Ҳ��Ҫ����һ��ת����
		inline unsigned short GetPort()
		{
			return ntohs(_Handle.sin_port);
		}
		//����IP��ַ���ִ���ʾ��in_addr��ʾ��������ʾ��
		inline void SetIp(IP addr)
		{
			_Handle.sin_addr = addr;
		}
		inline IP GetIp()
		{
			return _Handle.sin_addr;
		}
		//���õ�ַ���壬ʵ����ʹ��������AF_INET��
		inline void SetFamily(short family)
		{
			_Handle.sin_family = family;
		}
		//���ص�ַ�ļ������͡�
		inline short Family()
		{
			return _Handle.sin_family;
		}
		//��������ṹ�Ĵ�С���еĺ�����Ҫʹ�����ֵ��
		inline int Size()
		{
			return (int)sizeof(_Handle);
		}
	};
	/*
	typedef struct hostent {
	char FAR* h_name;				//������ѧ����
	char FAR  FAR** h_aliases;		//�����ı����б�һ�����������ж����������
	short h_addrtype;				//�����ĵ�ַ���ͣ�AF_INET ���� AF_INET6�ȡ�
	short h_length;					//������IP��ַ����,һ����4,û�б��ֵ
	char FAR  FAR** h_addr_list;	//������IP��ַ�ִ�.
	}*/
	//Host��ʾһ�������ṹ��
	class CONCISE_API  Host : public _class
	{
		hostent* _Hostent;
		WORD _aliasesCount,_addrCount;
	public:
		Host();
		~Host();
		bool IsNull(){return _Hostent==0;}
		//��ȡ�����ĵ�ַ��Ϣ.
		bool Create();
		//ͨ��������ʽ�������ַ��ȡ.
		bool Create(u_long addr);
		//ͨ����������ȡ��������Ϣ��name�ȿ�����һ�����������ַ:www.google.com(��Ҫ��http://����ʵ���������ֻ�Ƿ���DNS�������������������������������� ),
		//Ҳ������һ���ִ���ʽ��IP��ַ:192.168.1.1,Ҳ������һ������������������:server.
		bool Create(const char* name);
		//ͨ����������ȡ��������Ϣ��name�ȿ�����һ�����������ַ:www.google.com,Ҳ������һ���ִ���ʽ��IP��ַ:192.168.1.1,Ҳ������һ������������:server.
		bool Create(const wchar_t* name);
		//������ѧ��
		char* GetOfficial(){return _Hostent->h_name;}
		//�����ı���,һ�����������ж������
		char* GetAliases(int index = 0);
		//ȡ�����������ĸ���.
		int GetAliasesCount(){return _aliasesCount;}
		//ȡ������IP��ַ�ĸ���.
		int GetIpCount(){return _addrCount;}
		//��ȡ������IP��ַ,������������ 0 .
		IP GetIp(int index = 0);
		//��ȡ������IP��ַ����,AF_INET ���� AF_INET6��.
		int GetIpType(){return _Hostent->h_addrtype;}
		//��ȡ������IP��ַ����,һ����4,û�б��ֵ.
		int GetIpLength(){return _Hostent->h_length;}
	};
	//��װ Win Sock ����
	class CONCISE_API  Socket : public HandleType<SOCKET>
	{
	public:
		Socket();
		~Socket();
		Socket(SOCKET s);
		void operator = (SOCKET s){_Handle = s;}
		//ʵ���󶨵���һ����Ч��SOCKET
		bool IsNull(){return INVALID_SOCKET==_Handle;}
		//ʵ���󶨵���һ����Ч��SOCKET
		bool NotNull(){return INVALID_SOCKET!=_Handle;}
		//��һ����Ч��SOCKET,���s=INVALID_SOCKET,����0��
		bool Attach(SOCKET s);
		//����Ѿ��󶨵�SOCKET.
		SOCKET Detach();
		//����һ��SOCKET��Դ.
		bool Create(int protocol = IPPROTO_TCP,int type = SOCK_STREAM);
		//�ͷŰ󶨵�SOCKET��Դ.
		bool Close();


		inline bool Shutdown(int how = SD_BOTH){return shutdown(_Handle,how)==0;}
		//Accept
		SOCKET Accept(sockaddr* srv = 0);
		//������backlog�ǿ���ͬʱ�����Ŀͻ���������
		inline bool Listen(int backlog = SOMAXCONN){return 0==listen(_Handle,backlog);}
		//�󶨣�һ��ͨ�Žṹ��Bind���ڼ���Socket�����Socket�Ͳ�������Connect����Send�ˡ�
		inline bool Bind(const sockaddr* srv){return 0==bind(_Handle,srv,sizeof(sockaddr_in));}


		//���ӷ����
		inline bool Connect(const sockaddr* addr){return 0==connect(_Handle,addr,sizeof(sockaddr_in));}
		//�������ݣ�send������buf�е����ݳɹ�copy��s�ķ��ͻ����ʣ��ռ�������ͷ����ˣ����Ǵ�ʱ��Щ���ݲ���һ�����ϱ��������ӵ���һ��
		inline int Send(const char* buf,int len,int flags = 0){return send(_Handle,buf,len,flags);}
		//�������ݣ����ճɹ����ؽ����ֽ����������رշ��� 0 ������ ���� -1 ��
		inline int Receive(char* buf,int len,int flags = 0){return recv(_Handle,buf,len,flags);}
		//�����Ӵ��䷢�����ݣ����len==0�������������RecvFrom����һ�Ρ�
		inline int SendTo(const char* buf,int len,int flags,const struct sockaddr* to,int tolen = sizeof(sockaddr_in)){return sendto(_Handle,buf,len,flags,to,tolen);}
		//�����Ӵ����������
		inline int RecvFrom(char* buf,int len,int flags,struct sockaddr* from,int* fromlen){return recvfrom(_Handle,buf,len,flags,from,fromlen);}
		//�������ԡ�
		inline int SetSockOpt(int level,int opt,const char* val,int len){return setsockopt(_Handle,level,opt,val,len);}
		inline int GetSockOpt(int level,int opt,char* val,int* len){return getsockopt(_Handle,level,opt,val,len);}
		//ȡ��һ�������� Socket �Է��� SOCKADDR ��Ϣ��
		inline bool GetPeerAddr(SOCKADDR& si){
			int len = sizeof(si);
			return getpeername(_Handle,si,&len)==0;
		}
		//ȡ�ñ��� Scoket �ĵ�ַ��Ϣ��
		inline bool GetSockAddr(SOCKADDR& si){
			int len = sizeof(si);
			return getsockname(_Handle,si,&len)==0;
		}
	};
	//���ڼ������������ſͻ��˺ͷ����������ӡ���ʹ��������رգ��Ѿ�������������Ȼ���á����� Scoket ֻ���𴴽����ӣ�����������������Ҳ���������ݡ�
	class CONCISE_API  TCPServer : public _class
	{
	protected:
		Socket	_Handle;
		Thread _thread;

		void _Accept();
	public:
		//���ͻ�������������ʱ������Ӧ�Ļص�������
		Function<void,SOCKET> OnAccept;			

		~TCPServer();
		//���ذ󶨵� Socket ָ�롣
		Socket* GetSocket(){return &_Handle;}
		//ͬ��������������ֱ��Close�����ã�����������᷵�ء����Ҫ�����κ�IP�����ӣ�������ipΪINADDR_ANY��
		HRESULT Startup(ushort port,IP ip = INADDR_ANY);
		//ͨ��һ�����߳��첽���������������Ҫ�����κ�IP�����ӣ�������ipΪINADDR_ANY��
		HRESULT StartupAsyn(ushort port,IP ip = INADDR_ANY);
		//�رշ���ˡ����������������������������ӡ�
		void Close();
	};
	//TCPTransfer���͵����ݰ���4���ֽڵ�����ͷ��������TCPTransfer����������
	/*class CONCISE_API  TCPTransfer : public _class{
	protected:
		Socket				_handle;
		Memory<char>		_sendBuf;
		void*				_userParam;
		Thread				_recvThread,_sendThread;
		bool				_connected;
		Event				_sendBlock;

		void _connect(SOCKADDR sa,bool startRecv,bool asyn);
		void _send();
	public:
		Function<void,void*> OnSend;
		Function<void,void*,int,void*> OnRecv;//call back function of recv data
		Function<void,void*> OnClose;			//call back function be called when a socket be closed and have startup a recv thread,this function be call in a Asynchronous thread. 
		Function<void,bool,void*> OnConnect;

		TCPTransfer();
		~TCPTransfer();
		//���ذ󶨵� Socket ָ�롣
		Socket* GetSocket(){return &_handle;}
		//attach a socket,cn is indicate this socket whether connected.
		void Attach(SOCKET s,bool cn);
		//detach a socket from this instance
		SOCKET Detach();
		//accept a socket from server accept call back function,this is equal to Attach(s,true).
		void Accept(SOCKET s);
		//close this transfer,this don't call in any call back function(OnClose,OnRecv,OnConnect).
		void Close();
		//�ڴ�ʵ�����κλص��������������Close���������γ���������������������Ҫ�رմ�Socket������CloseSocket�����ر�Socket�����
		void CloseSocket(){_handle.Close();}
		//set user data,the last param of OnClose and OnRecv.
		void SetUserParam(void* param);
		//connect a server,startRecv indicate whether auto starup recv thread when connect is success.
		bool Connect(IP ip,WORD port,bool startRecv = 0);
		//connect a server,startRecv indicate whether auto starup recv thread when connect is success.
		bool ConnectAsyn(IP ip,WORD port,bool startRecv = 0);
		//start recv thread,some socket is only use for send data,so not nessary call this function.
		bool StartRecv();
		//start recv thread,some socket is only use for send data,so not nessary call this function.
		bool StartRecvAsyn();
		//equal with Attatch(s,false).
		void operator = (SOCKET s);
		//add data to send buffer
		bool AddSendData(const void* data,int len,UINT offset = -1); 
		//send data in send buffer,if no data in buffer,remote point always receive a call of datalen is zero.
		//zip is indicate whether compress data with zip before send data.
		//yes or no this function success,sendbuf always be cleared.
		int Send(bool zip = 0);
		//�����첽�����̡߳�
		bool StartSendAsyn();
	};*/

	//�㲥��ʽ�������ݣ����������ӷ������ݣ���ҲҪʹ��һ���˿ڣ���������˿ڿ��Ժ���һ����������һ���˿ڣ����������������һ���˿ڣ��ڶ������û�ʧ�ܣ���
	class CONCISE_API  UDPSender : public _class
	{
	protected:
		Socket _Handle;
	public:
		~UDPSender(){_Handle.Close();}
		bool Close(){return _Handle.Close();}
		//���ɹ㲥�����
		bool Create();
		//���͹㲥���ݣ�ip��ָ�����͵�IP�������ΪINADDR_BROADCAST��������������㲥��port��Ҫʹ�õĶ˿ڡ�
		int SendTo(char* data,int dLen,ushort port,IP ip = INADDR_BROADCAST);
		//������������ǰѱ��� UDP Socket ��Ŀ���ַ�Ͷ˿�����Ϊָ����ֵ��֮����Ե���Send�����������ݣ���
		//��ÿ�ζ�ָ��Ŀ���ַ��
		bool SetDest(ushort port,IP ip = INADDR_BROADCAST);
		//�����Ѿ�����SetDest�������ſ��Ե������������
		int Send(char* data,int len);
		//����socket
		Socket* GetSocket(){return &_Handle;}
	};
	//���չ㲥���ݣ��������������ݣ��˿ڿ���ʹ���Ѿ�ʹ�õĶ˿ڡ������Ӻ���������������ͬ����ϵ�����ǵĶ˿�Ҳ�Ǹ���ʹ�õġ�
	class CONCISE_API  UDPReceiver : public _class
	{
	protected:
		Socket _Handle;
		Thread _thread;

		void _Receive();
	public:
		//���ջص�����������:���ݣ����ݳ��ȣ������ͻ�����Ϣ��sockaddr_inָ�롣
		Function<void,char*,int,SOCKADDR&> OnRecv;

		~UDPReceiver();
		//��ʼ����ָ���˿ڵĹ㲥��
		bool Start(ushort port);
		//�첽��ʼ����ָ���˿ڵĹ㲥��
		bool StartAsyn(ushort port);
		//�رս��ա�
		void Close();
		//����socket
		Socket* GetSocket(){return &_Handle;}
	};
	//���ڼ������������ſͻ��˺ͷ����������ӡ���ʹ��������رգ��Ѿ�������������Ȼ���á����� Scoket ֻ���𴴽����ӣ�����������������Ҳ���������ݡ�
	/*class CONCISE_API  TCPServer : public _class{
	protected:
		Socket	_Handle;
		Thread _thread;
		void _Accept();
	public:
		//���ͻ�������������ʱ������Ӧ�Ļص�������
		Function<void,SOCKET> OnAccept;			

		~TCPServer();
		//���ذ󶨵� Socket ָ�롣
		Socket* GetSocket(){return &_Handle;}
		//���ɷ���˼�������ʵ�������Ҫ�����κ�IP�����ӣ�������ipΪINADDR_ANY��
		bool Starup(ushort port,IP ip = INADDR_ANY);
		//�رշ���ˡ����������������������������ӣ�ֱ�����ʵ�����٣�������������Ҳ�����١�
		void Close();
	};
	//TCPTransfer���͵����ݰ���4���ֽڵ�����ͷ��������TCPTransfer����������
	class CONCISE_API  TCPTransfer : public _class{
	protected:
		Socket		handle;
		Memory<char>	sendBuf;
		void*				userParam;
		Thread		recvThread;
		bool				connected;

		void onRecv();
		void connect(SOCKADDR sa,bool startRecv);
	public:
		Function<void,void*,int,void*> OnRecv;//call back function of recv data
		Function<void,void*> OnClose;			//call back function be called when a socket be closed and have startup a recv thread,this function be call in a Asynchronous thread. 
		Function<void,bool,void*> OnConnect;
		TCPTransfer();
		~TCPTransfer();
		//���ذ󶨵� Socket ָ�롣
		Socket* GetSocket(){return &handle;}
		//attach a socket,cn is indicate this socket whether connected.
		void Attach(SOCKET s,bool cn);
		//detach a socket from this instance
		SOCKET Detach();
		//accept a socket from server accept call back function,this is equal to Attach(s,true).
		void Accept(SOCKET s);
		//close this transfer,this don't call in any call back function(OnClose,OnRecv,OnConnect).
		void Close();
		//�ڴ�ʵ�����κλص��������������Close���������γ���������������������Ҫ�رմ�Socket������CloseSocket�����ر�Socket�����
		void CloseSocket(){handle.Close();}
		//set user data,the last param of OnClose and OnRecv.
		void SetUserParam(void* param);
		//connect a server,startRecv indicate whether auto starup recv thread when connect is success.
		bool Connect(IP ip,WORD port,bool startRecv = 0,bool asyn = 0);
		//start recv thread,some socket is only use for send data,so not nessary call this function.
		bool StartRecv();
		//equal with Attatch(s,false).
		void operator = (SOCKET s);
		//add data to send buffer
		bool AddSendData(const void* data,int len,UINT offset = -1); 
		//send data in send buffer,if no data in buffer,remote point always receive a call of datalen is zero.
		//zip is indicate whether compress data with zip before send data.
		//yes or no this function success,sendbuf always be cleared.
		int Send(bool zip = 0);
	};

	//�㲥��ʽ�������ݣ����������ӷ������ݣ���ҲҪʹ��һ���˿ڣ���������˿ڿ��Ժ���һ����������һ���˿ڣ����������������һ���˿ڣ��ڶ������û�ʧ�ܣ���
	class CONCISE_API  UDPSender : public _class
	{
	protected:
		Socket _Handle;
	public:
		~UDPSender(){_Handle.Close();}
		bool Close(){return _Handle.Close();}
		//���ɹ㲥�����
		bool Create();
		//���͹㲥���ݣ�ip��ָ�����͵�IP�������ΪINADDR_BROADCAST��������������㲥��port��Ҫʹ�õĶ˿ڡ�
		int SendTo(char* data,int dLen,ushort port,IP ip = INADDR_BROADCAST);
		//������������ǰѱ��� UDP Socket ��Ŀ���ַ�Ͷ˿�����Ϊָ����ֵ��֮����Ե���Send�����������ݣ���
		//��ÿ�ζ�ָ��Ŀ���ַ��
		bool SetDest(ushort port,IP ip = INADDR_BROADCAST);
		//�����Ѿ�����SetDest�������ſ��Ե������������
		int Send(char* data,int len);
		//����socket
		Socket* GetSocket(){return &_Handle;}
	};
	//���չ㲥���ݣ��������������ݣ��˿ڿ���ʹ���Ѿ�ʹ�õĶ˿ڡ������Ӻ���������������ͬ����ϵ�����ǵĶ˿�Ҳ�Ǹ���ʹ�õġ�
	class CONCISE_API  UDPReceiver : public _class
	{
	protected:
		Socket _Handle;
		Thread _thread;
		void _Receive();
	public:
		//���ջص�����������:���ݣ����ݳ��ȣ������ͻ�����Ϣ��sockaddr_inָ�롣
		Function<void,char*,int,SOCKADDR&> OnRecv;

		~UDPReceiver();
		//��ʼ����ָ���˿ڵĹ㲥��
		bool Start(ushort port);
		//�رս��ա�
		void Close();
		//����socket
		Socket* GetSocket(){return &_Handle;}
	};*/
}