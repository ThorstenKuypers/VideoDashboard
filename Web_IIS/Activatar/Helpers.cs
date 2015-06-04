using System;
using System.IO;
using System.Text;
using System.Security.Cryptography;

namespace Activatar
{
    public class Helpers
    {
        static public string GetResourceString(string resourceName)
        {
            Stream stream = System.Reflection.Assembly.GetCallingAssembly().GetManifestResourceStream(resourceName);
            if (stream == null)
                throw new FileNotFoundException("Resource not found.");
            using (StreamReader reader = new StreamReader(stream))
            {
                return reader.ReadToEnd();
            }
        }
    }
}
