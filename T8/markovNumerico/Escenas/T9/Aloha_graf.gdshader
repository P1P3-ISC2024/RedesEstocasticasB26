shader_type spatial;
render_mode cull_disabled;

uniform bool altura = true;
uniform float linea:hint_range(0.0, 0.01) = 0.005;
varying smooth vec3 pos;

void vertex(){
	pos = VERTEX;
}

void fragment() {
	// Según UV...
	ALBEDO.rg = UV;
	EMISSION.rg = UV-0.5;
	ALBEDO.b = 0.;
	EMISSION.b = 0.;
	if(altura){
		ALBEDO.b = pos.y;
		//EMISSION = vec3(1,1,1)*pos.y;
	}
	if( mod(pos.x,0.1) < linea || mod(pos.z,0.1) < linea ){
		ALBEDO = vec3(0);
		EMISSION = vec3(0);
	}
	else if( mod(pos.x,0.05) < linea*0.5 || mod(pos.z,0.05) < linea*0.5 ){
		ALBEDO = vec3(0);
		EMISSION = vec3(0.1);
	}
	
}
