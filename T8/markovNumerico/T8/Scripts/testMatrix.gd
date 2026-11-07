extends Node

var m:Matrix


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	m = Matrix.new(2,2)
	m.matriz.fill(1);
	#print(m.matriz)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
