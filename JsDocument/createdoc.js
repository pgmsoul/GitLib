"use strict"

require("base");

var kwClass = [];
var kwConst = [];
kwConst.push("JSON");
var kwFunction = [];
var kwProperty = [];
var keywords = [0,0,kwClass,kwFunction,kwProperty,kwConst];
function createKw(){
	load("createkw.jsuser");
	loadJsIner();
	formatKws();
	createTip();
	createColorKey();
}
Class(function main() {
	log.show();
	log.setFontSize(32);
	
	var jsuser = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsuser.doc");
	
	var jsbase = jsuser.member[0];
	var jswnd = jsuser.member[1];
	var jscrypt = jsuser.member[2];

	jsbase.member = createJsbaseDoc();
	jswnd.member = createJswinDoc();
	jscrypt.member = createJscryptDoc();
	
	parseJsonDoc(jsuser,[],"");
	//生成 Notepad++ 的个性化配置
	createKw();
	startMsgLoop();
	//str = JSON.stringify(json,null,"\t");
},true);
Class(function saveString(str,fn){
    var file = new CFile();
	file.create(fn);
	file.length = 0;
	file.writeString(str,3);
},true);
function createJscryptDoc(){
	var groups = [];
	var group = getFileDocJson("D:\\SoftProject\\GitLib\\jsuser\\cryptdoc.cpp");
	groups.push(group);
	return groups;
}
function createJsbaseDoc(){
	var groups = [];
	var group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jskernel.cpp");
	groups.push(group);
	group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsregistry.cpp");
	groups.push(group);
	group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jshttp.cpp");
	groups.push(group);
	group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsfile.cpp");
	groups.push(group);
	group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsgdi.cpp");
	groups.push(group);
	var base = getFileDocJson("D:/SoftProject/GitLib/jsuser/v8base.cpp");
	groups.push(base);
	var cproc = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsapicb.cpp");
	base.member.push(cproc);
	return groups;
}
function createJswinDoc(){
	var groups = [];
	var group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jswnd.cpp");
	groups.push(group);
	group = getFileDocJson("D:/SoftProject/GitLib/jsuser/jsdialog.cpp");
	groups.push(group);
	return groups;
}
function ascSort(a,b){
	a = a.name.toLowerCase();
	b = b.name.toLowerCase();
	if(a>b) return 1;
	else if(a==b) return 0;
	else return -1;
}
function getMultiFileDocJson(fns){
	var member = [];
	var count = fns.length;
	for(var i=0;i<count;i++){
		var filejson = getFileDocJson(fns[i]);
		member.push(filejson);
	}
	return member;
}
function getFileDocJson(fn){
    var str = readFile(fn);
    var data = getCommentBlock(str);
    for(var i=0;i<data.length;i++){
        data[i] = formatBlock(data[i]);
    }
    str = data.join("");
	var sf = FilePath.getTitle(fn);
	sf = "Debug\\" + sf + ".js";
	///用于调试////////////////
	saveString(str,sf);
	log(sf);

	return JSON.parse(str);
}
function formatNumber(n){
	if(n<10) return '0' + n;
	else return "" + n;
}
function getTimeStampString(){
	var date = new Date();
	return date.getFullYear() + "-" +
	formatNumber(date.getMonth() + 1) + "-" +
	formatNumber(date.getDate()) + " " +
	formatNumber(date.getHours()) + ":" +
	formatNumber(date.getMinutes()) + ":" +
	formatNumber(date.getSeconds());
}
Class(function readFile(fn,code) {
    var file = new CFile();
	file.create(fn,true,true);
    if(file.isNull()){
        alert("open failed");
        return;
    }
    return file.readString(code);
},true);
function formatBlock(str) {
	//选出的 doc 文本按行 split
    var lines = str.split("\n");
	//去掉每行的第一个 “//”，取后面的文本，并且去掉行尾的空字符。
    var reg = /\s*\/\/([\s\S]*)/g;
    for(var i=0;i<lines.length;i++){
        var line = lines[i].trimRight();
        reg.lastIndex = 0;
        var a = reg.exec(line);
		//空行会被去掉
        if(a==null) continue;
        lines[i] = replaceTab(a[1]);
    }
    return lines.join("\n");
}
function getCommentBlock(str) {
	//选出两个 //* 之间的文本。
	var reg = /\/\/\*([\s\S]*?)\/\/\*/g;
	var data = [];
	while (true) {
		var a = reg.exec(str);
		if (a == null) break;
		data.push(a[1].trim());
	}
	return data;
}
//JSON.parse 不能处理字串值含 tab 字符的情况，替换为 4 个空格。
function replaceTab(str){
	function getQuote(str,pos){
		while(true){
			pos = str.indexOf('\"',pos);
			if(pos==-1) return -1;
			if(pos>0&&str[pos-1]=='\\'){
				pos++;
				continue;
			}
			return pos;
		}
	}
	var pos1,pos2 = -1;
	var a = [];
	while(true){
		pos1 = getQuote(str,pos2+1);
		if(pos1==-1) break;
		pos1 += 1;
		pos2 = getQuote(str,pos1);
		if(pos2==-1) break;
		for(var i=pos1;i<pos2;i++){
			if(str[i]=='\t') a.push(i);
		}
	}
	var ns = "";
	for(var i=a.length-1;i>=0;i--){
		ns = str.substr(a[i]+1) + ns;
		if(a[i]>0)
			str = str.slice(0,a[i]);
		ns = "&nbsp;&nbsp;&nbsp;&nbsp;" + ns;
	}
	ns = str + ns;
	return ns;
}
function getPhpName(tree,nm){
	tree = tree.slice(0);
	if(nm&&nm.length) tree.push(nm);
	var fn = "";
	for(var i=0;i<tree.length;i++){
		var name = tree[i];
		var len = name.indexOf("(");
		if(len!=-1)
			name = name.substr(0,len);
		if(fn.length!=0) fn += "_";
		fn += name;
	}
	fn += ".php";
	return fn;
}
function getHtmlName(tree,nm){
	tree = tree.slice(0);
	if(nm&&nm.length) tree.push(nm);
	var fn = "";
	for(var i=0;i<tree.length;i++){
		var name = tree[i];
		var len = name.indexOf("(");
		if(len!=-1)
			name = name.substr(0,len);
		if(fn.length!=0) fn += "_";
		fn += name;
	}
	fn += ".html";
	return fn;
}
function parseJsonDoc(json,tree,comment,menu){
	if(!tree) tree = [];
	if(!comment) comment = "";
	if(!menu) menu = "";
	
	if(json.member){
		json.member.sort(ascSort);
	}
	tree = tree.slice(0);
	tree.push(json.name);
	var content = "";
	if(json.type=="library"){
		content = createContent(json,"类库",menu,tree);
	}else if(json.type=="const"){
		content = createContent(json,"常量",menu,tree);
		kwConst.push(json.name);
	}else if(json.type=="group"){
		content = createContent(json,"功能组",menu,tree);
	}else if(json.type=="variant"){
		content = createContent(json,"变量",menu,tree);
	}else if(json.type=="class"){
		content = createContent(json,"类",menu,tree);
		kwClass.push(json.name);
	}else if(json.type=="function"){
		content = createContent(json,"函数",menu,tree);
		kwFunction.push(json["return"].type+" "+json.name);
	}else if(json.type=="property"){
		content = createContent(json,"属性",menu,tree);
		kwProperty.push(json.name);
	}
	
	var html = createHtmlPage(json.name,content,comment,menu);
	return saveHtmlPage(getPhpName(tree),html);
}
function createMenuHtml(menu,tree,nm){
	var menuHtml = menu;
	if(menuHtml.length!=0) menuHtml += ' &gt;&gt; ';
	menuHtml += '<a href="' + getHtmlName(tree) + '">' + nm + '</a>';
	return menuHtml;
}
function createHtmlPage(title,content,comment,menuHtml){
	//var baseUrl = "http://jsuser.sinaapp.com/";
	var baseUrl = "<?PHP echo $baseUrl; ?>";
	return ['<?PHP include("../record_ip.php"); $baseUrl = "http://".$_SERVER["SERVER_NAME"]."/"; ?>',
	'<!DOCTYPE html>' ,
	'<html>' ,
    '<head>' ,
	'	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />' ,
    '	<link rel="stylesheet" type="text/css" href="./doc.css" />' ,
	'	<title>'+title+'</title>' ,
	'	<script type="text/javascript" src="prettify.js"></script>',
	'	<link type="text/css" rel="stylesheet" href="prettify.css"/>',
    '</head>' ,
    '<body onload="prettyPrint()">' ,
    ' 	<center>' ,
    '		<div id="header"><center>',
	'		<table style="width:90%;"><tr>',
	'			<td class="cla_logo">',
	'				<a class="cla_logo_j" href="'+baseUrl+'" title="首页">jsuser</a>&nbsp;&nbsp;',
	'				<a class="cla_logo_s" href="'+baseUrl+'document/jsuser.html" title="查阅jsuser文档">文档</a>&nbsp;&nbsp;',
	'				<a class="cla_logo_s" href="'+baseUrl+'sdk_tools.html" title="下载SDK">SDK Tools</a>&nbsp;&nbsp;',
	'				<a class="cla_logo_s" href="'+baseUrl+'app.html" title="应用">应用</a>&nbsp;&nbsp;',
	'				<a class="cla_logo_s" href="'+baseUrl+'connect_us.html" title="联系我们">联系我们</a>',
	'			</td>',
	'			<td class="cla_our"></td>',
	'		</tr></table></center></div>' ,
    ' 		<div id="container">' ,
    '			<div id="title">' + title + '</div>' ,
    '			<table width="100%">' ,
    '			<tr>' ,
	'				<td class="menu">' ,
    					menuHtml ,
    '				</td>' ,
    '				<td class="info">' + getTimeStampString(),
    '				</td>' ,
    '			</tr>' ,
    '			</table>' ,
    '			<table width="100%">' ,
    '			<tr>' ,
    '				<td class="comment">' ,
						comment,
    '				</td>' ,
    '				<td class="content">' ,
						content,
    '				</td>' ,
    '			</tr>' ,
    '			</table>' ,
    '		</div>' ,
    '		<div id="footer">Copyright <a href="http://www.jsuser.com">www.jsuser.com</a></div>' ,
    '	</center>' ,
    '</body>' ,
	'</html>'].join("\r\n");
}
Class(function saveHtmlPage(fn,html){
    var file = new CFile();
	fn = "D:\\wamp\\www\\jsuser\\3\\document\\" + fn;
	file.create(fn);
    file.length = 0;
    file.writeString(html,65001);
	return fn;
},true);
function createParam(doc,prop,name,content){
	if(doc[prop]){
		content.push('				<div><span class="classic">'+name+':</span>');
		for(var i=0;i<doc[prop].length;i++){
			content.push(createParamHtml(doc[prop][i])) ;
		}
		content.push('				</div>');
	}
}
function createContent(doc,typename,menu,tree){
	var content = [];
	if(doc.type){
		if(!typename) typename = doc.type;
		content.push('				<p><span class="classic">类型:</span> '+typename+'</p>');
	}
	if(doc.objtype){
		content.push('				<p><span class="classic">类型:</span> <span class="type">'+doc.objtype+'</span></p>');
	}
    if(doc.text){
		content.push('				<p><span class="classic">说明:</span> ' + doc.text + '</p>');
	}
	createParam(doc,"param","参数",content);
	createParam(doc,"property","属性",content);
	createParam(doc,"component","组件",content);
	if(doc.return){
		content.push('				<p><span class="classic">返回值</span>: <span class="type">'+doc.return.type+'</span>, ' + doc.return.text + '</p>');
	}
    if(doc.remark){
		var html = '					<div><span class="classic">备注</span>: ';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
		html += '</div>\r\n';
		content.push(html);
	}
	if(doc.member){
		var subcomment = "";
		var submenu = createMenuHtml(menu,tree,doc.name);
		subcomment += '\r\n				<div><span class="classic">成员:</span>\r\n';
		var str = '';
		var count = doc.member.length;
		for(var i=0;i<count;i++){
			var cmt = '';
			cmt += '					<p class="list"><a href="';
			var name = doc.member[i].name;
			cmt += getHtmlName(tree,name);
			cmt += '" title="';
			cmt += name;
			cmt += '">';
			cmt += name;
			cmt += '</a></p>\r\n';
			str += cmt;
		}
		subcomment += str;
        subcomment += '				</div>';
		var count = doc.member.length;
		for(var i=0;i<count;i++){
			parseJsonDoc(doc.member[i],tree,subcomment,submenu);
		}
		content.push(subcomment);
	}

	if(doc.example){
		var html = '					<div><span class="classic">示例: </span>\r\n<pre class="prettyprint js font">\r\n';
		for(var i=0;i<doc.example.length;i++){
			html += '<div class="codeline">' + doc.example[i] + '</div>\r\n';
		}
		html += '</pre></div>\r\n';
		content.push(html);
	}
	if(doc.source){
		//从文件加载源码，必须指定一个绝对路径，因为此程序的运行位置可能会变化。
		var source = readFile(doc.source);
		if(!source) throw "read doc.source failed:" + doc.source;
		source = source.replace(/[\<]/g,"&lt;");
		source = source.replace(/[\>]/g,"&gt;");
		var html = '					<div><span class="classic">示例: </span>\r\n<pre class="prettyprint js font">';
		html += source;
		html += '</pre></div>\r\n';
		content.push(html);
	}
	return content.join("\r\n");
}
function createParamHtml(param){
	return '					<div class="param">\r\n' + 
	'						<p><span class="param_name">' + 
	param.name +
	'</span> : <span class="type">' +
	param.type +
	'</span></p>\r\n' +
	'						<p>' +
	param.text +
	'</p>\r\n' +
	'					</div>';
}
main();