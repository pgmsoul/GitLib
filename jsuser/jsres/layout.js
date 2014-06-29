function LayoutParam() {
    this.margin = new Rect(0,0,0,0);
    this.weight = 1;
    this.style = 0;
    this.length = 0;
}
LayoutParam.prototype.setVertical = function () {
    this.style |= 1;
    this.style -= 1;
}
LayoutParam.prototype.setHorizontal = function () {
    this.style |= 1;
}
LayoutParam.prototype.isHorizontal = function () {
    return (this.style & 1)==1;
}
LayoutParam.prototype.isVertical = function () {
    return (this.style & 1)==0;
}
LayoutParam.prototype.reset = function () {
    this.margin.setValue(0,0,0,0);
    this.length = 0;
    this.weight = 1;
	this.style = 0;
}

function Layout(){
	this._location = new Point();
	this._size = new Size();
    this.param = new LayoutParam();
	this.name = "";
	this.control = null;
	this.childs = [];
}
Layout.prototype._setRect = function(left,top,width,height){
	this._location.x = left;
	this._location.y = top;
	this._size.x = width;
	this._size.y = height;
	if(this.control instanceof CWnd){
		this.control.setPosition(left,top);
		this.control.setSize(width,height);
	}
	if(this.param.isHorizontal())
		this._setChildHor(left,top,width,height);
	else
		this._setChildVer(left,top,width,height);
}
Layout.prototype._weightIsZero = function(weight){
	return ((weight<0.0001)&&(weight>-0.0001));
}
Layout.prototype.getByName = function(name){
	if(this.name===name) return this;
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var lay = this.childs[i].getByName(name);
		if(lay) return lay;
	}
	return undefined;
}
Layout.prototype._setChildHor = function(left,top,width,height){
	var sumWeight = 0;
	var sumDim = 0;
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var lp = this.childs[i].param;
		sumWeight += lp.weight;
		sumDim += lp.length + lp.margin.left + lp.margin.right;
	}
	var rest = width - sumDim;
	var x = left;
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var lay = this.childs[i];
		var lp = lay.param;
		var rationW;
		if(this._weightIsZero(sumWeight))
			rationW = lp.length;
		else
			rationW = lp.length + rest*lp.weight/sumWeight;
		x += lp.margin.left;
		lay._setRect(Math.floor(x),top+lp.margin.top,Math.floor(x+rationW)-Math.floor(x),height-lp.margin.top-lp.margin.bottom);
		x += rationW + lp.margin.right;
	}
	return sumDim;
}
Layout.prototype._setChildVer = function(left,top,width,height){
	var sumWeight = 0;
	var sumDim = 0;
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var lp = this.childs[i].param;
		sumWeight += lp.weight;
		sumDim += lp.length + lp.margin.top + lp.margin.bottom;
	}
	var rest = height - sumDim;
	var y = top;
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var lay = this.childs[i];
		var lp = lay.param;
		var rationH;
		if(this._weightIsZero(sumWeight))
			rationH = lp.length;
		else
			rationH = lp.length + rest*lp.weight/sumWeight;
		y += lp.margin.top;
		lay._setRect(left+lp.margin.left,Math.floor(y),width-lp.margin.left-lp.margin.right,Math.floor(y+rationH)-Math.floor(y)/*消除舍1导致的一个像素的间距*/);
		y += rationH + lp.margin.bottom;
	}
	return sumDim;
}
Layout.prototype._calcMinSize = function(lay,parentHor){
	var sz = new Size(0,0);
	if(parentHor){
		sz.x = lay.param.length;
	}else{
		sz.y = lay.param.length;
	}
	var ms = new Size(0,0);
	var count = lay.childs.length;
	for(var i=0;i<count;i++){
		var child = lay.childs[i];
		var childSz = this._calcMinSize(child,lay.param.isHorizontal());
		ms.x += childSz.x;
		ms.y += childSz.y;
	}
	if(sz.x<ms.x) sz.x = ms.x;
	if(sz.y<ms.y) sz.y = ms.y;

	sz.x += lay.param.margin.left + lay.param.margin.right;
	sz.y += lay.param.margin.top + lay.param.margin.bottom;
	return sz;
}
Layout.prototype.ptInLayout = function(x,y){
	var rect = new Rect(this._location.x,this._location.y,this._location.x+this._size.x,this._location.y+this._size.y);
	return rect.ptInRect(x,y);
}
Layout.prototype.reset = function(){
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		var child = this.childs[i];
		child.reset();
	}
	this.param.reset();
	if(this.control instanceof CWnd){
		this.control.dispose();
	}
	this.control = null;
	this.name = "";
	this.childs = [];
}
Layout.prototype.parent = function(){
	return _parent;
}
Layout.prototype.setControl = function(className,id,text,style,exstyle){
	if(className){
		if(this.control){
			this.control.close();
		}else{
			this.control = this._createCtrlObject(className);
		}
		this.control.className = className;
		this.control.text = text;
		this.control.id = id;
		this.control.style = style;
		this.control.exStyle = exStyle;
	}else if(this.control){
		this.control.dispose();
		this.control = null;
	}
}
Layout.prototype._createControlTree = function(hwnd){
	if(!hwnd) return;
	if(this.control){
		this._createControl(hwnd);
	}
	var count = this.childs.length;
	for(var i=0;i<count;i++){
		this.childs[i]._createControlTree(hwnd);
	}
}
Layout.prototype.refresh = function(hwnd){
	if(hwnd){
		this._createControlTree(hwnd);
	}
	var _parent = this._parent;
	if(_parent){
		_parent._setRect(_parent._location.x,_parent._location.y,_parent._size.x,_parent._size.y);
	}else{
		this._setRect(this._location.x,this._location.y,this._size.x,this._size.y);
	}
}
Layout.prototype.move = function(pix){
	if(!this._parent) return 0;
	var index = this._parent.childs.indexOf(this);
	if(index==0||index==(this._parent.childs.length-1)){
		//最前面和最后面的 Layout 无法调整位置。
		return 0;
	}

	var preLay = this._parent.childs[index-1];
	var nextLay = this._parent.childs[index+1];
	var dx1 = preLay.Param.Length + pix;
	var dx2 = nextLay.Param.Length - pix;
	var dx = Math.min(dx1,dx2);
	if(dx<0) pix += dx;

	if(pix==0) return 0;
	if(pix<0&&preLay.param.length<=0) return 0;
	if(pix>0&&nextLay.param.length<=0) return 0;

	preLay.param.length += pix;
	nextLay.param.length -= pix;
	this._parent._setRect(this._parent._location.x,this._parent._location.y,this._parent._size.x,this._parent._size.y);
	return pix;
}
Layout.prototype.add = function(index){
	var lay = new Layout();
	lay._parent = this;
	if(isNaN(index)) index = this.childs.length;
	this.childs.splice(index,0,lay);
	return lay;
}
Layout.prototype.del = function(index){
	if(isNaN(index)) return undefined;
	if(index>=this.childs.length||index!=-1) return undefined;
	var lay = this.childs.splice(index,1);
	if(!lay) return undefined;
	lay = lay[0];
	lay.reset();
	return true;
}
Layout.prototype.remove = function(index){
	if(isNaN(index)) return undefined;
	if(index>=this.childs.length||index!=-1) return undefined;
	return this.childs.splice(index,1);
}
Layout.prototype.addin = function(lay,index){
	lay._parent = this;
	if(isNaN(index)||index<0||index>this.childs.length) index = this.childs.length;
	this.childs.splice(index,0,lay);
	return lay;
}
Layout.prototype.toJson = function(){
	var json = {};
	json.name = this.name;
	json.style = this.param.style;
	json.weight = this.param.weight;
	json.length = this.param.length;
	json.margin_left = this.param.margin.left;
	json.margin_top = this.param.margin.top;
	json.margin_right = this.param.margin.right;
	json.margin_bottom = this.param.margin.bottom;
	if(this.control){
		var ctrl = this.control;
		json.ctrl_class = ctrl.className;
		json.ctrl_text = ctrl.text;
		json.ctrl_style = ctrl.style;
		json.ctrl_exstyle = ctrl.exStyle;
		json.ctrl_id = ctrl.id;
	}
	var count = this.childs.length;
	if(count>0) json.childs = [];
	for(var i=0;i<count;i++){
		var childjson = this.childs[i].toJson();
		json.childs.push(childjson);
	}
	return json;
}
Layout.prototype.parseJson = function(json){
	this.reset();
	var param = this.param;
	param.style = json.style;
	param.weight = json.weight;
	param.length = json.length;
	param.margin.setValue(json.margin_left,json.margin_top,json.margin_right,json.margin_bottom);
	if(typeof json.ctrl_class == "string"){
		this.control = this._createCtrlObject(json.ctrl_class);
		var cp = this.control;
		cp.text = json.ctrl_text;
		cp.style = json.ctrl_style;
		cp.exStyle = json.ctrl_exstyle;
		cp.id = json.id;
	}
	this.name = json.name;
	if(json.childs instanceof Array){
		var count = json.childs.length;
		for(var i=0;i<count;i++){
			var child = new Layout();
			child.parseJson(json.childs[i]);
			this.addin(child);
		}
	}
}
Layout.prototype.load = function(jsonfile,code){
	var file = new CFile();
	if(!file.create(jsonfile,true)) return false;
	if(!code) code = 65001;
	var jstr = file.readString(code);
	file.dispose();
	var json = JSON.parse(jstr);
	if(!json) return false;
	this.parseJson(json);
	return true;
}
Layout.prototype.save = function(jsonfile,code){
	var file = new CFile();
	if(!file.create(jsonfile)) return false;
	file.length = 0;
	var json = this.toJson();
	var str = JSON.stringify(json,undefined,'\t');
	if(!code) code = 65001;
	file.writeString(str,code);
	file.dispose();
	return true;
}




















