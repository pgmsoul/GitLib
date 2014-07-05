"use strict";
//检测参数，只有二进制数据参数才会返回成功。
function parseMemParamAsBin(data,len,errFlag){
	len = 0xffffffff&len;
	if(len<0) len = 0;
	if(data instanceof CMemory){
		if(len==0) len = data.length;
		if(len>data.length){
			throw name+",len exceed data.length:"+len + ">"+data.length;
			return false;
		}
		data = data.handle();
	}else if(data&0xffff0000||data==0){
		data = data&0xffffffff;
		if(data<0){
			throw name+",data not a available pointer:"+data;
			return false;
		}
	}else{
		throw errFlag + ",data must be a CMemory or a pointer";
		return false;
	}
	return {"data":data,"len":len};
}
//检测参数
function parseMemParam(data,len,name){
	var mem;
	len = 0xffffffff&len;
	if(len<0) len = 0;
	if(data instanceof CMemory){
		if(len==0) len = data.length;
		if(len>data.length){
			throw name+",len exceed data.length:"+len + ">"+data.length;
			return false;
		}
		data = data.handle();
	}else if(data&0xffff0000||data==0){
		data = data&0xffffffff;
		if(data<0){
			throw name+",data not a available pointer:"+data;
			return false;
		}
	}else{
		mem = new CMemory();
		len = mem.setString(data);
		data = mem.handle();
	}
	return {"data":data,"len":len,"mem":mem};
}
