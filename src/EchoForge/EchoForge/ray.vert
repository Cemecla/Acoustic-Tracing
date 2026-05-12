
VARYING vec3 vWorldPos;
VARYING vec3 vWorldNormal;
VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;
VARYING vec2 vUV;


void MAIN()
{
	vInstanceColor = INSTANCE_COLOR;
	vCustomData = INSTANCE_DATA;
	vUV = UV0;
}