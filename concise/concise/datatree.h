#pragma once
namespace cs{
	/*DataTree�����ݸ�ʽ�ǳ��򵥣�������3�����ݲ��֣�Name��Value��Node��Name��Value��ǰ2��
	�ֽ������ݵĳ��ȣ����������ݣ�Node���֣�ǰ�����ֽ���Node�ĸ�����ÿ��NodeҲ����Name��
	Value��Node 3���֡�*/
	class CONCISE_API DataTree : public TPTree<DataTree>
	{
	protected:
	public:
		Memory<uchar> Name;
		Memory<uchar> Value;
		//ObjectList<DataTree> Node;
		//�Ѷ���洢���ڴ��У�mem��Ҫ�洢���ڴ棬offset���洢���ڴ��е�λ�ã�����ʹ�õ��ڴ��С
		int ToMemory(Memory<uchar>& mem,int offset = 0);
		//��һ���ڴ���أ�len���ڴ�Ĵ�С������ֵ����ȷ�����ĳ��ȣ����������ȷ����Ӧ�÷���len��
		int FromMemory(uchar* mem,int len);
		bool Save(LPCWSTR name);
		bool Load(LPCWSTR name,bool always = 1);
		DataTree* OpenNode(void* name,int len,bool always = 0);
		bool SetValue(void* data,int len);
		bool SetName(void* name,int len);
	};
	/*��DataTree����Ϊ�ض����ļ��࣬���ṩ��Ψһ���ܾ�����һ��Ĭ���ļ�����
	�����ָ������ļ���������ָ��Ϊ NULL ���߿մ�����Ĭ������Ϊexe�ļ���+.dtf��׺*/
	class CONCISE_API DataTreeFile : public DataTree{
	protected:
		String fileName;
	public:
		DataTreeFile();
		//ָ���ļ�������� name = NULL ����һ���մ�����fileName������Ϊ���õ�exe�ļ���+.dtf��׺
		void SetFileName(LPCWSTR name);
		//��ȡ��ǰ�ļ�����
		LPCWSTR GetFileName(){return fileName;}
		//���ļ�����
		bool Load();
		//����Ϊ�ļ�
		bool Save();
	};
}