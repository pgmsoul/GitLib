function Color(r, g, b, a) {
	if (r !== undefined) this.red = r;
	else this.red = 0;
	if (g !== undefined) this.green = g;
	else this.green = 0;
	if (b !== undefined) this.blue = b;
	else this.blue = 0;
	if (a !== undefined) this.alpha = a;
	else this.alpha = 0;
}
Color.prototype.toString = function() {
	return "Color: " + this.red + "," + this.green + "," + this.blue + "," + this.alpha;
}
Color.prototype.getIntValue = function() {
	var v = this.red & 0xFF;
	v += ((this.green & 0xFF) << 8);
	v += ((this.blue & 0xFF) << 16);
	v += ((this.alpha & 0xFF) << 24);
	return v;
}
Color.prototype.setIntValue = function(v) {
	this.red = v & 0xFF;
	this.green = (v >> 8) & 0xFF;
	this.blue = (v >> 16) & 0xFF;
	this.alpha = (v >> 24) & 0xFF;
}

function TextDraw() {
	this._dt = 8400;
	this.tabstop = 8;
	this.color = 0;
	this.font = null;
	this.hdc = 0;
}
TextDraw.prototype.reset = function () {
	this._dt = 8400;
	this.tabstop = 8;
	this.color = 0;
	this.font = null;
}
TextDraw.prototype.setSingleLine = function (singleline) {
	this._dt |= 0x20;
	if(!singleline) this._dt -= 0x20;
}
TextDraw.prototype.setWordBreak = function (wordbreak) {
	this._dt |= 0x10;
	if(!wordbreak) this._dt -= 0x10;
}
TextDraw.prototype.setEditControl = function (editcontrol) {
	this._dt |= 0x2000;
	if(!editcontrol) this._dt -= 0x2000;
}
TextDraw.prototype.setNoClip = function (noclip) {
	this._dt |= 0x100;
	if(!noclip) this._dt -= 0x100;
}
TextDraw.prototype.setEndEllipsis = function (endellipsis) {
	this._dt |= 0x8000;
	if(!endellipsis) this._dt -= 0x8000;
}
TextDraw.prototype.setNoPrefix = function (noprefix) {
	this._dt |= 0x800;
	if(!noprefix) this._dt -= 0x800;
}
TextDraw.prototype.setAlignLeft = function (left) {
	this._dt |= 0x3;
	if(!left) this._dt -= 0x3;
}
TextDraw.prototype.setAlignCenter = function (center) {
	this._dt |= 0x3;
	if(!center) this._dt -= 0x2;
}
TextDraw.prototype.setAlignRight = function (right) {
	this._dt |= 0x3;
	if(!right) this._dt -= 0x1;
}
TextDraw.prototype.setAlignTop = function (top) {
	this._dt |= 0xc;
	if(!top) this._dt -= c;
}
TextDraw.prototype.setAlignVCenter = function (vcenter) {
	this._dt |= 0xc;
	if(!vcenter) this._dt -= 0x8;
}
TextDraw.prototype.setAlignBottom = function (bottom) {
	this._dt |= 0xc;
	if(!bottom) this._dt -= 0x4;
}
