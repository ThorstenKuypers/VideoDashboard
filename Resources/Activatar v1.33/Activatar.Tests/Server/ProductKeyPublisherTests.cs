using System;
using System.Security.Cryptography;

using Microsoft.VisualStudio.TestTools.UnitTesting;

using Activatar.Server;

namespace Activatar.Tests.Server
{
    [TestClass]
    public class ProductKeyPublisherTests
    {
        [TestMethod]
        public void GenerateProductKey()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey1 = keyPublisher.GenerateProductKey(0, 0, 0);
            Console.WriteLine(productKey1);
            string productKey2 = keyPublisher.GenerateProductKey(0, 0, 0);
            Console.WriteLine(productKey2);
            string productKey3 = keyPublisher.GenerateProductKey(0, 0, 0);
            Console.WriteLine(productKey3);
            string productKey4 = keyPublisher.GenerateProductKey(0, 0, 0);
            Console.WriteLine(productKey4);

            Assert.AreNotEqual(productKey1, productKey2);
            Assert.AreNotEqual(productKey1, productKey3);
            Assert.AreNotEqual(productKey1, productKey4);
            Assert.AreNotEqual(productKey2, productKey3);
            Assert.AreNotEqual(productKey2, productKey4);
            Assert.AreNotEqual(productKey3, productKey4);
        }

        [TestMethod]
        public void ValidateProductKey()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey = keyPublisher.GenerateProductKey(123, 456, 789);
            Console.WriteLine(productKey);

            keyPublisher = new ProductKeyPublisher(privateXmlKey);
            ProductKeyInfo result = keyPublisher.ValidateProductKey(productKey);

            Assert.AreEqual(123, result.ProductID);
            Assert.AreEqual(456, result.ProductFeatures);
            Assert.AreEqual(789, result.TrialDays);
            Assert.AreEqual(DateTime.Now.Date, result.GeneratedDate.Date);
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidProductKeyException))]
        public void ValidateFakeProductKey()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey = keyPublisher.GenerateProductKey(123, 456, 789);
            Console.WriteLine(productKey);
            productKey = productKey.Replace('1', '8');
            productKey = productKey.Replace('2', '7');
            productKey = productKey.Replace('3', '6');
            productKey = productKey.Replace('4', '5');
            Console.WriteLine(productKey);

            keyPublisher = new ProductKeyPublisher(privateXmlKey);
            ProductKeyInfo result = keyPublisher.ValidateProductKey(productKey);
        }

        [TestMethod]
        [ExpectedException(typeof(CryptographicException))]
        public void TryValidateWithPublicKey()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey = keyPublisher.GenerateProductKey(123, 456, 789);
            Console.WriteLine(productKey);

            privateXmlKey = KeyHelpers.GetPublicKey();
            keyPublisher = new ProductKeyPublisher(privateXmlKey);
            ProductKeyInfo result = keyPublisher.ValidateProductKey(productKey);
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidProductKeyException))]
        public void TryValidateWithFakeCryptProvider()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey = keyPublisher.GenerateProductKey(123, 456, 789);
            Console.WriteLine(productKey);

            privateXmlKey = (new RSACryptoServiceProvider()).ToXmlString(true);
            keyPublisher = new ProductKeyPublisher(privateXmlKey);
            ProductKeyInfo result = keyPublisher.ValidateProductKey(productKey);
        }

        [TestMethod]
        public void TryRandomKeyGeneration()
        {
            string privateXmlKey = KeyHelpers.GetPrivateKey();
            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);

            string productKey = keyPublisher.GenerateProductKey(0, 0, 0);
            Console.WriteLine(productKey);

            keyPublisher.ValidateProductKey(productKey);

            for (int n = 1; n < 0x100; n++)
            {
                byte[] bytes = BitConverter.GetBytes(n);
                string fakeSegment = Base32Converter.ToBase32String(new byte[] {bytes[2], bytes[1], bytes[0]});
                if (fakeSegment != productKey.Substring(6))
                {
                    productKey = String.Format("{0}{1}", fakeSegment, productKey.Substring(5));
                    try
                    {
                        keyPublisher.ValidateProductKey(productKey);
                        Assert.Fail("This should raise an InvalidProductKeyException");
                    }
                    catch (InvalidProductKeyException)
                    {
                        // OK. We are expecting this exception.
                    }
                    catch (Exception ex)
                    {
                        throw ex;
                    }
                }
            }
        }
    }
}
