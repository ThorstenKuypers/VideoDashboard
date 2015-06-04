using System;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace Activatar
{
    [ServiceContract]
    public interface IActivationService
    {
        [OperationContract]
        ProductLicenseInfo ActivateProduct(string productKey, string machineHash);
    }
}
