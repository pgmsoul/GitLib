namespace cs{
	class CONCISE_API Console : public _class{
	protected:
		HANDLE _hRead,_hWrite;
	public:
		Console();
		bool Show();
		inline bool IsShowed(){
			return _hRead!=0;
		}
		void Close();
		inline void Write(LPCWSTR str){
			DWORD len;
			WriteConsole(_hWrite,str,WcsLength(str),&len,NULL);
		}
		inline void Write(LPCSTR str){
			DWORD len;
			WriteConsoleA(_hWrite,str,MbsByteLen(str),&len,NULL);
		}
		void SetCloseMenuDisable(bool disable);
		//�����ı���ɫ, ����ָ̨֧�� 8(0--7) ����ɫ, Ҳ��������3����ɫ�Ļ����.
		void SetTextColor(int color);
		void WriteLn(LPCWSTR str);
		//����̨��ȡ�����ڻس�����, Ҳ�����������������, ֪���ڿ���̨���˻س�, 
		//���buf̫С, ��Ҫ��ȡ���ַ�����buffer, �������������������, ֪������ֵ��ĩβ��һ���س���һ������.
		DWORD ReadLn(String& str);
		inline ~Console(){
			Close();
		}
	};
	CONCISE_API Console* GetConsole();
}