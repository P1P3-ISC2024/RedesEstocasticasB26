extends Node
class_name GS

@export var P:Matrix
@export var on_proces:bool
@export var max_iter:int = 100
var pi:Array[float] = []
var vali:float = 0.000001

func _init(matrix:Matrix,default:bool=true) -> void:
	self.P = matrix
	self.pi.resize(P.col)
	if default:
		self.pi.fill(1.0/P.col)
	return;

func calcPi(i:int):
	var pi_i = 0
	for j in range(P.fil):
		if j == i:
			continue;
		pi_i += pi[j] * P.at(j,i)
		pass;
	pi_i /= 1 - P.at(i,i)
	return pi_i;

func calcPis():
	for i in range(pi.size()):
		pi[i] = calcPi(i)
		pass
	return;

func normalizar():
	var sum = 0
	for i in range(pi.size()):
		sum += pi[i]
	if sum != 0:
		for i in range(pi.size()):
			pi[i] /= sum
	return;
	
func isValido(old_pi):
	var sum = 0
	for i in range(pi.size()):
		sum += abs(old_pi[i] - pi[i])
	#print("E = ",sum)
	return sum < vali;


func _ready() -> void:
	var old_pi:Array = pi.slice(0)
	var valido = false
	var i=0
	if not on_proces:
		#print("_iteración:")
		while(not valido and i <= max_iter):
			#print("  _",i,":")
			calcPis()
			#print("\tValores Brutos...")
			#print(pi)
			normalizar()
			#print("\tValores Normalizados..")
			#print(pi)
			valido = isValido(old_pi)
			old_pi = pi.slice(0)
			i += 1
	return;
	
func runGS():
	var old_pi:Array = pi.slice(0)
	var valido = false
	var i=0
	if not on_proces:
		#print("_iteración:")
		while(not valido and i <= max_iter):
			#print("  _",i,":")
			calcPis()
			#print("\tValores Brutos...")
			#print(pi)
			normalizar()
			#print("\tValores Normalizados..")
			#print(pi)
			valido = isValido(old_pi)
			old_pi = pi.slice(0)
			i += 1
	return;
