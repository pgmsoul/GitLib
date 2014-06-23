var FilePath = {};
FilePath.getExt = function(path) {
	var i1 = path.lastIndexOf(".");
	var i2 = path.lastIndexOf("\\");
	var i3 = path.lastIndexOf("/");
	if (i1 < 0 || i2 > i1 || i3 > i1) return "";
	return path.substr(i1 + 1);
}
FilePath.getName = function(path) {
	var i2 = path.lastIndexOf("\\");
	var i3 = path.lastIndexOf("/");
	var i = i2 > i3 ? i2 : i3;
	if (i < 0) return path;
	return path.substr(i + 1);
}
FilePath.getTitle = function(path) {
	var name = FilePath.getName(path);
	var i = name.lastIndexOf(".");
	if (i < 0) return name;
	return name.substr(0, i);
}
FilePath.getParent = function(path) {
	var i2 = path.lastIndexOf("\\");
	var i3 = path.lastIndexOf("/");
	var i = i2 > i3 ? i2 : i3;
	if (i < 0) return "";
	return path.substr(0, i);
}
FilePath.toSlash = function(path) {
	path = path.replace(/(\\{1,})|(\/{2,})/g, "/");
	if (path.length > 0 && path.charAt(path.length - 1) == "/") path = path.substr(0, path.length - 1);
	return path;
}
FilePath.toBackslash = function(path) {
	path = path.replace(/(\\{2,})|(\/{1,})/g, "\\");
	if (path.length > 0 && path.charAt(path.length - 1) == "\\") path = path.substr(0, path.length - 1);
	return path;
}
FilePath.linkPath = function(path) {
	for (var i = 1; i < arguments.length; i++)
		path += "/" + arguments[i];
	return FilePath.toSlash(path);
}
FilePath.linkExt = function(path, ext) {
	if (ext.length > 0 && ext.charAt(0) != ".") ext = "." + ext;
	return path + ext;
}
FilePath.isSame = function(path1, path2) {
	path1 = path1.toLowerCase();
	path2 = path2.toLowerCase();
	path1 = FilePath.toSlash(path1);
	path2 = FilePath.toSlash(path2);
	return path1 === path2;
}
CFileList.prototype.addFilter = function(filter) {
	var a = filter.toString().split(",");
	var length = a.length;
	for (var i = 0; i < length; i++) {
		if(this.filter.indexOf(a[i])==-1)
			this.filter.push(a[i]);
	}
};
CFileList.prototype.delFilter = function(filter) {
	var a = filter.toString().split(",");
	var length = a.length;
	for (var i = 0; i < length; i++) {
		var fl = this.filter;
		for (var j = 0; j < fl.length; i++) {
			if (fl[j] != a[i]) continue;
			fl.slice(j, 1);
			j--;
			break;
		}
	}
}