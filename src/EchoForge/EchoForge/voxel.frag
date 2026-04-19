VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

void MAIN()
{
	BASE_COLOR = vInstanceColor;
	BASE_COLOR.a = vInstanceColor.a;

	METALNESS = vCustomData.x;
    ROUGHNESS = vCustomData.y;
	SPECULAR_AMOUNT = vCustomData.z;
}

void POST_PROCESS()
{

}