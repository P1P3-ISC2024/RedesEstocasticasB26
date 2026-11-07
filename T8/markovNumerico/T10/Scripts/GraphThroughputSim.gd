extends MeshInstance3D

@export var by_frame:bool = true
@export var N:int = 10
@export_custom(PROPERTY_HINT_NONE, "suffix:px") # PAra que no pierda decimales el inspector
var rango:Vector2 = Vector2(0.01,0.9999999)
@export var punto:Node3D
@export var slots:int = 1000
@export var vertex_by_frame:int = 100
@export var barra:Sprite2D

var best:Vector3
var contador:int = 0

# Cambia el valor de la barra de progreso.
func progreso(porcentaje):
	if barra != null:
		barra.material.set_shader_parameter("load",porcentaje)
	return;

# Troughput promedio.
func s_miu(sig,tau):
	var succes:int  = 0
	var slot:int = 0
	var colision:int = 0
	var empty:int = 0
	var new:int = 0
	var tx:int = 0
	var n:int = 0
	# Empieza la simulación...
	while(slot < slots):
		tx = 0
		n = 0
		# Los nuevos paquetes...
		for i in range(new):
			if randf() <= tau:
				tx+=1
				n+=1
		# Los retransmitidos...
		for i in range(N-new):
			if randf() <= sig:
				tx+=1
		if tx == 0:
			empty+=1
		elif tx == 1:
			succes+=1
		else:
			colision+=n
			new-=n
		slot+=1
	return float(succes)/float(slots);

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
			punto.global_position = best + Vector3(0,0.085,0)
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
				punto.global_position = best + Vector3(0,0.085,0)
			if barra != null:
				%CanvasLayer.visible = false
	return;
