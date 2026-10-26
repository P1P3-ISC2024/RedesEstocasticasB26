extends Camera3D

@export var velocidad: float = 0.5
@export var interes:Node3D

func _process(delta: float) -> void:
	var direccion := Vector3.ZERO
	# Miro hacia el objeto de interes...
	self.look_at(interes.position+ Vector3(0.5,0.5,0.5))
	# WASD...
	if Input.is_key_pressed(KEY_W):
		direccion -= global_basis.z
	if Input.is_key_pressed(KEY_S):
		direccion += global_basis.z
	if Input.is_key_pressed(KEY_A):
		direccion -= global_basis.x
	if Input.is_key_pressed(KEY_D):
		direccion += global_basis.x
	# EQ arriba abajo...
	if Input.is_key_pressed(KEY_E):
		direccion += global_basis.y
	if Input.is_key_pressed(KEY_Q):
		direccion -= global_basis.y
	# Normalizar...
	if direccion != Vector3.ZERO:
		direccion = direccion.normalized()
	# Mover...
	position += direccion * velocidad * delta
