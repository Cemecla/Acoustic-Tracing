VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

void MAIN()
{
	vInstanceColor = INSTANCE_COLOR;
	vCustomData = INSTANCE_DATA;
}