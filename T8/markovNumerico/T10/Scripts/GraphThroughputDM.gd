extends MeshInstance3D

## Numero de estados N = nodos + 1
@export var by_frame:bool = true
@export var N:int = 10
@export_custom(PROPERTY_HINT_NONE, "suffix:px") # PAra que no pierda decimales el inspector
var rango:Vector2 = Vector2(0.0000001,0.9999999)
@export var punto:Node3D
@export var vertex_by_frame:int = 100
@export var barra:Sprite2D

var P:Matrix
var best:Vector3
var pi:Array = []
var contador:int = 0

func fact(n):
	var ans:int = 1;
	for i in range(1,n+1):
		ans *= i;
	return ans;

func comb(n,k):
	return fact(n)/( fact(k)*fact(n-k) );

func calcP(i,j,sig,tau):
	if j < i-1:
		return 0;
	if j == i-1:
		return i*sig*((1-tau)**(N-i));
	if j == i:
		return ( 1-i*sig*((1-sig)**(i-1)) )*((1-tau)**(N-i)) + (N-i)*tau*((1-tau)**(N-i-1))*((1-sig)**i);
	if j == i+1:
		return (N-i)*tau*((1-tau)**(N-i-1))*(1-(1-sig)**i);
	return comb(N-i,j-i)*(tau**(j-i))*((1-tau)**(N-j));

func createP(sig,tau):
	var p_ij:float
	P = Matrix.new(N,N)
	for i in range(N+1):
		for j in range(N+1):	# desde i para solo recorrer N^2/2
			p_ij = calcP(i,j,sig,tau)
			P.matriz[P.pos(i,j)] = p_ij

# Troughput.
func s(i,sig,tau):
	return (
		i*sig*((1-sig)**(i-1))*((1-tau)**(N-i))
		+
		(N-i)*tau*((1-tau)**(N-i-1))*((1-sig)**i)
	)

func calcPis(sigma,tau):
	var sum:float = 1
	var sum_pi:float = 0
	# Calculo los pi...
	pi = []
	pi.append(1.0)
	for i in range(1,N+1):
		sum_pi = 0
		for j in range(i):
			sum_pi+=pi[j]*calcP(j,i-1,sigma,tau)
		pi.append( (pi[i-1]-sum_pi)/calcP(i,i-1,sigma,tau) )
		sum += pi[i]
	#"""
	# Normalizo...
	for i in range(N+1):
		pi[i] /= sum
	return;

# Troughput promedio.
func s_miu(sig,tau):
	var sum:float = 0;
	sig = clamp(sig,rango.x,rango.y)
	tau = clamp(tau,rango.x,rango.y)
	# Caculo las probabilidades de caer en un estado...
	calcPis(sig,tau)
	# Realizo la esperanza donde PI_i es la distribución...
	for i in range(N+1):
		sum += s(i,sig,tau)*pi[i]
	return sum;


func _updateAll():
	# Obtengo variables...
	var arrays = mesh.surface_get_arrays(0)
	var vertices: PackedVector3Array = arrays[Mesh.ARRAY_VERTEX]
	best = Vector3(0,0,0)
	# Para cada columna...
	for i in range(vertices.size()):
		var vertice: Vector3 = vertices[i]
		vertice += Vector3(0.5,0,0.5)	# Porque va de -0.5 a 0.5 ya que el centro es 0
		# Modificar únicamente la altura con la esperanza de s(i)
		vertice.y = s_miu(vertice.x,vertice.z)
		if vertice.y > best.y:
			best = vertice
		vertices[i] = vertice
	arrays[Mesh.ARRAY_VERTEX] = vertices
	# Creo la nueva malla...
	var nueva_malla = ArrayMesh.new()
	nueva_malla.add_surface_from_arrays(
		Mesh.PRIMITIVE_TRIANGLES,
		arrays
	)
	# Copiar los materiales de la malla anterior
	for i in range(nueva_malla.get_surface_count()):
		if i < mesh.get_surface_count():
			nueva_malla.surface_set_material(
				i,
				mesh.surface_get_material(i)
			)
	# Actualizo la malla...
	mesh = nueva_malla

func progreso(porcentaje):
	if barra != null:
		barra.material.set_shader_parameter("load",porcentaje)
	return;

func _update():
	# Obtengo variables...
	var arrays = mesh.surface_get_arrays(0)
	var vertices: PackedVector3Array = arrays[Mesh.ARRAY_VERTEX]
	var v:int
	# Para cada columna...
	for i in range(vertex_by_frame):
		if i + contador >= vertices.size():
			by_frame = false
			break;
		v = i+contador
		var vertice: Vector3 = vertices[v]
		vertice += Vector3(0.5,0,0.5)	# Porque va de -0.5 a 0.5 ya que el centro es 0
		# Modificar únicamente la altura con la esperanza de s(i)
		vertice.y = s_miu(vertice.x,vertice.z)
		if vertice.y > best.y:
			best = vertice
		vertices[v] = vertice
		progreso(float(v)/float(vertices.size()))
		pass
	contador+=vertex_by_frame
	arrays[Mesh.ARRAY_VERTEX] = vertices
	# Creo la nueva malla...
	var nueva_malla = ArrayMesh.new()
	nueva_malla.add_surface_from_arrays(
		Mesh.PRIMITIVE_TRIANGLES,
		arrays
	)
	# Copiar los materiales de la malla anterior
	for i in range(nueva_malla.get_surface_count()):
		if i < mesh.get_surface_count():
			nueva_malla.surface_set_material(
				i,
				mesh.surface_get_material(i)
			)
	# Actualizo la malla...
	mesh = nueva_malla
	return;

func _ready() -> void:
	if not by_frame:
		_updateAll()
		print("Mejor valor: ",best)
		if punto != null:
			punto.global_position = best + Vector3(0,0.02,0)
		if barra != null:
			%CanvasLayer.visible = false
	else:
		contador = 0
		best = Vector3(0,0,0)
	return;

func _process(delta: float) -> void:
	if by_frame:
		_update()
		if not by_frame:
			print("Mejor valor: ",best)
			if punto != null:
				punto.global_position = best + Vector3(0,0.02,0)
			if barra != null:
				%CanvasLayer.visible = false
	return;
