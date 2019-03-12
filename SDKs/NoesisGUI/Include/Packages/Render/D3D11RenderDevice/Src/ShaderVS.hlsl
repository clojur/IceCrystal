#define fixed half
#define fixed4 half4

struct In
{
    half4 position: POSITION;

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

struct Out
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

float4x4 projectionMtx;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(in In i, out Out o)
{
    o.position = mul(i.position, projectionMtx);

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
}