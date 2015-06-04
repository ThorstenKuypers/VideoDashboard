using System;
using System.ServiceModel;
using System.Security.Cryptography;

using Activatar.Providers;
using Activatar.MachineIdentifiers;

namespace Activatar
{
    public class ProductLicenseManager
    {
        RSACryptoServiceProvider _cryptoService;

        private ILicenseStore _licenseStore;
        private IMachineIdentifierProvider _identifierService;

        public ProductLicenseManager(string publicXmlKey) :
            this(publicXmlKey, new FileLicenseStore(), new MachineIdentifierProvider(
                new IMachineIdentifier[] { new MachineNameIdentifier(), new NetworkAdapterIdentifier(), new VolumeInfoIdentifier() }))
        { }
        public ProductLicenseManager(string publicXmlKey, ILicenseStore licenseStore, IMachineIdentifierProvider identifierService)
        {
            _cryptoService = new RSACryptoServiceProvider();
            _cryptoService.FromXmlString(publicXmlKey);

            _licenseStore = licenseStore;
            _identifierService = identifierService;
        }

        public ProductLicense LoadLicense(string productName)
        {
            try
            {
                ProductLicenseInfo licenseInfo = _licenseStore.LoadLicense(productName);
                ProductLicense productLicense = new ProductLicense(_cryptoService, _identifierService, licenseInfo);
                return productLicense;
            }
            catch (LicenseNotFoundException ex)
            {
                return new ProductLicense(LicenseStatus.NotFound, ex.Message);
            }
            catch (Exception ex)
            {
                return new ProductLicense(LicenseStatus.Invalid, ex.Message);
            }
        }

        public ProductLicense ActivateProduct(string productKey)
        {
            try
            {
                ChannelFactory<IActivationService> factory = new ChannelFactory<IActivationService>("activationBinding");
                IActivationService service = factory.CreateChannel();

                ProductLicenseInfo licenseInfo = service.ActivateProduct(productKey, Convert.ToBase64String(_identifierService.MachineHash));

                if (licenseInfo.Signature != null)
                {
                    ProductLicense productLicense = new ProductLicense(_cryptoService, _identifierService, licenseInfo);
                    return productLicense;
                }
                else
                {
                    return new ProductLicense(LicenseStatus.Invalid, licenseInfo.ActivationInfo);
                }
            }
            catch (Exception ex)
            {
                return new ProductLicense(LicenseStatus.InternalError, ex.Message);
            }
        }

        public void SaveLicense(string productName, ProductLicense license)
        {
            if (String.IsNullOrEmpty(productName))
                throw new ArgumentNullException("ProductName is null or empty.");

            if (license == null)
                throw new ArgumentNullException("ProductLicense is null.");

            if (license.LicenseInfo == null)
                throw new InvalidOperationException("ProductLicense is not valid and can't be saved.");

            _licenseStore.SaveLicense(productName, license.LicenseInfo);
        }
    }
}
