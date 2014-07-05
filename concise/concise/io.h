#pragma once
namespace cs{
	//�ж�һ��·��������,0��·�������ڣ�1����һ���ļ���2����һ���ļ��С�
	CONCISE_API int GetFileType(LPCWSTR file);
	//��Ӧ�Ƿ���Ŀ¼���ƣ�ϵͳ�� GetFileAttributes ���ܷ���һ�������ֵ�������������˼��Ŀ¼�Ϸ��ԡ�
	CONCISE_API DWORD GetFileAttributes(LPCWSTR path);
	//����һ���ļ��У�file=1��ʾpath��һ���ļ������������ļ��У�����path���ļ��еĵ�·����
	CONCISE_API bool CreateFolder(LPCWSTR path,bool file = 0);
	//ɾ��һ���ļ�����ļ��С�
	CONCISE_API bool RemoveFile(LPCWSTR file);
	//ת��Ϊȫ·����name �����ļ���������׺��name �� path ��ĳ��λ�á�
	CONCISE_API bool FPToFullPath(String& path,LPCWSTR* name = 0);
	//�Ƚ������ִ��Ƿ��ʾ��ͬ��·�������ǲ���֤�ִ��Ƿ�Ϸ���·�����ơ�
	CONCISE_API bool FPIsSame(LPCWSTR path1,LPCWSTR path2);
	//ȡ���ִ���·����ĩβû��б�ܻ�б�ܡ�����ִ�������һ��·������ȡ����һ��Ŀ¼�����û����һ��Ŀ¼�����ִ����仯��
	CONCISE_API void FPToParent(String& ps);
	//��б��ת��Ϊ��б�ܡ�
	CONCISE_API void FPToBackSlash(String& ps);
	//�ѷ�б��ת��Ϊб�ܡ�
	CONCISE_API void FPToSlash(String& ps);
	//ȡ�ú�׺�����������ַ�"."��
	CONCISE_API void FPToExt(String& ps);
	//ȡ���ļ��ı��⣨��������׺�����ļ����������ع�����������ĩβ�ġ�.����������б�ܻ�б�ܣ���֮����ִ�������title�С�
	CONCISE_API void FPToTitle(String& ps);
	//ȡ���ļ���(�ļ�����ͺ�׺)��
	CONCISE_API void FPToFileName(String& ps);
	//ȥ��ĩβ��б�ܻ�б�ܡ�
	CONCISE_API void FPDelTailSlash(String& ps);
	//����ĩβ��һ��б�ܻ�б�ܣ���ʾ�ִ���һ��Ŀ¼��
	CONCISE_API void FPSetTailSlash(String& ps);
	//���������ִ����м䱣��һ����.���š�
	CONCISE_API void FPLinkExt(String& ps,LPCWSTR ext);
	//���������ִ����м䱣��һ����б�ܡ�
	CONCISE_API void FPLinkPath(String& ps,LPCWSTR path);
	typedef struct DRIVEINFO
	{
		wchar_t Name[65],Path[3],FileSystemType[6];
		DWORD Type,Flag,MaxPath;
	}DRIVEINFO,*LPDRIVEINFO;
	class CONCISE_API Drives
	{
	public:
		Drives();
		~Drives();
		bool GetInfo(int index,LPDRIVEINFO lpDriveInfo);
		bool GetInfo(LPCWSTR path,LPDRIVEINFO lpDriveInfo);
		int Count(){return _Count;}
	protected:
		int _Count;
		StringMemList Paths;
	};
	typedef struct ListData : public _struct
	{
		WIN32_FIND_DATA*	FindData;		//�ļ���������Ժ���Ϣ��
		LPCWSTR				Original;		//ԭʼ·��, ���������Ĳ���.
		LPCWSTR				Relative;		//���·�������·������һ��ĩβ�ķ�б�ܡ�
		WORD				Flag;			//�����Աָʾ���з�ʽ��������ļ��У���_Pre��ʽ���У���ֵΪ1��_After��ʽ���д�ֵΪ2���ļ�ʱ����ֵΪ0.
		bool				Stop;			//�Ƿ�ȡ�������ı�ʶ��
		bool				Skip;			//��PreCallʱ���ֵ�����Ϊ true ��������������ļ��е��ļ��������������ļ�����
		void*				ExtraData;		//�û�������Ҫ���ݶ�������ݡ�
	}ListData;
	//
	//FileTree����ö��һ��Ŀ¼�������ļ�,���������༶���ļ���.OnSearch��һ���ص�����,ÿ������һ���ļ�(���ļ���),�������
	//�ᱻ����.�ļ����оٵĴ���;�������й�,һ��NTFS������CDFS��������ĸ˳���о�,��FAT32�������ļ��ڴ����ϵĽṹ�й�.
	//��������ѡ�subָʾ�Ƿ��������ļ��У�dirָʾ�Ƿ�����ļ��У�dirΪ0ʱ���������ļ��У������sub=1���ļ����е��ļ���Ȼ�ᱻ��������
	//preCallָʾ�������ļ���ʱ���ȷ����ļ����������������ݣ�afterCallָʾ����
	//���ļ����ǣ��������������ļ����ڷ������������������ΪTRUE�����������ļ���ʱ�᷵�����Ρ������Ϊ0���򲻷����ļ��С�
	//����dirΪ 0��sub ��Ϊ 0 �����Σ���ִ�д洢����ʱ����Ϊ���ļ��е��ļ���Ҫ�����������֧�£����ԣ��ļ�����Ȼ�ᱻ���档
	//FileTree�����첽ִ�ж��������OnList �� OnCompete ��������ָ����������Ӱ���Ѿ�ִ�е����������ǣ��ı������������ѡ�
	//��Ӱ������ִ�е�������
	//Search����������dir���������һ����ͨ������ִ���������һ���ļ��������᷵�ط���ͨ������ļ�����),�����ļ����������һ��Ŀ¼����ִ�е�������Ŀ¼��
	class CONCISE_API FileTree : public _class
	{
	protected:
		Thread th;
		StringMemList _Filter;
		bool _Sub,_Pre,_After;

		Tree<String>* _Store(Tree<String>* files,LPCWSTR file,bool sub = 0);
		bool _search(LPCWSTR dir,void* extra = 0);
		bool _Search(LPCWSTR original,LPCWSTR relative,void* extra = 0);
		bool _searchStore(LPCWSTR dir,Tree<String>* tl);
		bool _SearchStore(LPCWSTR original,LPCWSTR relative,Tree<String>* tl);
	public:
		//�����ص�������
		Function<void,ListData*> OnList;
		//�첽����ʱ�������߳̽���֪ͨ������ͬ�����ò���������������void* ������ʱָ���� extra �� tl ʵ�������ָ�룬LPCWSTR ���������ļ��С�
		Function<void,void*,LPCWSTR> OnComplete;
		//Function<void,void*,LPCWSTR,bool> OnStart;

		FileTree();
		//�����ļ����͹�����������һ���ַ����б���Ա���ļ���׺������������.���͡�*���ַ��硰jpg��������".jpg"��"*.jpg"����֧��ͨ����������ִ�Сд��
		StringMemList* GetFilter(){return &_Filter;}
		//�����Ƿ��������ļ���
		void SetSearchSub(bool enable){_Sub = enable;}
		//���ûص������ȷ���һ�����ļ��к��У��ٷ������������ļ���
		void SetPreCall(bool enable){_Pre = enable;}
		//���ûص������������������ļ�֮�󣬲�ִ��һ�η����ļ��к��С�
		void SetAfterCall(bool enable){_After = enable;}
		//dir��Ҫ������Ŀ¼,extra���û����õ����ݣ������ڻص������н���.asynָʾ�����Ƿ����첽�ġ����dir��������һ���ļ��У�����һ���ļ�����������fasle��
		bool Search(LPCWSTR dir,void* extra = 0,bool asyn = 0);
		//dir��Ҫ������Ŀ¼,tl�����ڴ洢�ļ������������ݽṹ.asynָʾ�����Ƿ����첽�ġ�
		bool Search(LPCWSTR dir,Tree<String>* tl,bool asyn = 0);
	};
	//FilsSystem Copy ,Delete, Move �������ܵķ���ֵ��
#define FS_OK						0	//�����ɹ�
#define FS_FALSE					1	//BOOL���� ���� FALSE��
#define FS_SAME_NAME				2	//Ŀ���ļ���Դ�ļ�ͬ��
#define FS_OPEN_SOURCE_FAILED		3	//��Դ�ļ�ʧ��
#define FS_OPEN_DEST_FAILED			4	//��Ŀ���ļ�ʧ��
#define FS_FILE_COPYED				5	//�ļ��Ѿ���֮ǰ������ɡ�
#define FS_COPY_PAUSE				6	//���������ж�
#define FS_CANNOT_ACCESS			7	//�ļ������޷����á�
#define FS_DEL_FAILED				8	//ɾ���ļ�����ʧ��
#define FS_MOVE_FAILED				9	//�ļ��ƶ�ʧ��
#define FS_MOVE_COPY_FAILED			10	//�ƶ�����ʱ�ļ�����ʧ��
#define FS_MAX						10
	//�ļ�ϵͳ�ṩ�ļ��Ļ���������
	class CONCISE_API FileSystem : public _class{
	protected:
		String _name;
	public:
		void* UserData;
		FileSystem():UserData(0){}
		//��һ���ļ�����ʼ��ʵ����
		FileSystem(LPCWSTR name);
		//��һ���ļ�����ʼ��ʵ��������ļ������ڷ��� 0.
		bool Initialize(LPCWSTR name);
		//�����ļ���ȫ�ơ�
		LPCWSTR GetName(){return _name;}
		//�Ƿ�Ϊ�ա�
		bool IsNull(){return _name.IsNull();}
		//�Ƿ�Ϊ�ա�
		bool NotNull(){return _name.NotNull();}
		//�����ļ���check = 1: ���Ŀ���ļ��Ĵ�С��д��ʱ�䣬��κ�Դ�ļ���ͬ��ִ�����������ݿ��������Һ������ز����ɹ���־1.
		//�������check = 1���Ҽ��Ŀ���ļ������ڻ��ߴ�С��д��ʱ���Դ�ļ���ͬ���򿽱����ݣ��������֮�󣬻������ļ������д��ʱ���Դ�ļ���ͬ��
		//Copy���������������ݣ�����ȡ���������´�ִ��ͬһ�ļ�ʱ�����ݿ������������Ƿ�������ֹ�Ĳ����ǲ����������ġ�
		HRESULT Copy(LPCWSTR newName,bool check = 1);
		//ɾ���ļ���
		HRESULT Delete();
		//�ƶ��ļ���ͬ�������ƶ����ؿ������ݣ���ͬ�������ǽ���� Copy �� Delete ������
		HRESULT Move(LPCWSTR newName);
		//�ļ���������ʱ�Ľ��Ȼص������������ֱ�Ϊ���Ѿ����������ݣ������ܳ����Ƿ�ֹͣ�������������true��
		//�������ݵĲ�����ֹͣ������֮��ĵ��ÿ������������Դ����û�иı�Ļ���ÿ����1Mb�����ݣ�������������һ�Ρ�
		Function<void,uint64,uint64,bool&,void*> OnProgress;
	};
	//��ȡ�����������
	LPCWSTR CONCISE_API FSGetErrorMsg(uint fs_error);
	//�ļ��в���ʹ�õĽṹ
	typedef struct DirData{
		bool Stop;			//�Ƿ�ֹͣ����������
		bool Skip;		//�Ƿ�ȡ����ǰ�ļ��Ĳ�����
		uchar Error;			//�������صĴ��룬FS_OK��0����ʾ�ɹ�
		uchar PreOrAfter;	//�Ե�ǰ�ļ�����һ�ε��ã�0��������ǰ�ĵ��ã���ʱ��������ȡ��������1����������ĵ��ã���ʱ�ɹ���־��Ч��
		LPCWSTR SourceDir;	//��ǰ������Դ�ļ������ļ��У��ĸ�Ŀ¼��
		LPCWSTR DestDir;	//��ǰ������Ŀ���ļ������ļ��У��ĸ�Ŀ¼��
		WIN32_FIND_DATA* FindData;	//�ļ���Ϣ�ṹ�������ļ�����Ϣ��
	}DirData;
	class CONCISE_API  DSParam : public _class{
	public:
		void* UserData;
		String Source,Destinate;
		//�����ļ��������Ȼص�������
		Function<void,uint64,uint64,bool&,void*> OnFileProgress;
		//�ļ������ص�������ÿ����һ���ļ������ļ��У������������2�Σ���һ���ǲ���ǰ��
		//��������ȡ��������ǰ�ļ����ڶ��η����ǲ���֮��Success ��Աָʾ��ǰ�ļ��Ĳ����Ƿ�ɹ���
		Function<void,DirData*,void*> OnProgress;
		//���в����Ѿ���ɣ�LPCWSTR ָʾԴĿ¼,LPCWSTR ָʾĿ��Ŀ¼���� Delete ������Ϊ 0 ��
		Function<void,LPCWSTR,LPCWSTR,void*> OnComplete;
		DSParam():UserData(0){}
	};
	//�ļ��в���
	class CONCISE_API DirectorySystem : public _class{
	protected:
		static void __stdcall _Copy(ListData* ld);
		static void __stdcall _Move(ListData* ld);
		static void __stdcall _Delete(ListData* ld);
	public:
		//�����ļ�������ΪԴĿ¼��Ŀ��Ŀ¼���������ļ���
		static bool Copy(DSParam* dsp);
		//ɾ���ļ��С�
		static bool Delete(DSParam* dsp);
		//�ƶ��ļ��С�
		static bool Move(DSParam* dsp);
	};
	//class FileWatch
	class CONCISE_API FileWatch : public _class
	{
		IntegerList<HANDLE> _hdir;
		StringMemDiction	_folder;
		VirtualAddress _buffer;
		HANDLE _CompPortHandle;
		DWORD _Type;	//�������ͣ�
		bool _Sub;		//�Ƿ�������ļ��У�
		Thread	_thread;
		void _watch(); 
	public:
	//FileSystemʹ�õĽṹ��
	enum WatchType{
		FileName = FILE_NOTIFY_CHANGE_FILE_NAME,
		DirName = FILE_NOTIFY_CHANGE_DIR_NAME,
		Atrributes = FILE_NOTIFY_CHANGE_ATTRIBUTES,
		Size = FILE_NOTIFY_CHANGE_SIZE,
		LastWrite = FILE_NOTIFY_CHANGE_LAST_WRITE,
		LastAccess = FILE_NOTIFY_CHANGE_LAST_ACCESS,
		Creation = FILE_NOTIFY_CHANGE_CREATION,
		Security = FILE_NOTIFY_CHANGE_SECURITY
	};
		FileWatch();
		~FileWatch();
		//���ü�������.
		void SetWatchType(DWORD type){_Type = type;}
		//�����Ƿ�������ļ���.
		void SetWatchSub(bool sub){_Sub = sub;};
		//�������ӣ�������ֹͣ�������ٴ����������򷵻�fasle��
		bool Start();
		//ֹͣ����.
		bool Stop();
		//���Ҫ���ӵ��ļ��У�����ļ����ڼ����б��Ѿ����ڣ����� false ���������������ļ����Ƿ���Ч����������Ѿ�����,��ӵ��ļ������´�����������������.
		bool AddFolder(LPCWSTR folder);
		//�Ƴ����ӵ��ļ��У��´�������Ч��
		bool DelFolder(LPCWSTR folder);

		Function<void,DWORD,LPCWSTR> OnChange;//�����ļ�ϵͳ��Ϣ�Ļص�����.
	};
	//class File File�༯�ɻ������ļ���д����.
	class CONCISE_API File : public HandleType<HANDLE>
	{
	public:
		//����
		File();
		//��һ���ļ�������ʼ������ļ�.
		File(LPCWSTR file);
		//����
		~File();
		//�ж��Ƿ���һ����Ч���ļ����.
		bool IsNull(){return _Handle==INVALID_HANDLE_VALUE;}
		//ʵ���Ƿ����һ����Ч���ļ����.
		bool NotNull(){return _Handle!=INVALID_HANDLE_VALUE;}
		//��һ����֪���ļ����.
		bool Attach(HANDLE file);
		//���һ���Ѿ��󶨵��ļ����.
		HANDLE Detach();
		//���ɻ��һ���ļ�.
		bool Create(LPCWSTR name,DWORD create = OPEN_ALWAYS,DWORD share = FILE_SHARE_READ|FILE_SHARE_WRITE,DWORD access = FILE_GENERIC_READ|FILE_GENERIC_WRITE,DWORD attribute = FILE_ATTRIBUTE_NORMAL);
		//�ر��ļ�.
		bool Close();
		//��ȡ�ļ��Ĵ�С.
		unsigned __int64 GetLength();
		//�����ļ��ĳ���(���ܴ���4G).
		bool SetLength(uint64 length);
		//���õ�ǰָ��λ��Ϊ�ļ���β.
		bool EndOfFile(){return SetEndOfFile(_Handle)!=0;}
		//������д���ļ�,����ʵ��д��ĳ���.
		int Write(const void* handle,int length);
		//���ļ���ȡ��Ӧ����(length)������,����ʵ�ʶ�ȡ�ĳ���.
		int Read(void* handle,int length);
		//������д���ļ�,����ʵ��д��ĳ���.
		template<typename T> int Write(Memory<T>* buf){
			return Write(buf->Handle(),buf->Length()*sizeof(T))/sizeof(T);
		}
		//���ļ���ȡ��Ӧ����(length)������,����ʵ�ʶ�ȡ�ĳ���.
		template<typename T> int Read(Memory<T>* buf){
			uint flen = GetFileSize(_Handle,0);
			uint pos = (uint)GetPointer();
			if(pos>=flen) return 0;
			uint buflen = flen - pos;
			uint count = buflen/sizeof(T);
			if(buf->Length()<count)
				if(!buf->SetLength(count)) return 0;
			return Read(buf->Handle(),buflen)/sizeof(T);
		}
		//�����ļ���ָ��λ��.
		bool SetPointer(uint64 offset,DWORD seek = FILE_BEGIN);
		//�����ļ���ָ��λ��.-1��ʾʧ��,����ʱ���ֵҲ��һ����Чֵ.
		uint64 GetPointer();
		//�ѻ����е�����д�����.
		bool Flush();
	};
	//class FileStream
	class CONCISE_API FileStream : public File
	{
	public:
		bool Attach(HANDLE hFile);
		
		//��ָ��λ�ö�ȡ�� code ���뱣����ı�(����β)��buffer�����ʼ����code = 0 ��ʾ����ʹ�� Unicode ��ʽ���棬�������κα���ת����-1 ��ʾ�ӵ�ǰλ�ÿ�ʼ��ȡ�����ض�ȡ���ַ�����
		int ReadString(String& buffer,int position = -1,DWORD code = -1);
		//���ı�д���ļ���position ��д���λ�ã�code �Ǵ洢ʹ�õı��룬Ϊ 0  ʱ��ʾֱ��ʹ�� Unicode ���뱣�棨�����б���ת������affix ��Ҫ����ķָ�����
		int WriteString(LPCWSTR buffer,int position = -1,LPCWSTR affix = 0,DWORD code = CP_THREAD_ACP);
		//��ָ��λ�ö�ȡ Unicode ��ʽ������ı�(����β)��buffer�����ʼ����code ��Ҫ��ȡ��Ҫת���ı���,code = 0 ��ʾ���ݲ������κα���ת����-1 ��ʾ�ӵ�ǰλ�ÿ�ʼ��ȡ�����ض�ȡ���ֽ�����
		int ReadMbsFromWs(Memory<char>& buffer,int position = -1,DWORD code = CP_THREAD_ACP);
		//���ı�ת��Ϊ Unicode ���ݲ�д���ļ���position ��д���λ�ã�code �� buffer ʹ�õı��룬Ϊ 0 ʱ��ʾ������ֱ��д���ļ��������б���ת������affix ��Ҫ����ķָ�����
		int WriteMbsToWs(LPCSTR buffer,int position = -1,LPCSTR affix = 0,DWORD code = 0);
		//code = 0,���ı�ʹ���ִ��ı���д���ļ�,code != 0 ���ı���Unicode����д���ļ�,code��buffer�ִ��ı���.
		//���Ϻ�������ֵΪ��ȡ��д����ֽ���.
		int ReadInt(int position = -1,int defValue = 0);
		bool WriteInt(int value,int position = -1);

		char ReadChar(int position = -1,char defValue = 0);
		bool WriteChar(char value,int position = -1);
	};
	/*�ļ�ӳ���漰��3���ļ�����������CreateFile���ļ���CreateFileMap����ӳ��MapViewOfFileӳ�䵽�����ڴ档���ڴ��ļ���MapViewOfFile
	�����������Ϊ�ڴ治�㵼�µ�ʧ�ܡ����ڴ򿪷�ʽ���ļ���һ���漰 GENERIC_READ�����ļ���, GENERIC_WRITE��д�ļ���, �� GENERIC_EXECUTE
	��ִ���ļ������ַ��ע�⣬�ļ���ֻд��ʽ��ʱ���ǲ��ܽ��ж�ȡ�ġ�
	CreateFileMapping�����򿪷�ʽ��PAGE_READONLY��ֻ����ʽ��PAGE_READWRITE����д��ʽ��PAGE_WRITECOPY��д��������д���ļ�������ӳ���ǿɶ�д�ģ�ֻ��Ҫ GENERIC_READ Ȩ�ޣ�
	PAGE_EXECUTE_READ���ɶ�ȡ��ִ�У��ļ��������� GENERIC_READ �� GENERIC_EXECUTE Ȩ�ޣ�PAGE_EXECUTE_READWRITE������ִ�кͶ�д���ļ���ʱ
	������ GENERIC_READ, GENERIC_WRITE, �� GENERIC_EXECUTEȨ�ޡ���������ʽֻ�� XP SP2 �� Server 2003 SP1 ֮��Ĳ���ϵͳ�п��á�
	MapViewOfFile���������ӳ���ļ�ӳ�䵽��ַ�ռ䣬FILE_MAP_READ����ȡȨ�ޣ�FILE_MAP_WRITE��д��Ȩ�ޣ�FILE_MAP_ALL_ACCESS����дȨ�ޣ�ʵ����
	���� FILE_MAP_READ | FILE_MAP_WRITE����֪��Ϊʲô���� FILE_MAP_READWRITE��FILE_MAP_EXECUTE��ִ��Ȩ�ޣ�FILE_MAP_COPY����ӦPAGE_WRITECOPY
	�����Ρ�*/
	//class FileMap
//FILE_MAP_READ		//�ڴ���ֻ����
//FILE_MAP_WRITE	//�ڴ��ǿɶ�д��
//FILE_MAP_EXECUTE	//���ڴ��ǿ�ִ�еģ����Զ��߱��ɶ����ԣ����ǿ��Ժ�MapWrite���ʹ�ã��߱���д���ԡ�
//FILE_MAP_COPY		//�ڴ��ǿɶ�д�ģ�����д�����ݲ�ˢ�µ��ļ���ʵ�����ļ���ӳ������ֻ����ʽ�򿪵ġ�
	class CONCISE_API FileMap : public HandleType<HANDLE>{
	protected:
	public:
		FileMap();
		~FileMap();
		bool Create(HANDLE file,DWORD flag = FILE_MAP_WRITE,uint64 size = 0);
		void Close();
	};
	class CONCISE_API MapView : public HandleType<void*>{
	protected:
		uint _length;
	public:
		MapView();
		~MapView();
		void Close();
		//ƫ����offset ������64K��64��1024 = 0x10000���ı�������������ʧ�ܡ�ʵ����lengthҲ�����������Ϊ��λ�ģ���Ȼָ���ĳ��Ⱥ��٣�д����Ҳû�����⡣
		bool Create(HANDLE map,uint64 offset,uint length,DWORD access = FILE_MAP_WRITE|FILE_MAP_READ);
		bool Flush(uint start = 0,uint length = -1);
		inline uint Length(){return _length;}
	};
}