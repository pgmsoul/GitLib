#pragma once
namespace cs{
	//���������ȡ���ļ�������ͼ��.����ļ�������,����һ��ȱʡ�ļ�ͼ��.������ļ���Ϊ�մ�,�����ļ��е�ͼ��.
	//��һ���ļ�����˵,�������һ��ͼ��,�򷵻����ͼ��,��֮����һ���ļ���ͼ��.�Կ�ִ���ļ�(����DLL,OCX��),
	//��������ִ���ļ�����ͼ��,�򷵻����ͼ��,������indexΪ����(С��0ʱ����0������ͼ��);��֮����һ����ִ
	//���ļ���ȱʡͼ��;�Է�EXE�Ŀ�ִ���ļ�,indexΪ-1ʱ,��������ͼ��.����ͼ���ļ�,��indexΪ0ʱ,��������ļ�
	//������ͼ,indexΪ-1ʱ,��������ͼ��.Ҳ����˵,����EXE��ָ��ͼ����ļ���,ֻҪindex��-1,�򷵻��ļ�����ͼ��;
	CONCISE_API HICON GetFileIcon(LPCWSTR file,int index = 0);
	//����һ���ļ���ͼ��,iconfile������һ��ͼ���ļ����ִ���ļ�,iconindex���ļ���ͼ�������,toolinfo����ʾ��Ϣ.
	//���������ƽ̨��ص�,������XP,����֤�����������汾�Ĳ���ϵͳ.
	CONCISE_API bool SetFolderIcon(LPCWSTR folder,LPCWSTR iconfile,LPCWSTR iconindex = 0,LPCWSTR toolinfo = 0);
	//�����ļ����ԣ�add:�����Խ�����ӣ�del�������Խ����Ƴ���
	CONCISE_API bool SetFileAttr(LPCWSTR file,DWORD add,DWORD del);
	//ֱ�������ļ����ԡ�
	CONCISE_API inline bool SetFileAttr(LPCWSTR file,DWORD attr){return 0!=SetFileAttributes(file,attr);}

	//�����ļ�ʹ�õ����ݽṹ
	typedef struct CONCISE_API FileSearchData : public _struct{
		WIN32_FIND_DATA*	FileInfo;		//�ļ���������Ժ���Ϣ��
		LPCWSTR				Original;		//�ļ���·�������·��ĩβ��������б�ܡ�
		LPCWSTR				Relative;		//���·�������·��ĩβ�Ϳ�ͷ��������б�ܡ�
		//����������־�����ڻص�����ʱ��ֹ������������
		bool				ExitSearch;		//�Ƿ�ȡ�������ı�ʶ��
		bool				SkipThisFolder;	//���FileSearch������SubFolderPre��־��SearchSubFolder��־��
		//���������ļ���ǰ���ص��ᱻ����һ�Σ������ʱ���������ʶΪ1����������������ļ��С�
		//��ǰ���Ƿ���һ���ļ��С�
		inline bool			IsDir(){
			return (FileInfo->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0;
		}
		FileSearchData();
		FileSearchData(WIN32_FIND_DATA* wfd,LPCWSTR ori,LPCWSTR rel);
	}FileSearchData;
	class CONCISE_API FolderWatch : public _class{
	public:
		enum WatchType{
			FileName = FILE_NOTIFY_CHANGE_FILE_NAME,
			FolderName = FILE_NOTIFY_CHANGE_DIR_NAME,
			Atrributes = FILE_NOTIFY_CHANGE_ATTRIBUTES,
			Size = FILE_NOTIFY_CHANGE_SIZE,
			LastWrite = FILE_NOTIFY_CHANGE_LAST_WRITE,
			LastAccess = FILE_NOTIFY_CHANGE_LAST_ACCESS,
			Creation = FILE_NOTIFY_CHANGE_CREATION,
			Security = FILE_NOTIFY_CHANGE_SECURITY,
			Normal = FileName|FolderName|LastWrite|Creation
		};
		enum FileAction{
			Added = FILE_ACTION_ADDED,		// 1
			Removed = FILE_ACTION_REMOVED,	//2
			Modifyed = FILE_ACTION_MODIFIED,//3
			RenameOldName = FILE_ACTION_RENAMED_OLD_NAME,	//4
			RenameNewName = FILE_ACTION_RENAMED_NEW_NAME	//5   
		};
	protected:
		HANDLE					_compPortHandle;
		HANDLE					_handle;
		String			_name;
		Memory<char>		_buffer;
		DWORD					_type;	//�������ͣ�
		bool					_sub;	//�Ƿ�������ļ��У�
	public:
		Function<void,DWORD,LPCWSTR,FolderWatch*>	OnChange;	//�����ļ�ϵͳ��Ϣ�Ļص�����.
		Function<void,FolderWatch*>			OnStop;		//ֹͣ����֪ͨ���첽����ʱ��������������Ա�󶨵ĺ�����ɾ����ǰ����
		void*										UserData;	
		FolderWatch();
		LPCWSTR GetWatchPath(){
			if(_name.IsNull()) return 0;
			return _name;
		}
		DWORD GetWatchType(){
			return _type;
		}
		bool GetWatchSub(){
			return _sub;
		}
		//�رռ��ӣ���ΪStart������һ����������������ֱ������������������᷵�أ��������������Start�����������߳�����á�
		//����������͹ر���Ϣ������ȴ��رս����Ĳ�����֪��Start���أ�����Żᱻ��ȫ�رա�
		bool Close();
		//�������ӻ�����
		bool Create(LPCWSTR folder,bool subfolder = true,DWORD type = Normal);
		//���Ǹ����������������ϣ����������һ���µ��̵߳��á�
		bool Start();
		~FolderWatch(){
			Close();
		}
	};
	class CONCISE_API MultiFolderWatch : public _class{
	protected:
		ObjectList<FolderWatch> _watchList;
		TaskPool<bool>			_taskEngine;
		//_taskEngine����_watchList�ĺ��棬���Ա�֤����_watchList�����٣�����_taskEngine������Ҫ�ȴ������߳��˳�
		//�����������Stop�������������̣߳���������ã���һֱ�ȴ���
		CriticalSection	_csList;

		void _onChange(DWORD action,LPCWSTR fn,FolderWatch* pfw);
		void _onStop(FolderWatch* pfw);
	public:
		//�����ļ�ϵͳ��Ϣ�Ļص�����������ص������Ƕ��߳��첽���С�
		Function<void,DWORD,LPCWSTR,FolderWatch*> OnChange;
		//��ʼ����һ���ļ��У�ÿ�ε���Start������һ�����߳��
		bool Start(LPCWSTR folder,void* userData = 0,bool subfolder = true,DWORD type = FolderWatch::Normal);
		//ֹͣ����һ���ļ��У����Ƕ�Ӧ���̲߳�һ��������
		bool Stop(LPCWSTR folder);
		//ֹͣ���������ļ��У��ر������̣߳����ҵȴ���Щ�߳̽������Ż᷵�ء�
		void Close();
		~MultiFolderWatch();
	};

}