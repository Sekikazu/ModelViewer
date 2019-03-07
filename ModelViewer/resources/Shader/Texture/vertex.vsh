struct VS_IN
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
};
 
struct VS_OUT
{
    float4 pos  : SV_POSITION;
    float2 tex  : TEXCOORD0;
};

cbuffer ConstantBuffer
{
    float4x4 buffer;
}
 
VS_OUT vs_texture( VS_IN input )
{
    VS_OUT output;

    output.pos = mul(input.pos, buffer);
    output.tex = input.tex;
 
    return output;
}