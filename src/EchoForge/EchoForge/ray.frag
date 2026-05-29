VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;
VARYING vec2 vUV;

void MAIN()
{
    vec3 emissive = vInstanceColor.rgb * intensity;

    BASE_COLOR = vInstanceColor;
    EMISSIVE_COLOR = emissive;
    ROUGHNESS = 1.0;
    METALNESS = 0.0;
    SPECULAR_AMOUNT = 0.0;
}