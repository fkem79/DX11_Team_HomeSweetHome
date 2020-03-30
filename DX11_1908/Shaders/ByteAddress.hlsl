struct Group
{
    uint3 groupID;
    uint3 groupThreadID;
    uint3 dispatchThreadID;
    uint groupIndex;
};

struct ComputeInput
{
    uint3 groupID : SV_GroupID;
    uint3 groupThreadID : SV_GroupThreadID;
    uint3 dispatchThreadID : SV_DispatchThreadID;
    uint groupIndex : SV_GroupIndex;
};

RWByteAddressBuffer output : register(u0);

[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
    Group group;
    group.groupID = asuint(input.groupID);
    group.groupThreadID = asuint(input.groupThreadID);
    group.dispatchThreadID = asuint(input.dispatchThreadID);
    group.groupIndex = asuint(input.groupIndex);
    
    uint index = input.groupID.x * 240 + input.groupIndex;
    uint address = index * 10 * 4;
    //uint address = input.groupIndex * 10 * 4;
    
    output.Store3(address + 0, asuint(group.groupID));
    output.Store3(address + 12, asuint(group.groupThreadID));
    output.Store3(address + 24, asuint(group.dispatchThreadID));
    output.Store(address + 36, asuint(group.groupIndex));
}
