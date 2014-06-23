#pragma once
namespace cs{
	/*-------------------------------------------------------------------------------------------------------------
	SystemTime��һ����ʶʱ��Ľṹ,���ĳ�Ա�ֱ������,��,����,��,ʱ,��,��,����.ʱ��ֱ�׼ʱ��͵���ʱ��,�������һ��
	ʱ��.GetLocalTime,GetSystemTime���������ֱ�ȡ��������ʱ��.FileTime��һ����ʶʱ��ĳ���ṹ.������,ʱ����64���ֽ�
	��ʾ�Ѿ��㹻,��SystemTime��128���ֽ�,����Ϊ�˷ֽ������ϰ�ߵ���ʽ,�����������.�ļ�ʱ���ʶ����FileTime�ṹ������
	,���Ҫȡ�����Ŀɶ���ʽ,��Ҫ�õ�FileTimeToLocalFileTime��FileTimeToSystemTime����.��Ȼֻ�б�׼ʱ�������������ʱ
	��,����ȴ���������ǵ�ʱ��ϰ��,�����ճ���ʱ���ǵ���ʱ��,ͬһ��׼ʱ��,�ڲ�ͬʱ�����õļ������,����ʱ���ǲ�ͬ��.��
	������FileTimeToLocalFileTime��ϵͳʱ��ת��Ϊ����ʱ��,����FileTimeToSystemTime,�ѳ���ʱ��ת��Ϊ�ɶ�ʱ��.��Ȼ,Ҳ��
	��,ֱ����FileTimeToSystemTime��ʱ��ת��Ϊ�ɶ�ʱ��,����SystemTimeToTzSpecificLocalTime�������ʱ��.
	---------------------------------------------------------------------------------------------------------------*/
#pragma pack(push,1)
	//FileTime��ʱ���64λ������ʾ��
	typedef struct CONCISE_API FileTime : public _struct{
		//��2��ʱ��Ĳ�.���ֵ��1/10000����1����.
		void operator -= (FileTime& t){
			Value -= t.Value;
		}
		//���������ʱ��ֵ.
		void operator = (SYSTEMTIME& st){
			SystemTimeToFileTime(&st,(LPFILETIME)this);
		}
		void operator = (FILETIME& ft){
			memcpy(this,&ft,sizeof(ft));
		}
		operator FILETIME& (){
			return (*(LPFILETIME)this);
		}
		operator LPFILETIME (){
			return (LPFILETIME)this;
		}
		//ȡ�õ�ǰ�ı�׼ʱ��ֵ,���ֵ��������κμ������,ͬһʱ��Ӧ������ͬ��.
		void GetCurrentTime(){
			::GetSystemTimeAsFileTime((LPFILETIME)this);
		}
		union{
			/*HighValue ����36ΪСʱ��,LowValue����10000000Ϊ����(��ȷ������,Ҳ����˵,�����ֵ�㶨Ϊ0).���ֵ����ʱ��������ǵ�����*/
			struct{
				DWORD LowValue;
				DWORD HighValue;
			};
			struct{
				unsigned __int64 Value;//ʱ���Ǵ�1601��1��1�տ�ʼ��.
			};
		};
	}FileTime;
	//SystemTime��ʱ��ķ�����ʾ
	typedef struct CONCISE_API SystemTime : public _struct{
		WORD Year; 
		WORD Month;
		WORD DayOfWeek; //����,���������ֵ��0,����һ��1,....
		WORD Day; 
		WORD Hour;
		WORD Minute;
		WORD Second; 
		WORD Milliseconds;

		void operator = (SYSTEMTIME& st){
			*(LPSYSTEMTIME)this = st;
		}
		operator SYSTEMTIME& (){
			return *(LPSYSTEMTIME)this;
		}
		operator LPSYSTEMTIME (){
			return (LPSYSTEMTIME)this;
		}
		void operator = (FILETIME& ft){
			::FileTimeToSystemTime((LPFILETIME)&ft,(LPSYSTEMTIME)this);
		}
		//ȡ�ñ�׼ʱ�䡣
		inline void GetStandardTime(){
			::GetSystemTime((LPSYSTEMTIME)this);
		}
		//����ϵͳʱ�䣨��Ϊ��׼ʱ����
		inline bool SetStandardTime(){
			return ::SetSystemTime((LPSYSTEMTIME)this)!=0;
		}
		//ȡ�õ���ʱ�䡣
		inline void GetLocalTime(){
			::GetLocalTime((LPSYSTEMTIME)this);
		}
		//����ϵͳʱ�䣨��Ϊ����ʱ����
		inline bool SetLocalTime(){
			return ::SetLocalTime((LPSYSTEMTIME)this)!=0;
		}
		//ת��Ϊ��׼ʱ��
		bool ToStandard(){
			TIME_ZONE_INFORMATION tzi;
			if(::GetTimeZoneInformation(&tzi)==TIME_ZONE_ID_INVALID) return false;
			return ::TzSpecificLocalTimeToSystemTime(&tzi,(LPSYSTEMTIME)this,(LPSYSTEMTIME)this)!=0; 
		}
		//ת��Ϊ����ʱ��
		bool ToLocal(){
			TIME_ZONE_INFORMATION tzi;
			if(::GetTimeZoneInformation(&tzi)==TIME_ZONE_ID_INVALID) return false;
			return ::SystemTimeToTzSpecificLocalTime(&tzi,(LPSYSTEMTIME)this,(LPSYSTEMTIME)this)!=0;
		}
	}SystemTime;
	//������̵İ���
	typedef union CONCISE_API KeyState{
		struct{
			bool Toggled		//ÿһ�����������л�(����CapsLock,NumLock��Щ��),�����Ա��ʾ��������״̬.true��ʾ״̬ON.
				,Pressing;		//������Ƿ����ڱ�����.
		};
		struct{
			short State;		//�����״̬��һ��ֵ.
		};
		//��������Ӵ��ֵ���Ϣ������ȡ����Ϣ.
		void GetKeyState(int vk){
			State = ::GetKeyState(vk);
		}
		//���������Ӳ���ж�ȡ����Ϣ.
		void GetAsyncState(int vk){
			State = ::GetAsyncKeyState(vk);
		}
		void* operator new(size_t size){
			return MemoryAlloc((int)size);
		}
		void operator delete(void* p){
			MemoryFree(p);
		}
	}KeyState;
#pragma pack(pop)
}