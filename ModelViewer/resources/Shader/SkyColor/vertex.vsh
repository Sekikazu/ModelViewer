struct VS_IN
{
    float4 pos : POSITION0;
	float4 norm: NORMAL0;
};
 
struct VS_OUT
{
    float4 pos  : SV_POSITION;
	float4 absPos : TEXCOOD0;
};

cbuffer ConstantBuffer
{
	float4x4 World;         //ワールド変換行列
	float4x4 View;          //ビュー変換行列
	float4x4 Projection;    //透視射影変換行列
	float4   Light;
	float4   LightColor;
	float4   Eye;
}
 
VS_OUT vs_skycolor( VS_IN input )
{
    VS_OUT output = (VS_OUT)0;

	// pos
	float4x4 temp = View;
	temp[3][0] = temp[3][1] = temp[3][2] = 0.f;

	output.pos = mul(input.pos, temp);
	output.pos = mul(output.pos, Projection);
 
	// norm
	output.absPos = input.pos;

    return output;
}