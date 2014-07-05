"use strict";

(function() {
	alert.ERROR = 0x10;
	alert.ASK = 0x20;
	alert.WARN = 0x30;
	alert.INFO = 0x40;
	alert.OK = 0x0;
	alert.OK_CANCEL = 0x1;
	alert.ABORT_RETRY_IGNORE = 0x2;
	alert.YES_NO_CANCEL = 0x3;
	alert.YES_NO = 0x4;
	alert.RETRY_CANCEL = 0x5;
	alert.CANCEL_TRY_CONTINUE = 0x6;
	alert.DEF_BUTTON1 = 0x000;
	alert.DEF_BUTTON2 = 0x100;
	alert.DEF_BUTTON3 = 0x200;
	alert.OK = 1;
	alert.CANCEL = 2;
	alert.ABORT = 3;
	alert.RETRY = 4;
	alert.IGNORE = 5;
	alert.YES = 6;
	alert.NO = 7;
	alert.CLOSE = 8;
	alert.HELP = 9;
	alert.TRYAGAIN = 10;
	alert.CONTINUE = 11;
})();
var __stack__ = [];
Stack.create = function() {
	var a = [];
	__stack__.push(a);
}
Stack.close = function() {
	var a = __stack__.pop();
	if (!a) return;
	var i = a.length - 1;
	for (; i >= 0; i--) {
		var obj = a[i];
		if(typeof obj.dispose == "function") obj.dispose();
	}
}
Stack.push = function(obj) {
	if (__stack__.length === 0) {
		alert("使用C对象必须先初始化Stack, 先调用Stack.create()函数, 函数退出时调用Stack.close()函数, Stack.create和Stack.close必须成对调用.");
	}
	var a = __stack__[__stack__.length - 1];
	a.push(obj);
}
Stack.pop = function(obj) {
	if(__stack__.length<2) return false;
	var index = __stack__.length - 1;
	var a = __stack__[index];
	for (var i = a.length - 1; i >= 0; i--) {
		if (a[i] === obj) {
			__stack__[index-1].push(a.splice(i, 1));
			return true;
		}
	}
	return false;
}
Stack.dispose = function(obj) {
	if (__stack__.length === 0) {
		alert("使用C对象必须先初始化Stack, 先调用Stack.create()函数, 函数退出时调用Stack.close()函数, Stack.create和Stack.close必须成对调用.");
	}
	var a = __stack__[__stack__.length - 1];
	for(var i=a.length-1;i>=0;i--){
		if(a[i]==obj){
			a.splice(i,1);
			return true;
		}
	}
	return false;
}
Stack.create();

function Class(func,isGlobal) {
	var f = function() {
		Stack.create();
		var r = func.apply(this, arguments);
		Stack.close();
		return r;
	}
	if (isGlobal)
	Global[func.name] = f;
	return f;
}
function Point(x, y) {
	this.x = x;
	this.y = y;
}
Point.prototype.toString = function() {
	return "Point/Size: " + this.x + "," + this.y;
}
Point.prototype.move = function(dx, dy) {
	this.x += dx;
	this.y += dy;
}
Point.prototype.setValue = function(x, y) {
	this.x = x;
	this.y = y;
}
var Size = Point;
function Rect(left, top, right, bottom) {
	this.left = left;
	this.top = top;
	this.right = right;
	this.bottom = bottom;
}
Rect.prototype.toString = function() {
	return "Rect: " + this.left + "," + this.top + "," + this.right + "," + this.bottom;
}
Rect.prototype.ptInRect = function(x, y) {
	if (y === undefined) {
		if (this.left > x.x)
			return false;
		if (this.right <= x.x)
			return false;
		if (this.top > x.y)
			return false;
		if (this.bottom <= x.y)
			return false;
		return true;
	}
	if (this.left > x)
		return false;
	if (this.right <= x)
		return false;
	if (this.top > y)
		return false;
	if (this.bottom <= y)
		return false;
	return true;
}
Rect.prototype.getLocation = function() {
	return new Point(this.left, this.top);
}
Rect.prototype.setLocation = function(x, y) {
	if (x !== undefined) {
		this.right = this.right - this.left + x;
		this.left = x;
	}
	if (y !== undefined) {
		this.bottom = this.bottom - this.top + y;
		this.top = y;
	}
}
Rect.prototype.getSize = function() {
	return new Point(this.right - this.left, this.bottom - this.top);
}
Rect.prototype.setSize = function(x, y) {
	if (x !== undefined) {
		this.right = this.left + x;
	}
	if (y !== undefined) {
		this.bottom = this.top + y;
	}
}
Rect.prototype.positive = function() {
	var v;
	if (this.right < this.left) {
		v = this.left;
		this.left = this.right;
		this.right = v;
	}
	if (this.bottom < this.top) {
		v = this.top;
		this.top = this.bottom;
		this.bottom = v;
	}
}
Rect.prototype.setValue = function(left, top, right, bottom) {
	this.left = left;
	this.top = top;
	this.right = right;
	this.bottom = bottom;
}
Rect.prototype.width = function () {
	return this.right - this.left;
}
Rect.prototype.height = function () {
	return this.bottom - this.top;
}
