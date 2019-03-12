#include <metal_stdlib>
using namespace metal;

#if EFFECT_RGBA
    #define FNAME RGBA_FS

#elif EFFECT_PATH_SOLID
    #define FNAME PathSolid_FS
    #define EFFECT_PATH 1
    #define PAINT_SOLID 1

#elif EFFECT_PATH_LINEAR
    #define FNAME PathLinear_FS
    #define EFFECT_PATH 1
    #define PAINT_LINEAR 1

#elif EFFECT_PATH_RADIAL
    #define FNAME PathRadial_FS
    #define EFFECT_PATH 1
    #define PAINT_RADIAL 1

#elif EFFECT_PATH_PATTERN
    #define FNAME PathPattern_FS
    #define EFFECT_PATH 1
    #define PAINT_PATTERN 1

#elif EFFECT_PATH_AA_SOLID
    #define FNAME PathAASolid_FS
    #define EFFECT_PATH_AA 1
    #define PAINT_SOLID 1

#elif EFFECT_PATH_AA_LINEAR
    #define FNAME PathAALinear_FS
    #define EFFECT_PATH_AA 1
    #define PAINT_LINEAR 1

#elif EFFECT_PATH_AA_RADIAL
    #define FNAME PathAARadial_FS
    #define EFFECT_PATH_AA 1
    #define PAINT_RADIAL 1

#elif EFFECT_PATH_AA_PATTERN
    #define FNAME PathAAPattern_FS
    #define EFFECT_PATH_AA 1
    #define PAINT_PATTERN 1

#elif EFFECT_IMAGE_PAINT_OPACITY_SOLID
    #define FNAME ImagePaintOpacitySolid_FS
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_SOLID 1

#elif EFFECT_IMAGE_PAINT_OPACITY_LINEAR
    #define FNAME ImagePaintOpacityLinear_FS
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_LINEAR 1

#elif EFFECT_IMAGE_PAINT_OPACITY_RADIAL
    #define FNAME ImagePaintOpacityRadial_FS
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_RADIAL 1

#elif EFFECT_IMAGE_PAINT_OPACITY_PATTERN
    #define FNAME ImagePaintOpacityPattern_FS
    #define EFFECT_IMAGE_PAINT_OPACITY 1
    #define PAINT_PATTERN 1

#elif EFFECT_TEXT_SOLID
    #define FNAME TextSolid_FS
    #define EFFECT_TEXT 1
    #define PAINT_SOLID 1

#elif EFFECT_TEXT_LINEAR
    #define FNAME TextLinear_FS
    #define EFFECT_TEXT 1
    #define PAINT_LINEAR 1

#elif EFFECT_TEXT_RADIAL
    #define FNAME TextRadial_FS
    #define EFFECT_TEXT 1
    #define PAINT_RADIAL 1

#elif EFFECT_TEXT_PATTERN
    #define FNAME TextPattern_FS
    #define EFFECT_TEXT 1
    #define PAINT_PATTERN 1
#else
    #error No valid effect defined
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
struct Uniforms
{
#if EFFECT_RGBA
    packed_float4 rgba;
#endif

#if PAINT_RADIAL
    packed_float4 radialGrad[2];
#endif

#if PAINT_LINEAR || PAINT_RADIAL || PAINT_PATTERN
    float opacity;
#endif
};

struct In
{
#if HAS_COLOR
    half4 color;
#endif

#if HAS_UV0
    half2 uv0;
#endif

#if HAS_UV1
    half2 uv1;
#endif

#if HAS_COVERAGE
    half coverage;
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
fragment half4 FNAME(In i [[ stage_in ]], constant Uniforms& u [[ buffer(0) ]],
    texture2d<half> pattern [[ texture(0) ]], sampler patternSampler [[ sampler(0) ]],
    texture2d<half> ramps   [[ texture(1) ]], sampler rampsSampler   [[ sampler(1) ]],
    texture2d<half> image   [[ texture(2) ]], sampler imageSampler   [[ sampler(2) ]],
    texture2d<half> glyphs  [[ texture(3) ]], sampler glyphsSampler  [[ sampler(3) ]]
    )
{
    /////////////////////////////////////////////////////
    // Fetch paint color and opacity
    /////////////////////////////////////////////////////
    #if PAINT_SOLID
        half4 paint = i.color;
        half opacity_ = 1.0h;

    #elif PAINT_LINEAR
        half4 paint = ramps.sample(rampsSampler, (float2)i.uv0);
        half opacity_ = u.opacity;

    #elif PAINT_RADIAL
        float dd = u.radialGrad[1][1] * i.uv0.x - u.radialGrad[1][2] * i.uv0.y;
        float uu = u.radialGrad[0][0] * i.uv0.x + u.radialGrad[0][1] * i.uv0.y + u.radialGrad[0][2] * 
            sqrt(u.radialGrad[0][3] * i.uv0.x * i.uv0.x + u.radialGrad[1][0] * i.uv0.y * i.uv0.y - dd * dd);
        half4 paint = ramps.sample(rampsSampler, (float2)(uu, u.radialGrad[1][3]));
        half opacity_ = u.opacity;

    #elif PAINT_PATTERN
        half4 paint = pattern.sample(patternSampler, (float2)i.uv0);
        half opacity_ = u.opacity;
    #endif

    /////////////////////////////////////////////////////
    // Apply selected effect
    /////////////////////////////////////////////////////
    #if EFFECT_RGBA
        return (half4)u.rgba;

    #elif EFFECT_PATH
        return opacity_ * paint;

    #elif EFFECT_PATH_AA
        return (opacity_ * i.coverage) * paint;

    #elif EFFECT_IMAGE_PAINT_OPACITY
        return image.sample(imageSampler, (float2)i.uv1) * (opacity_ * paint.a);

    #elif EFFECT_TEXT
        return (glyphs.sample(glyphsSampler, (float2)i.uv1).r * opacity_) * paint;

    #endif
}
