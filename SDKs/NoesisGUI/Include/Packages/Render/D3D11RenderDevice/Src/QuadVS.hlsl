float4 main(in uint id: SV_VertexID): SV_Position
{
    float4 output = 0.0f;

    if(id == 0)
    {
        output = float4(-1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if(id == 1)
    {
        output = float4(3.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        output = float4(-1.0f, -3.0f, 1.0f, 1.0f);
    }

    return output;
}