extends MeshInstance3D

@export var N:int = 10
@export_custom(PROPERTY_HINT_NONE, "suffix:px") # PAra que no pierda decimales el inspector
var rango:Vector2 = Vector2(0.0000001,0.9999999)
@export var punto:Node3D

var gauss:GS
var P:Matrix
var best:Vector3

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
	for i in range(N):
		for j in range(i,N):	# desde i para solo recorrer N^2/2
			p_ij = calcP(i,j,sig,tau)
			P.matriz[P.pos(i,j)] = p_ij
			P.matriz[P.pos(j,i)] = p_ij	# Porque es simetrica.

# Troughput.
func s(i,sig,tau):
	return (
		i*sig*((1-sig)**(i-1))*((1-tau)**(N-i))
		+
		(N-i)*tau*((1-tau)**(N-i-1))*((1-sig)**i)
	)
# Troughput promedio.
func s_miu(sig,tau):
	var sum:float = 0;
	sig = clamp(sig,rango.x,rango.y)
	tau = clamp(tau,rango.x,rango.y)
	# Creo la matriz de Pij...
	createP(sig,tau)
	# Calculo con GAUSS-SEIDEL...
	gauss = GS.new(P)
	gauss.runGS()
	# Realizo la esperanza donde PI_i es la distribución...
	for i in range(N):
		sum += s(i,sig,tau)*gauss.pi[i]
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

func _ready() -> void:
	_updateAll()
	print("Mejor valor: ",best)
	punto.global_position = best + Vector3(0,0.005,0)
