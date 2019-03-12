#if EFFECT_RGBA

#elif EFFECT_MASK

#elif EFFECT_PATH_SOLID
    #define EFFECT_PATH 1
    #define PAINT_SOLID 1

#elif EFFECT_PATH_LINEAR
    #define EFFECT_PATH 1
    #define PAINT_LINEAR 1

#elif EFFECT_PATH_RADIAL
    #define EFFECT_PATH 1
    #define PAINT_RADIAL 1

#elif EFFECT_PATH_PATTERN
    #define EFFECT_PATH 1
    #define PAINT_PATTERN 1

#elif EFFECT_PATH_AA_SOLID
    #define EFFECT_PATH_AA 1
    #define PAINT_SOLID 1

#elif EFFECT_PATH_AA_LINEAR
    #define EFFECT_PATH_AA 1
    #define PAINT_LINEAR 1

#elif EFFECT_PATH_AA_RADIAL
    #define EFFECT_PATH_AA 1
    #define PAINT_RADIAL 1

#elif EFFECT_PATH_AA_PATTERN
    #define EFFECT_PATH_AA 1
    #define PAINT_PATTERN 1

#elif EFFECT_IMAGE_PAINT_OPACITY_SOLID
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_SOLID 1

#elif EFFECT_IMAGE_PAINT_OPACITY_LINEAR
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_LINEAR 1

#elif EFFECT_IMAGE_PAINT_OPACITY_RADIAL
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_RADIAL 1

#elif EFFECT_IMAGE_PAINT_OPACITY_PATTERN
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_PATTERN 1

#elif EFFECT_TEXT_SOLID
    #define EFFECT_TEXT 1
    #define PAINT_SOLID 1

#elif EFFECT_TEXT_LINEAR
    #define EFFECT_TEXT 1
    #define PAINT_LINEAR 1

#elif EFFECT_TEXT_RADIAL
    #define EFFECT_TEXT 1
    #define PAINT_RADIAL 1

#elif EFFECT_TEXT_PATTERN
    #define EFFECT_TEXT 1
    #define PAINT_PATTERN 1
#else
    #error EFFECT not defined
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if EFFECT_PATH_AA
    #define HAS_COVERAGE 1
#endif

#if PAINT_SOLID
    #define HAS_COLOR 1
#endif

#if PAINT_LINEAR || PAINT_RADIAL || PAINT_PATTERN
    #define HAS_UV0 1
#endif

#if EFFECT_IMAGE_PAINT_OPACITY || EFFECT_TEXT
    #define HAS_UV1 1
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define fixed half
#define fixed4 half4

#if EFFECT_RGBA
    float4 rgba;
#endif

#if PAINT_RADIAL
    float4 radialGrad[2];
#endif

#if PAINT_LINEAR || PAINT_RADIAL || PAINT_PATTERN
    float opacity;
#endif

Texture2D pattern: register(t0);
SamplerState patternSampler: register(s0);

Texture2D ramps: register(t1);
SamplerState rampsSampler: register(s1);

Texture2D image: register(t2);
SamplerState imageSampler: register(s2);

Texture2D glyphs: register(t3);
SamplerState glyphsSampler: register(s3);

struct In
{
    half4 position: SV_Position;

#if HAS_COLOR
    fixed4 color: COLOR;
#endif

#if HAS_UV0
    half2 uv0: TEXCOORD0;
#endif

#if HAS_UV1
    half2 uv1: TEXCOORD1;
#endif

#if HAS_COVERAGE
    fixed coverage: TEXCOORD2;
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
fixed4 main(in In i): SV_Target
{
    /////////////////////////////////////////////////////
    // Fetch paint color and opacity
    /////////////////////////////////////////////////////
    #if PAINT_SOLID
        fixed4 paint = i.color;
        fixed opacity_ = 1.0f;

    #elif PAINT_LINEAR
        fixed4 paint = ramps.Sample(rampsSampler, i.uv0);
        fixed opacity_ = opacity;

    #elif PAINT_RADIAL
        half dd = radialGrad[1].y * i.uv0.x - radialGrad[1].z * i.uv0.y;
        half u = radialGrad[0].x * i.uv0.x + radialGrad[0].y * i.uv0.y + radialGrad[0].z * 
            sqrt(radialGrad[0].w * i.uv0.x * i.uv0.x + radialGrad[1].x * i.uv0.y * i.uv0.y - dd * dd);
        fixed4 paint = ramps.Sample(rampsSampler, half2(u, radialGrad[1].w));
        fixed opacity_ = opacity;

    #elif PAINT_PATTERN
        fixed4 paint = pattern.Sample(patternSampler, i.uv0);
        fixed opacity_ = opacity;
    #endif

    /////////////////////////////////////////////////////
    // Apply selected effect
    /////////////////////////////////////////////////////
    #if EFFECT_RGBA
        return rgba;

    #elif EFFECT_MASK
        return fixed4(1.0f, 1.0f, 1.0f, 1.0f);

    #elif EFFECT_PATH
        return opacity_ * paint;

    #elif EFFECT_PATH_AA
        return (opacity_ * i.coverage) * paint;

    #elif EFFECT_IMAGE_PAINT_OPACITY
        return image.Sample(imageSampler, i.uv1) * (opacity_ * paint.a);

    #elif EFFECT_TEXT
        return (glyphs.Sample(glyphsSampler, i.uv1).r * opacity_) * paint;

    #endif
}
