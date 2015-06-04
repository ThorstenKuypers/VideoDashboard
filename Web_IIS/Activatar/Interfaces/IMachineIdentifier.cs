using System;

namespace Activatar
{
    public interface IMachineIdentifier
    {
        byte[] IdentifierHash { get;}
        bool Match(byte[] identifierHash);
    }
}
