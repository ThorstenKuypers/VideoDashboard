using System;
using System.IO;
using System.Xml.Serialization;

namespace Activatar.Providers
{
    class FileLicenseStore : ILicenseStore
    {
        public ProductLicenseInfo LoadLicense(string productName)
        {
            if (File.Exists(productName))
            {
                using (StreamReader reader = new StreamReader(productName))
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(ProductLicenseInfo));
                    return (ProductLicenseInfo)serializer.Deserialize(reader);
                }
            }
            else
            {
                throw new LicenseNotFoundException("License not found.");
            }
        }

        public void SaveLicense(string productName, ProductLicenseInfo licenseInfo)
        {
            using (StreamWriter writer = new StreamWriter(productName, false))
            {
                XmlSerializer serializer = new XmlSerializer(typeof(ProductLicenseInfo));
                serializer.Serialize(writer, licenseInfo);
            }
        }
    }
}
