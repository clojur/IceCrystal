Texture2DMS<float4, NUM_SAMPLES> t;

float4 main(in float4 pos: SV_Position): SV_Target
{
    float4 o = 0.0f;

    [unroll]
    for (int s = 0; s < NUM_SAMPLES; s++)
    {
        o += t.Load(pos.xy, s);
    }

    return o / NUM_SAMPLES;
}