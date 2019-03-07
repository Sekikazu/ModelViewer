struct VS_IN
{
    float4 pos : POSITION0;
};
 
struct VS_OUT
{
    float4 pos  : SV_POSITION;
};

cbuffer ConstantBuffer
{
	float4x4 World;         //���[���h�ϊ��s��
	float4x4 View;          //�r���[�ϊ��s��
	float4x4 Projection;    //�����ˉe�ϊ��s��
	float4   Light;
	float4   LightColor;
	float4   Eye;
}
 
VS_OUT vs_wireframe( VS_IN input )
{
    VS_OUT output;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
 
    return output;
}