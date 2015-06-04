using System;
using System.IO;
using System.Reflection;

namespace Activatar.Tests
{
    public class KeyHelpers
    {
        static public string GetPrivateKey()
        {
            return Helpers.GetResourceString("Activatar.Tests.RSAKeys.PrivateKey.xml");
        }

        static public string GetPublicKey()
        {
            return Helpers.GetResourceString("Activatar.Tests.RSAKeys.PublicKey.xml");
        }
    }
}
