using System;

namespace Activatar.Server
{
    public class ProductKeyInfo
    {
        public short ProductID { get; set; }
        public short ProductFeatures { get; set; }
        public short TrialDays { get; set; }
        public DateTime GeneratedDate { get; set; }
    }
}
