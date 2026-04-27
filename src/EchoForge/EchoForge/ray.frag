VARYING vec3 vWorldPos;
VARYING vec3 vWorldNormal;
VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

void MAIN()
{
    BASE_COLOR = vInstanceColor;

    EMISSIVE_COLOR = vInstanceColor.rgb * intensity;
    ROUGHNESS  = 1.0;
    METALNESS  = 0.0;
    SPECULAR_AMOUNT = 0.0;
}