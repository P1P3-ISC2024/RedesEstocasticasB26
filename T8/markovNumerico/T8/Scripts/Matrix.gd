extends Resource
class_name Matrix

var col:int;
var fil:int;
var matriz:Array = []

func _init(filas=3,columnas=3) -> void:
	self.col = columnas
	self.fil = filas
	self.matriz.resize(col*fil)
	return;
	
func pos(x,y):
	return x*col + y;

func at(x,y):
	return self.matriz[pos(x,y)];
