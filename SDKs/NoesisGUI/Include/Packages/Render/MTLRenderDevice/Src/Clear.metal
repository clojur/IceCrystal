#include <metal_stdlib>
using namespace metal;

struct Out
{
    float4 position [[ position ]];
};

vertex Out ClearVS(ushort id [[ vertex_id ]])
{
    Out o;
    o.position = id == 0 ? float4(-1.0f, 1.0f, 1.0f, 1.0f): 
                 id == 1 ? float4(3.0f, 1.0f, 1.0f, 1.0f):
                           float4(-1.0f, -3.0f, 1.0f, 1.0f);
    return o;
}

fragment half4 ClearFS()
{
    return 0.0h;
}