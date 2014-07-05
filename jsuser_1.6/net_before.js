
function PortMapping(){
	this.port = 0;			//外部端口
	this.localPort = 0;		//内部端口
	this.tcp = true;		//协议类型（两种TCP或者UDP）
	this.ip = "";			//映射的主机IP，可能机器名称也可以。
	this.name = "jsuser network";	//描述。
	this.enabled = true;			//启用还是禁用，默认是启用。
}