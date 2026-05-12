VARYING vec3 vWorldPos;
VARYING vec3 vWorldNormal;
VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;
VARYING vec2 vUV;

void MAIN()
{
    float tipFade = 1.0 - pow(vUV.y, 2.0);

    vec3 emissive = vInstanceColor.rgb * intensity * tipFade;

    // alpha goes into BASE_COLOR.a — there is no separate ALPHA keyword
    BASE_COLOR     = vec4(vInstanceColor.rgb, vInstanceColor.a * tipFade);
    EMISSIVE_COLOR = emissive;
    ROUGHNESS       = 1.0;
    METALNESS       = 0.0;
    SPECULAR_AMOUNT = 0.0;
}