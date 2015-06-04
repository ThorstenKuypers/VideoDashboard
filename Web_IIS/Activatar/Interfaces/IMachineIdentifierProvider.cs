using System;

namespace Activatar
{
    public interface IMachineIdentifierProvider
    {
        byte[] MachineHash { get; }
        bool Match(byte[] machineHash);
    }
}
