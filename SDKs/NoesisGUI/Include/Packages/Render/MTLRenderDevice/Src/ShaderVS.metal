#include <metal_stdlib>
using namespace metal;

#define ENTRY_POINT_(pos, color, uv0, uv1, coverage) pos##color##uv0##uv1##coverage##_VS
#define ENTRY_POINT(pos, color, uv0, uv1, coverage) ENTRY_POINT_(pos, color, uv0, uv1, coverage)
#define FNAME ENTRY_POINT(POS_NAME, COLOR_NAME, UV0_NAME, UV1_NAME, COVERAGE_NAME)

#if HAS_POSITION
    #define POS_NAME Pos
#else
    #define POS_NAME
#endif

#if HAS_COLOR
    #define COLOR_NAME Color
#else
    #define COLOR_NAME
#endif

#if HAS_UV0
    #define UV0_NAME Tex0
#else
    #define UV0_NAME
#endif

#if HAS_UV1
    #define UV1_NAME Tex1
#else
    #define UV1_NAME
#endif

#if HAS_COVERAGE
    #define COVERAGE_NAME Coverage
#else
    #define COVERAGE_NAME
#endif

struct In
{
#if HAS_POSITION
    float4 position [[ attribute(0) ]];
#endif

#if HAS_COLOR
    half4 color     [[ attribute(1) ]];
#endif

#if HAS_UV0
    half2 uv0       [[ attribute(2) ]];
#endif

#if HAS_UV1
    half2 uv1       [[ attribute(3) ]];
#endif

#if HAS_COVERAGE
    half coverage   [[ attribute(4) ]];
#endif
};

struct Out
{
#if HAS_POSITION
    float4 position [[ position ]];
#endif

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

struct Uniforms
{
   float4x4 projectionMtx;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vertex Out FNAME(In i [[ stage_in ]], constant Uniforms& u [[ buffer(1) ]])
{
    Out o;

#if HAS_POSITION
    o.position = (i.position * u.projectionMtx);
#endif

#if HAS_COLOR
    o.color = i.color;
#endif

#if HAS_UV0
    o.uv0 = i.uv0;
#endif

#if HAS_UV1
    o.uv1 = i.uv1;
#endif

#if HAS_COVERAGE
    o.coverage = i.coverage;
#endif

    return o;
}
