
#include <concise/concise.h>
#include <stdio.h>
#include "../miniwget.h"
#include "../miniupnpc.h"
#include "../upnpcommands.h"
#include "../upnperrors.h"

#define MINIUPNP_STATICLIB
#define _CRT_SECURE_NO_WARNINGS

typedef struct PORTMAPPING : public cs::_struct{
	char ip[40];
	char port[6];
	char localPort[6];
	char protocol[4];
	char name[80];
	char enabled[6];
	PORTMAPPING(){
		Reset();
	}
	void Reset(){
		ip[0] = 0;
		port[0] = 0;
		localPort[0] = 0;
		protocol[0] = 0;
		name[0] = 0;
		enabled[0] = 0;
	}
}PORTMAPPING,*LPPORTMAPPING;
class Upnp : public cs::_class{
protected:
	UPNPDev*	_upnpDevice;
	UPNPUrls	_urls;// @UPNPUrls : �������ֵ��0��UPNPUrls��Ϊ��������
	IGDdatas	_igdDatas;// @IGDdatas : �������ֵ��0��IGDdatas��Ϊ��������
	char		_localIp[64];	/* my ip address on the LAN */
public:
	Upnp(){
		_upnpDevice = 0;
		_urls.controlURL = 0;
		_localIp[0] = 0;
		WSADATA wsd;
		::WSAStartup(MAKEWORD(2,2),&wsd);
	}
	~Upnp(){
		::WSACleanup();
		if(_urls.controlURL) FreeUPNPUrls(&_urls);
		if(_upnpDevice) freeUPNPDevlist(_upnpDevice);
	}
	bool Create(){
		const char * multicastif = 0;
		const char * minissdpdpath = 0;
		int error = 0;
		_upnpDevice = upnpDiscover(2000,multicastif,minissdpdpath,0,0,&error);
		if(_upnpDevice==0) return 0;
		int r = UPNP_GetValidIGD(_upnpDevice,&_urls,&_igdDatas,_localIp,sizeof(_localIp));
		return r==1;
	}
	bool GetPortMapping(PORTMAPPING& pm,int index){
		if(!_urls.controlURL) return 0;
		char iBuf[6];
		_snprintf(iBuf,6,"%d",index);
		char rHost[64] = {0};
		char duration[16] = {0};
		int r = UPNP_GetGenericPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,iBuf,pm.port, pm.ip, pm.localPort,pm.protocol, pm.name, pm.enabled,rHost, duration);
		return r==0;
	}
	uint GetAllPortMapping(cs::ObjectList<PORTMAPPING>& pmList){
		if(!_urls.controlURL) return 0;

		int i = 0;
		char index[6];
		char rHost[64];
		char duration[16];
		while(true){
			_snprintf(index, 6, "%d", i);
			PORTMAPPING* pm = new PORTMAPPING;
			rHost[0] = '\0'; 
			duration[0] = '\0';
			int r = UPNP_GetGenericPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,index,pm->port, pm->ip, pm->localPort,pm->protocol, pm->name, pm->enabled,rHost, duration);
			if(r){
				delete pm;
				break;
			}
			pmList.AddIn(pm);
			i++;
		}
		return pmList.Count();
	}
	bool GetExernalIp(char* extip40){
		if(!_urls.controlURL) return 0;
		int r = UPNP_GetExternalIPAddress(_urls.controlURL,_igdDatas.first.servicetype,extip40);
		return (r==UPNPCOMMAND_SUCCESS);
	}
	bool SetPortMapping(int eport,const char * description = 0,const char * proto = "TCP",bool enable = true,const char * iaddr = 0,int iport = 0){
		if(!_urls.controlURL) return 0;

		if(eport==0) return 0;

		char eBuf[6],iBuf[6];
		_snprintf(eBuf,6,"%d",eport);
		if(proto==0||strcmp(proto,"UDP")!=0)
			proto = "TCP";
		if(description==NULL)
			description = "jsuser network";
		if(iport==0)
			iport = eport;
		_snprintf(iBuf,6,"%d",iport);
		if(iaddr==NULL)
			iaddr = _localIp;
		const char* enableBuf = enable?"1":"0";

		int r = UPNP_AddPortMapping(_urls.controlURL,_igdDatas.first.servicetype,
				eBuf, iBuf, iaddr, description,
				proto, 0, 0,enableBuf);
		return r==UPNPCOMMAND_SUCCESS;
	}
	bool GetPortMapping(PORTMAPPING& pm,int eport,bool tcpOrUdp){
		if(!_urls.controlURL) return 0;
		if(eport==0) return 0;
		pm.Reset();
		_snprintf(pm.port,6,"%d",eport);
		if(tcpOrUdp) strcpy(pm.protocol,"TCP");
		else strcpy(pm.protocol,"UDP");

		int r = UPNP_GetSpecificPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,pm.port,pm.protocol,0,pm.ip,pm.localPort,pm.name,pm.enabled,0);
		return r==UPNPCOMMAND_SUCCESS;
	}
	bool RemovePortMapping(int eport,bool tcp = true){
		if(!_urls.controlURL) return 0;
		if(eport==0) return 0;

		char port[6],*proto;
		_snprintf(port,6,"%d",eport);

		if(tcp) proto = "TCP";
		else proto = "UDP";

		int r = UPNP_DeletePortMapping(_urls.controlURL,_igdDatas.first.servicetype,port, proto, 0);
		return r==UPNPCOMMAND_SUCCESS;
	}

};
static void DisplayInfos(struct UPNPUrls * urls,struct IGDdatas * data)
{
	char externalIPAddress[40];
	char connectionType[64];
	char status[64];
	char lastconnerr[64];
	unsigned int uptime;
	unsigned int brUp, brDown;
	time_t timenow, timestarted;
	int r;
	if(UPNP_GetConnectionTypeInfo(urls->controlURL,
		data->first.servicetype,
		connectionType) != UPNPCOMMAND_SUCCESS)
		cs::Print("GetConnectionTypeInfo failed.\n");
	else
		cs::Print("Connection Type : %s\n", connectionType);
	if(UPNP_GetStatusInfo(urls->controlURL, data->first.servicetype,
		status, &uptime, lastconnerr) != UPNPCOMMAND_SUCCESS)
		cs::Print("GetStatusInfo failed.\n");
	else
		cs::Print("Status : %s, uptime=%us, LastConnectionError : %s\n",
		status, uptime, lastconnerr);
	timenow = time(NULL);
	timestarted = timenow - uptime;
	cs::Print("  Time started : %s", ctime(&timestarted));
	if(UPNP_GetLinkLayerMaxBitRates(urls->controlURL_CIF, data->CIF.servicetype,
		&brDown, &brUp) != UPNPCOMMAND_SUCCESS) {
			cs::Print("GetLinkLayerMaxBitRates failed.\n");
	} else {
		cs::Print("MaxBitRateDown : %u bps", brDown);
		if(brDown >= 1000000) {
			cs::Print(" (%u.%u Mbps)", brDown / 1000000, (brDown / 100000) % 10);
		} else if(brDown >= 1000) {
			cs::Print(" (%u Kbps)", brDown / 1000);
		}
		cs::Print("   MaxBitRateUp %u bps", brUp);
		if(brUp >= 1000000) {
			cs::Print(" (%u.%u Mbps)", brUp / 1000000, (brUp / 100000) % 10);
		} else if(brUp >= 1000) {
			cs::Print(" (%u Kbps)", brUp / 1000);
		}
		cs::Print("\n");
	}
	r = UPNP_GetExternalIPAddress(urls->controlURL,
		data->first.servicetype,
		externalIPAddress);
	if(r != UPNPCOMMAND_SUCCESS) {
		cs::Print("GetExternalIPAddress failed. (errorcode=%d)\n", r);
	} else {
		cs::Print("ExternalIPAddress = %s\n", externalIPAddress);
	}
}

WINMAIN{
	Upnp upnp;
	upnp.Create();
	PORTMAPPING pm;
	int index = 0;
	while(upnp.GetPortMapping(pm,index)){
		cs::Print("%s %5s->%s:%-5s '%s'",pm.protocol,pm.port,pm.ip,pm.localPort,pm.name);
		index++;
	}
	upnp.SetPortMapping(889,0,"TCP",0);
	char ip[40];
	upnp.GetExernalIp(ip);
	cs::Print("extern ip:%s",ip);
	pm.Reset();
	upnp.GetPortMapping(pm,889,true);
	bool b = upnp.RemovePortMapping(889);
	//upnp.AddPortMapping(889,"test");
	// ��upnp�豸���в�ѯ��Ч���豸��
	// @UPNPDev  : �豸�б�
	// @result   : =0û��IGD�豸��=1��һ�����ӣ�=2���豸��û�����ӣ�=3��UPNP������ICG�豸
	//DisplayInfos(&urls, &igd);
	cs::ObjectList<PORTMAPPING> pmList;
	//uint n = ListRedirections(&urls,&igd,pmList);
	// Used when skipping the discovery process.
	/*UPNP_GetIGDFromUrl()
	GetUPNPUrls()


	// �ж�url�Ƿ�������
	// @struct UPNPUrls: ָ�����豸
	// @struct IGDdatas: �豸��������Ϣ
	// @result         : =1����������=0û������
	UPNPIGD_IsConnected(UPNPUrls*, IGDdatas);


	// �ͷ�url��Դ
	FreeUPNPUrls(UPNPUrls*);

	// ��װ��recv����
	ReceiveData()

		// ������IP,port��·��������һ��ӳ��
		// @controlURL : IGD�豸��·��
		// @servicetype: IGD�豸����
		// @extPort    : ӳ����������˿�
		// @inPort     : ����port�˿�
		// @inClient   : ����IP��ַ
		// @desc       : ������Ϣ��ͨ������ΪNULL
		// @proto      : �������˿�֧�ֵ�����TCP��UDP
		// @remoteHost : �ö˿����Ƶ�IP��ַ��ͨ������ΪNULL����ʾ����IP���ܷ���
		// @result     : =0ӳ��ɹ�����0ʧ��
		UPNP_AddPortMapping(char* controlURL, servicetype, extPort, inPort, inClient, desc, proto, remoteHost);


	// ɾ��ָ���豸��ָ��ӳ��Ķ˿�
	// @controlURL : ����IGD�豸��·��
	// @servicetype: ����IGD�豸����
	// @extPort    : ����ӳ����������˿�
	// @proto      : ����������˿�֧�ֵ�����TCP��UDP
	// @remoteHost : ����ö˿����Ƶ�IP��ַ��ͨ������ΪNULL����ʾ����IP���ܷ���
	// @result     : =0�ɹ�����0ʧ��
	UPNP_DeletePortMapping(controlURL, servicetype, extPort, proto, remoteHost);*/
	// �ͷ�upnp�豸����
	return 0;
};