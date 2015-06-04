using System;
using System.Text;
using System.Collections.Generic;

using Activatar.MachineIdentifiers;
using System.IO;

namespace Activatar.Providers
{
    public class MachineIdentifierProvider : IMachineIdentifierProvider
    {
        public List<IMachineIdentifier> MachineIdentifiers { get; private set; }

        public MachineIdentifierProvider()
        {
            MachineIdentifiers = new List<IMachineIdentifier>();
        }
        public MachineIdentifierProvider(IMachineIdentifier[] machineIdentifiers)
            : this()
        {
            MachineIdentifiers = new List<IMachineIdentifier>(machineIdentifiers);
        }

        public bool Match(byte[] machineHash)
        {
            int matchs = 0;

            using (MemoryStream stream = new MemoryStream(machineHash))
            {
                byte[] hash = new byte[16];
                for (int n = 0; n < MachineIdentifiers.Count; n++)
                {
                    if (stream.Read(hash, 0, 16) != 16)
                        break;
                    if (MachineIdentifiers[n].Match(hash))
                    {
                        matchs++;
                    }
                }
            }
            return matchs > 0;
        }

        public byte[] MachineHash
        {
            get
            {
                using (MemoryStream stream = new MemoryStream())
                {
                    for (int n = 0; n < MachineIdentifiers.Count; n++)
                    {
                        stream.Write(MachineIdentifiers[n].IdentifierHash, 0, 16);
                    }
                    return stream.ToArray();
                }
            }
        }
    }
}
