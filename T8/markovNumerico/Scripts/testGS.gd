extends GS

func _init() -> void:
	var P:Matrix = Matrix.new()
	P.matriz = [
		0.0,0.3,0.7,
		0.3,0.0,0.7,
		0.9,0.1,0.0
		]
	super._init(P,false)
	pi = [0.4,0.2,0.4]
	print(pi)
	print(P.matriz)

# Called when the node enters the scene tree for the first time.
#func _ready() -> void:
#	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
