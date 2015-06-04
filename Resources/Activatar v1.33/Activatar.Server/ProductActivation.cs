using System;
using System.IO;
using System.Security.Cryptography;

namespace Activatar.Server
{
    public class ProductActivation
    {
        RSACryptoServiceProvider _cryptoService;

        #region Constructor
        public ProductActivation(string privateXmlKey)
        {
            _cryptoService = new RSACryptoServiceProvider();
            _cryptoService.FromXmlString(privateXmlKey);
            if (_cryptoService.PublicOnly)
            {
                throw new CryptographicException("A private key must be provided.");
            }
        }
        #endregion

        public ProductLicenseInfo ActivateProduct(string productKey, string machineHash)
        {
            try
            {
                ProductKeyPublisher keyPublisher = new ProductKeyPublisher(_cryptoService);
                ProductKeyInfo productInfo = keyPublisher.ValidateProductKey(productKey);

                byte[] proid = BitConverter.GetBytes(productInfo.ProductID);
                byte[] pinfo = BitConverter.GetBytes(productInfo.ProductFeatures);
                byte[] xinfo = BitConverter.GetBytes(productInfo.TrialDays);
                byte[] ticks = BitConverter.GetBytes(productInfo.GeneratedDate.Ticks);
                byte[] activ = BitConverter.GetBytes(DateTime.Now.Ticks);

                byte[] mhash = Convert.FromBase64String(machineHash);

                byte[] infoBytes;
                using (MemoryStream memStream = new MemoryStream())
                {
                    memStream.Write(proid, 0, 2);
                    memStream.Write(pinfo, 0, 2);
                    memStream.Write(xinfo, 0, 2);
                    memStream.Write(ticks, 0, 8);
                    memStream.Write(activ, 0, 8);
                    memStream.Write(mhash, 0, mhash.Length);
                    infoBytes = memStream.ToArray();
                }

                byte[] signBytes = _cryptoService.SignData(infoBytes, new SHA1CryptoServiceProvider());

                ProductLicenseInfo licenseInfo = new ProductLicenseInfo()
                {
                    ActivationInfo = Convert.ToBase64String(infoBytes),
                    Signature = Convert.ToBase64String(signBytes)
                };

                return licenseInfo;
            }
            catch (Exception ex)
            {
                return new ProductLicenseInfo()
                {
                    ActivationInfo = ex.Message,
                    Signature = null
                };
            }
        }
    }
}
