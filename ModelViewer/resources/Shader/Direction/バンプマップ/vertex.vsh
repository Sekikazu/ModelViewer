struct VS_IN
{
    float4 pos : POSITION0;
    float4 nor : NORMAL0;
    float4 bin : BINORMAL0;
    float4 tan : TANGENT0;
    float2 tex : TEXCOORD0;
};
 
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 nor : NORMAL0;
    float2 tex : TEXCOORD0;
    float4 lightTangentDirect : TEXCOORD1;
};
 
cbuffer ConstantBuffer
{
    float4x4 World;         //ワールド変換行列
    float4x4 View;          //ビュー変換行列
    float4x4 Projection;    //透視射影変換行列
    float4   Light;
}
 
VS_OUT vs_direction( VS_IN input )
{
    VS_OUT output;
    float3 nor;

    // 射影空間
    output.pos = mul(input.pos,  World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);

    // 法線
    nor = mul(input.nor, World).xyz;
    nor = normalize(nor);
    output.nor = float4(nor, 0.f);

   // uv座標の受け渡し
   output.tex = input.tex;

    // ライトをタンジェント空間へ
    float4x4 invTangentMat = {
        float4(input.tan.xyz, 0.f), float4(input.bin.xyz, 0.f), 
        float4(input.nor.xyz, 0.f), float4(0.f, 0.f, 0.f, 1.f)
    };
    invTangentMat = transpose(invTangentMat);
    output.lightTangentDirect = mul( float4(-Light.xyz, 1.f), invTangentMat);
    output.lightTangentDirect = normalize(output.lightTangentDirect);

    return output;
}