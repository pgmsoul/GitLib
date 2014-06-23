"use strict"

require("jsbase.dll");
Class(function main() {
    var file = new CFile();
    var str = readFile(file,"../jsbase/jskernel.cpp");
    var data = getCommentBlock(str);
    for(var i=0;i<data.length;i++){
        data[i] = formatBlock(data[i]);
    }
    str = data.join("");
	str = "[" + str + "]";

	var json = JSON.parse(str);

	json.sort(function(a,b){
		if(a.name>b.name) return 1;
		else if(a.name==b.name) return 0;
		else return -1;
	});
	
	var doc = {};
	doc.name = "jsuser";
	doc.member = json;
	doc.type = "library";
	doc.text = "jsuser 库文档";

	parseJsonDoc(doc,[],"");
	str = JSON.stringify(json,null,"\t");
	file.create("jdoc.js","w");
	file.length = 0;
	file.writeString(str,3);
},true);
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
function readFile(file,fn) {
    file.create(fn,"r");
    if(file.isNull()){
        alert("open failed");
        return;
    }
    return file.readString();
}
function formatBlock(str) {
    var lines = str.split("\n");
    var reg = /\s*\/\/([\s\S]*)/g;
    for(var i=0;i<lines.length;i++){
        var line = lines[i].trimRight();
        reg.lastIndex = 0;
        var a = reg.exec(line);
        if(a==null) continue;
        lines[i] = a[1];
    }
    return lines.join("\n");
}
function getCommentBlock(str) {
	var reg = /\/\/\*([\s\S]*?)\/\/\*/g;
	var data = [];
	while (true) {
		var a = reg.exec(str);
		if (a == null) break;
		data.push(a[1].trim());
	}
	return data;
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
	
	tree = tree.slice(0);
	tree.push(json.name);
	var content = "";
	if(json.type=="library"){
		content = createGroupContent(json,"类库");
	}else if(json.type=="group"){
		content = createGroupContent(json,"功能组");
	}else if(json.type=="class"){
		content = createGroupContent(json,"类");
	}else if(json.type=="function"){
		content = createFunctionContent(json);
	}else if(json.type=="property"){
		content = createPropertyContent(json);
	}
	var subcomment = "";
	if(json.member){
		var submenu = createMenuHtml(menu,tree,json.name);
		json.member.sort(function(a,b){
			if(a.name>b.name) return 1;
			else if(a.name==b.name) return 0;
			else return -1;
		});
		subcomment += '\r\n				<div><span class="classic">成员:</span>\r\n';
		var str = '';
		for(var i=0;i<json.member.length;i++){
			var cmt = '';
			cmt += '					<p class="param"><a href="';
			var name = json.member[i].name;
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
		for(var i=0;i<json.member.length;i++){
			parseJsonDoc(json.member[i],tree,subcomment,submenu);
		}
	}
	content += subcomment;
	
	var html = createHtmlPage(json.name,content,comment,menu);
	return saveHtmlPage(getHtmlName(tree),html);
}
function createMenuHtml(menu,tree,nm){
	var menuHtml = menu;
	if(menuHtml.length!=0) menuHtml += ' &gt;&gt; ';
	menuHtml += '<a href="' + getHtmlName(tree) + '">' + nm + '</a>';
	return menuHtml;
}
function createHtmlPage(title,content,comment,menuHtml){
	return ['<!DOCTYPE html>' ,
	'<html>' ,
	'<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />' ,
    '<head>' ,
    '	<link rel="stylesheet" type="text/css" href="./doc.css" />' ,
    '</head>' ,
    '<body>' ,
    ' 	<center>' ,
    ' 		<div id="container">' ,
    '			<div id="header"></div>' ,
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
    '		<div id="footer">Copyright www.jsuser.com</div>' ,
    '		</div>' ,
    '	</center>' ,
    '</body>' ,
	'</html>'].join("\r\n");
}
Class(function saveHtmlPage(fn,html){
    var file = new CFile();
	file.create(fn,"w");
    file.length = 0;
    file.writeString(html,65001);
	return fn;
},true);
function createGroupContent(doc,name){
	var content = [
	'				<p><span class="classic">类型:</span> ' + name + '</p>' ,
    '				<p><span class="classic">说明:</span> ' + doc.text + '</p>'];
    if(doc.remark){
		var html = '					<p><span class="classic">备注</span>: </p>\r\n';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
		content.push(html);
	}
	return content.join("\r\n");
}
/*function createLibraryContent(doc){
	var content = [
	'				<p><span class="classic">类型:</span> 类库</p>' ,
    '				<p><span class="classic">说明:</span> ' + doc.text + '</p>'];
    if(doc.remark){
		var html = '					<p><span class="classic">备注</span>: </p>\r\n';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
		content.push(html);
	}
	return content.join("\r\n");
}
function createClassContent(doc){
	var content = [
	'				<p><span class="classic">类型:</span> 类</p>' ,
    '				<p><span class="classic">说明:</span> ' + doc.text + '</p>'];
    if(doc.remark){
		var html = '					<p><span class="classic">备注</span>: </p>\r\n';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
		content.push(html);
	}
	return content.join("\r\n");
}*/
function createFunctionContent(doc){
	var content = [
	'				<p><span class="classic">类型:</span> 函数</p>' ,
    '				<p><span class="classic">说明:</span> ' + doc.text + '</p>' ,
    '				<div><span class="classic">参数:</span>'];
	if(doc.param){
		for(var i=0;i<doc.param.length;i++){
			content.push(createParamHtml(doc.param[i])) ;
		}
	}
	content.push('				</div>');
    content.push('				<p><span class="classic">返回值</span>: <span class="type">'+doc.return.type+'</span>, ' + doc.return.text + '</p>');
    if(doc.remark){
		var html = '					<p><span class="classic">备注</span>: </p>\r\n';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
		content.push(html);
	}
	return content.join("\r\n");
}
function createPropertyContent(doc){
	var content = [
	'				<p><span class="classic">类型:</span> 属性</p>' ,
    '				<p><span class="classic">说明:</span> ' + doc.text + '</p>'];
    if(doc.remark){
		var html = '					<p><span class="classic">备注</span>: </p>\r\n';
		for(var i=0;i<doc.remark.length;i++){
			html += '					<p>'+doc.remark[i]+'</p>\r\n';
		}
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