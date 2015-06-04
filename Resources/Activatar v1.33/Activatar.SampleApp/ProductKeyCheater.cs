using System;
using System.ServiceModel;
using System.Windows.Forms;

namespace Activatar.SampleApp
{
    [ServiceContract]
    public interface IProductKeyService
    {
        [OperationContract]
        string GenerateProductKey(short productID, short productFeatures, short trialDays);
    }

    class ProductKeyCheater
    {
        public string GenerateKey(short productID, short productFeatures, short trialDays)
        {
            try
            {
                ChannelFactory<IProductKeyService> factory = new ChannelFactory<IProductKeyService>("productKeyBinding");
                IProductKeyService service = factory.CreateChannel();

                return service.GenerateProductKey(productID, productFeatures, trialDays);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Exception", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return "Error";
            }
        }
    }
}
