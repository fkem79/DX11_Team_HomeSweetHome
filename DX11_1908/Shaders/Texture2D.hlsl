Texture2DArray<float4> input;
RWTexture2DArray<float4> output;

[numthreads(32, 32, 1)]
void CS( uint3 DTid : SV_DispatchThreadID )
{
    float4 color = input.Load(int4(DTid, 0));
    
    //output[DTid] = float4(1.0f - color.rgb, 1);           //¹ÝÀü
    //float average = (color.r + color.g + color.b) / 3;    //Èæ¹é
    //output[DTid] = float4(average.xxx, 1.0f);             //Èæ¹é
    
    output[DTid] = float4(color.rgb, 1);
}