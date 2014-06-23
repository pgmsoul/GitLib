CPen.prototype.fromDCPen = function () {
	return this.fromStockPen(19);//DC_PEN
}
CBrush.prototype.fromDCBrush = function () {
	return this.fromStockBrush(18);//DC_BRUSH
}