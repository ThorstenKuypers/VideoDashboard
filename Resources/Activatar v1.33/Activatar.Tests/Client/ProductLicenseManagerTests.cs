using System;
using System.Security.Cryptography;

using Microsoft.VisualStudio.TestTools.UnitTesting;

using Activatar.Tests.Mocks;

namespace Activatar.Tests.Client
{
    [TestClass]
    public class ProductLicenseManagerTests
    {
        [TestMethod]
        public void ActivateProduct()
        {
            string productKey = GenerateProductKey(123, 456, 0);

            string publicXmlKey = KeyHelpers.GetPrivateKey();
            ProductLicenseManager licenseManager = new ProductLicenseManager(publicXmlKey);
            ProductLicense license = licenseManager.ActivateProduct(productKey);

            Assert.AreEqual(LicenseStatus.Licensed, license.Status, license.StatusReason);
            Assert.AreEqual(123, license.ProductID);
            Assert.AreEqual(456, license.ProductFeatures);
            Assert.AreEqual(0, license.TrialDays);
        }

        [TestMethod]
        public void ActivateTrialProduct()
        {
            string productKey = GenerateProductKey(123, 456, 30);

            string publicXmlKey = KeyHelpers.GetPrivateKey();
            ProductLicenseManager licenseManager = new ProductLicenseManager(publicXmlKey);
            ProductLicense license = licenseManager.ActivateProduct(productKey);

            Assert.AreEqual(LicenseStatus.TrialVersion, license.Status, license.StatusReason);
            Assert.AreEqual(123, license.ProductID);
            Assert.AreEqual(456, license.ProductFeatures);
            Assert.AreEqual(30, license.TrialDays);
            Assert.AreEqual(30, license.TrialDaysLeft);
        }

        [TestMethod]
        public void ActivateExpiredProduct()
        {
            string productKey = GenerateProductKey(123, 456, -1);

            string publicXmlKey = KeyHelpers.GetPrivateKey();
            ProductLicenseManager licenseManager = new ProductLicenseManager(publicXmlKey);
            ProductLicense license = licenseManager.ActivateProduct(productKey);

            Assert.AreEqual(LicenseStatus.Expired, license.Status, license.StatusReason);
            Assert.AreEqual(123, license.ProductID);
            Assert.AreEqual(456, license.ProductFeatures);
            Assert.AreEqual(-1, license.TrialDays);
            Assert.AreEqual(-1, license.TrialDaysLeft);
        }

        [TestMethod]
        public void ActivateProductMachineMismatch()
        {
            MachineIdentifierProviderMock machineMock = new MachineIdentifierProviderMock(false);
            string productKey = GenerateProductKey(123, 456, 0);

            string publicXmlKey = KeyHelpers.GetPrivateKey();
            ProductLicenseManager licenseManager = new ProductLicenseManager(publicXmlKey, null, machineMock);
            ProductLicense license = licenseManager.ActivateProduct(productKey);

            Assert.AreEqual(LicenseStatus.MachineHashMismatch, license.Status, license.StatusReason);
        }

        [TestMethod]
        public void LoadSaveLicense()
        {
            string publicXmlKey = KeyHelpers.GetPrivateKey();
            ProductLicenseManager licenseManager = new ProductLicenseManager(publicXmlKey);
            ProductLicense license = licenseManager.LoadLicense("MyProductName");

            Assert.AreEqual(LicenseStatus.NotFound, license.Status, license.StatusReason);

            license = licenseManager.ActivateProduct(GenerateProductKey(123, 456, 0));
            licenseManager.SaveLicense("MyProductName", license);
            license = licenseManager.LoadLicense("MyProductName");

            Assert.AreEqual(LicenseStatus.Licensed, license.Status, license.StatusReason);
            Assert.AreEqual(123, license.ProductID);
            Assert.AreEqual(456, license.ProductFeatures);
            Assert.AreEqual(0, license.TrialDays);
        }

        private string GenerateProductKey(short productID, short productFeatures, short trialDays)
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            Activatar.Server.ProductKeyPublisher keyPublisher = new Activatar.Server.ProductKeyPublisher(privateXmlKey);

            return keyPublisher.GenerateProductKey(productID, productFeatures, trialDays);
        }
    }
}
