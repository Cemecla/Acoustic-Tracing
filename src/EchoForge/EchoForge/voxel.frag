VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

void MAIN()
{
    float packed = vCustomData.w;
    float topID = floor(packed / 65536.0);
    float sideID = floor(mod(packed / 256.0, 256.0));
    float bottomID = floor(mod(packed, 256.0));

    vec3 n = normalize(VAR_WORLD_NORMAL);

    float tileID;
    if (n.y >  0.5) 
        tileID = topID;
    else if (n.y < -0.5) 
        tileID = bottomID;
    else
        tileID = sideID;

    float col = mod(tileID, atlasCols);
    float row = floor(tileID / atlasCols);

    vec2 tileSize = vec2(1.0 / atlasCols, 1.0 / atlasRows);

    vec2 tileOrigin = vec2(col * tileSize.x, (atlasRows - 1.0 - row) * tileSize.y);

    vec2 texelInset = vec2(0.5 / atlasTexWidth, 0.5 / atlasTexHeight);
    vec2 localUV = clamp(fract(UV0), texelInset, vec2(1.0) - texelInset);

    vec2 atlasUV = tileOrigin + localUV * tileSize;

    vec4 sampled = texture(baseColorMap, atlasUV);

    if (sampled.a < 0.5) discard;

    BASE_COLOR = sampled;

    METALNESS = vCustomData.x;
    ROUGHNESS = vCustomData.y;
    SPECULAR_AMOUNT = vCustomData.z;
}