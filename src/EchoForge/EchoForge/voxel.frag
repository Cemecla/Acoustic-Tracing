VARYING vec4 vInstanceColor;

void MAIN()
{
	BASE_COLOR = vInstanceColor;
	SPECULAR_AMOUNT = 0.0;
    ROUGHNESS = 1.0;
}

void POST_PROCESS()
{
    // let Qt apply fog by not overriding COLOR here
}