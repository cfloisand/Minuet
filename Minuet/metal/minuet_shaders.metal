//
//  minuet_shaders.metal
//  Minuet
//
//  Created by Christian Floisand on 2022-10-29.
//

#include <metal_stdlib>
using namespace metal;


struct VertexOut {
    float4 pos [[ position ]];
};

vertex VertexOut basic_vertex(constant packed_float3 *vertexArray [[ buffer(0) ]],
                              ushort vid [[ vertex_id ]])
{
    VertexOut out;
    out.pos = float4(vertexArray[vid], 1.f);
    return out;
}

fragment float4 color_fragment(VertexOut f [[ stage_in ]])
{
    return float4(0.f, 0.5f, 0.3f, 1.f);
}
