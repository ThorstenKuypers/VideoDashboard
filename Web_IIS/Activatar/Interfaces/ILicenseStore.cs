using System;

namespace Activatar
{
    public interface ILicenseStore
    {
        ProductLicenseInfo LoadLicense(string productName);
        void SaveLicense(string productName, ProductLicenseInfo licenseInfo);
    }
}
