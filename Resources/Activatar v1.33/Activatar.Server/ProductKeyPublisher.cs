using System;
using System.IO;
using System.Text;
using System.Security.Cryptography;

namespace Activatar.Server
{
    public class ProductKeyPublisher
    {
        RSACryptoServiceProvider _cryptoService;

        #region Constructor
        public ProductKeyPublisher(string privateXmlKey)
        {
            _cryptoService = new RSACryptoServiceProvider();
            _cryptoService.FromXmlString(privateXmlKey);
            if (_cryptoService.PublicOnly)
            {
                throw new CryptographicException("A private key must be provided.");
            }
        }
        public ProductKeyPublisher(RSACryptoServiceProvider cryptoService)
        {
            _cryptoService = cryptoService;
        }
        #endregion

        #region GenerateProductKey
        public string GenerateProductKey(short productID, short productFeatures, short trialDays)
        {
            try
            {
                byte[] proid = BitConverter.GetBytes(productID);
                byte[] pinfo = BitConverter.GetBytes(productFeatures);
                byte[] xinfo = BitConverter.GetBytes(trialDays);
                byte[] ticks = BitConverter.GetBytes(DateTime.Now.Ticks);

                byte[] hiddenData;
                using (MemoryStream memStream = new MemoryStream())
                {
                    memStream.Write(proid, 0, 2);
                    memStream.Write(pinfo, 0, 2);
                    memStream.Write(xinfo, 0, 2);
                    memStream.Write(ticks, 0, 8);
                    hiddenData = memStream.ToArray();
                }

                byte[] sign = _cryptoService.SignData(proid, new SHA1CryptoServiceProvider());
                byte[] rkey = new byte[32];
                byte[] rjiv = new byte[16];
                Array.Copy(sign, rkey, 32);
                Array.Copy(sign, 32, rjiv, 0, 16);

                SymmetricAlgorithm algorithm = new RijndaelManaged();
                byte[] hiddenBytes = algorithm.CreateEncryptor(rkey, rjiv).TransformFinalBlock(hiddenData, 0, hiddenData.Length);

                byte[] keyBytes;
                using (MemoryStream stream = new MemoryStream())
                {
                    stream.Write(hiddenBytes, 0, 8);
                    stream.Write(proid, 0, 2);
                    stream.Write(hiddenBytes, 8, hiddenBytes.Length - 8);
                    keyBytes = stream.ToArray();
                }

                string productKey = Base32Converter.ToBase32String(keyBytes);

                return String.Format("{0}-{1}-{2}-{3}-{4}-{5}", productKey.Substring(0, 5), productKey.Substring(5, 5), productKey.Substring(10, 5), productKey.Substring(15, 5), productKey.Substring(20, 5), productKey.Substring(25, 5));
            }
            catch (Exception ex)
            {
                return String.Format("Error.{0}", ex.Message);
            }
        }
        #endregion

        #region ValidateProductKey
        public ProductKeyInfo ValidateProductKey(string productKey)
        {
            if (String.IsNullOrEmpty(productKey))
                throw new ArgumentNullException("Product Key is null or empty.");

            if (productKey.Length != 35)
                throw new ArgumentException("Product key is invalid.");

            productKey = productKey.Replace("-", "");

            byte[] keyBytes = Base32Converter.FromBase32String(productKey);

            byte[] signBytes = new byte[2];
            byte[] hiddenBytes = new byte[16];
            using (MemoryStream stream = new MemoryStream(keyBytes))
            {
                stream.Read(hiddenBytes, 0, 8);
                stream.Read(signBytes, 0, 2);
                stream.Read(hiddenBytes, 8, hiddenBytes.Length - 8);
                keyBytes = stream.ToArray();
            }

            byte[] sign = _cryptoService.SignData(signBytes, new SHA1CryptoServiceProvider());
            byte[] rkey = new byte[32];
            byte[] rjiv = new byte[16];
            Array.Copy(sign, rkey, 32);
            Array.Copy(sign, 32, rjiv, 0, 16);

            SymmetricAlgorithm algorithm = new RijndaelManaged();
            byte[] hiddenData;
            try
            {
                hiddenData = algorithm.CreateDecryptor(rkey, rjiv).TransformFinalBlock(hiddenBytes, 0, hiddenBytes.Length);
            }
            catch (Exception ex)
            {
                throw new InvalidProductKeyException("Product key is invalid.", ex);
            }

            byte[] proid = new byte[2];
            byte[] pinfo = new byte[2];
            byte[] xinfo = new byte[2];
            byte[] ticks = new byte[8];

            using (MemoryStream memStream = new MemoryStream(hiddenData))
            {
                memStream.Read(proid, 0, 2);
                memStream.Read(pinfo, 0, 2);
                memStream.Read(xinfo, 0, 2);
                memStream.Read(ticks, 0, 8);
            }

            if (signBytes[0] == proid[0] && signBytes[1] == proid[1])
            {
                DateTime generatedDate = new DateTime(BitConverter.ToInt64(ticks, 0));
                if (generatedDate.Year > 2000 && generatedDate.Year < 2100)
                {
                    return new ProductKeyInfo()
                    {
                        ProductID = BitConverter.ToInt16(proid, 0),
                        ProductFeatures = BitConverter.ToInt16(pinfo, 0),
                        TrialDays = BitConverter.ToInt16(xinfo, 0),
                        GeneratedDate = generatedDate
                    };
                }
                else
                {
                    throw new InvalidProductKeyException("Product key date is incorrect.");
                }
            }
            else
            {
                throw new InvalidProductKeyException("Product key info is incorrect.");
            }
        }
        #endregion
    }
}
