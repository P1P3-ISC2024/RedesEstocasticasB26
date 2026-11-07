shader_type canvas_item;

uniform vec4 back:source_color;
uniform vec4 full:source_color;
uniform float load:hint_range(0.0, 1.0);
uniform float vel;
uniform float dist:hint_range(0.0, 1.0);

void fragment() {
	float d = clamp(
		(2./dist)*abs( UV.x+dist*0.5 - mod(TIME*vel,load+dist) )
		,0,1);
	COLOR.rgb = back.rgb;
	if(UV.x <= load){
		COLOR.rgb = full.rgb + vec3(1)*(1.-d);
	}
}

//void light() {
//	// Called for every pixel for every light affecting the CanvasItem.
//	// Uncomment to replace the default light processing function with this one.
//}
