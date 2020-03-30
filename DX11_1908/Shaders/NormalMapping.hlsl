//Normal Mapping
cbuffer VP : register(b0)
{
    matrix view;
    matrix projection;
    matrix invView;
}

cbuffer W : register(b1)
{
    matrix world;
}

cbuffer Light : register(b0)
{
    float3 lightDir;
    
    float specExp;
}

cbuffer Mapping : register(b1)
{
    int isSpecularMap;
    int isNormalMap;
}

Texture2D diffuseMap : register(t0);
SamplerState diffuseSamp : register(s0);

Texture2D specularMap : register(t1);
SamplerState specularSamp : register(s1);

Texture2D normalMap : register(t2);
SamplerState normalSamp : register(s2);

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;    
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(diffuseSamp, input.uv);
    
    float3 light = normalize(lightDir);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 normal = N;
    if(isNormalMap)
    {
        float4 normalMapping = normalMap.Sample(normalSamp, input.uv);
    
        float3x3 TBN = float3x3(T, B, N);
    
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }    
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuse > 0)
    {
        //Phong Shading
        //float3 reflection = normalize(reflect(light, normal));
        //specular = saturate(dot(reflection, -input.viewDir));
        
        //Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specularIntensity = 1;
        if(isSpecularMap)
        {
            specularIntensity = specularMap.Sample(specularSamp, input.uv);
        }
        
        specular = pow(specular, specExp) * specularIntensity;
    }
    
    float4 ambient = albedo * 0.1f;
    
    return albedo * diffuse + specular + ambient;
}