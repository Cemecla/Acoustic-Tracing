VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

void MAIN()
{
	BASE_COLOR = vInstanceColor;

	METALNESS = vCustomData.x;
    ROUGHNESS = vCustomData.y;
	SPECULAR_AMOUNT = vCustomData.z;
}

void POST_PROCESS()
{

}