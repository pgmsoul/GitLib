"use strict";

function Message(hWnd,msg,wParam,lParam){
	this.hWnd = hWnd;
	this.msg = msg;
	this.lParam = wParam;
	this.wParam = lParam;
}

function Mouse(x,y,button,event,wheel) {
	this.x = x;
	this.y = y;
	this.event = event;
	this.button = button;
	this.wheel = wheel;
}
Mouse.prototype.isMove = function(){
	return this.event===0x200;
}
Mouse.prototype.isLUp = function(){
	return this.event===0x202;
}
Mouse.prototype.isLDown = function(){
	return this.event===0x201;
}
Mouse.prototype.isLDBClick = function(){
	return this.event===0x203;
}
Mouse.prototype.isRUp = function(){
	return this.event===0x205;
}
Mouse.prototype.isRDown = function(){
	return this.event===0x204;
}
Mouse.prototype.isRDBClick = function(){
	return this.event===0x206;
}
Mouse.prototype.MUp = function(){
	return this.event===0x208;
}
Mouse.prototype.isMDown = function(){
	return this.event===0x207;
}
Mouse.prototype.isMDBClick = function(){
	return this.event===0x209;
}
Mouse.prototype.XUp = function(){
	return this.event===0x20C;
}
Mouse.prototype.isXDown = function(){
	return this.event===0x20B;
}
Mouse.prototype.isXDBClick = function(){
	return this.event===0x20D;
}
Mouse.prototype.isWheel = function(){
	return this.event===0x20A;
}
Mouse.prototype.isLeave = function(){
	return this.event===0x2A3;
}
Mouse.prototype.isLButton = function(){
	return this.event===0x1;
}
Mouse.prototype.isRButton = function(){
	return this.event===0x2;
}
Mouse.prototype.isMButton = function(){
	return this.event===0x10;
}
Mouse.prototype.isX1Button = function(){
	return this.event===0x20;
}
Mouse.prototype.isX2Button = function(){
	return this.event===0x40;
}
