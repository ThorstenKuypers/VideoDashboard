using System;
using System.ServiceModel;
using System.Runtime.Serialization;

using Activatar.Server;

namespace Activatar.WebService
{
    [ServiceContract]
    public interface IProductKeyService
    {
        [OperationContract]
        string GenerateProductKey(short productID, short productFeatures, short trialDays);
    }

    public class ProductKeyService : IProductKeyService
    {
        public string GenerateProductKey(short productID, short productFeatures, short trialDays)
        {
            string privateXmlKey = Helpers.GetResourceString("Activatar.WebService.RSAKeys.PrivateKey.xml");

            ProductKeyPublisher keyPublisher = new ProductKeyPublisher(privateXmlKey);
            string productKey = keyPublisher.GenerateProductKey(productID, productFeatures, trialDays);

            return productKey;
        }
    }
}
