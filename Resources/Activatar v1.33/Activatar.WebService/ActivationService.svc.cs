using System;
using System.ServiceModel;
using System.Runtime.Serialization;

using Activatar.Server;

namespace Activatar.WebService
{
    public class ActivationService : IActivationService
    {
        public ProductLicenseInfo ActivateProduct(string productKey, string machineHash)
        {
            string privateXmlKey = Helpers.GetResourceString("Activatar.WebService.RSAKeys.PrivateKey.xml");

            ProductActivation activation = new ProductActivation(privateXmlKey);
            ProductLicenseInfo activationInfo = activation.ActivateProduct(productKey, machineHash);

            return activationInfo;
        }
    }
}
