using System;
using System.Runtime.Serialization;
using System.Security.Cryptography;

namespace Activatar
{
    #region LicenseStatus
    public enum LicenseStatus
    {
        Licensed,
        TrialVersion,
        Expired,
        MachineHashMismatch,
        NotFound,
        Invalid,
        InternalError
    }
    #endregion

    public class ProductLicense
    {
        private RSACryptoServiceProvider _cryptoService;
        private IMachineIdentifierProvider _identifierService;

        public ProductLicenseInfo LicenseInfo { get; private set; }

        public short ProductID { get; private set; }
        public short ProductFeatures { get; private set; }
        public short TrialDays { get; private set; }
        public DateTime ProductKeyCreationDate { get; private set; }
        public DateTime ActivationDate { get; private set; }
        public int TrialDaysLeft { get; private set; }

        public LicenseStatus Status { get; private set; }
        public string StatusReason { get; private set; }

        internal ProductLicense(LicenseStatus status, string statusReason) { Status = status; StatusReason = statusReason; }

        public ProductLicense(RSACryptoServiceProvider cryptoService, IMachineIdentifierProvider identifierService, ProductLicenseInfo licenseInfo)
        {
            _cryptoService = cryptoService;
            _identifierService = identifierService;
            LicenseInfo = licenseInfo;
            ProcessLincense();
        }

        private void ProcessLincense()
        {
            try
            {
                byte[] dataBytes = Convert.FromBase64String(LicenseInfo.ActivationInfo);
                byte[] signBytes = Convert.FromBase64String(LicenseInfo.Signature);

                if (_cryptoService.VerifyData(dataBytes, new SHA1CryptoServiceProvider(), signBytes))
                {
                    int infoLength = 22; // ProductID (2) + ProductFeatures (2) + TrialDays (2) + CreationDate (8) + ActivationDate (8) = 22
                    byte[] hash = new byte[dataBytes.Length - infoLength];
                    Buffer.BlockCopy(dataBytes, infoLength, hash, 0, hash.Length);
                    if (_identifierService.Match(hash))
                    {
                        ProductID = BitConverter.ToInt16(dataBytes, 0);
                        ProductFeatures = BitConverter.ToInt16(dataBytes, 2);
                        TrialDays = BitConverter.ToInt16(dataBytes, 4);
                        ProductKeyCreationDate = new DateTime(BitConverter.ToInt64(dataBytes, 6));
                        ActivationDate = new DateTime(BitConverter.ToInt64(dataBytes, 14));

                        if (TrialDays == 0)
                        {
                            Status = LicenseStatus.Licensed;
                            StatusReason = String.Empty;
                            TrialDaysLeft = Int32.MaxValue;
                        }
                        else
                        {
                            TrialDaysLeft = (TrialDays - (DateTime.Today - ActivationDate.Date).Days);
                            if (TrialDaysLeft > 0)
                            {
                                Status = LicenseStatus.TrialVersion;
                                StatusReason = String.Format("{0} days left.", TrialDaysLeft);
                            }
                            else
                            {
                                Status = LicenseStatus.Expired;
                                StatusReason = String.Format("License expired {0} days ago.", -TrialDaysLeft);
                            }
                        }
                    }
                    else
                    {
                        Status = LicenseStatus.MachineHashMismatch;
                        StatusReason = "Machine and product info hash mismatch.";
                    }
                }
                else
                {
                    Status = LicenseStatus.Invalid;
                    StatusReason = "Failed verifying signature.";
                }
            }
            catch (Exception ex)
            {
                Status = LicenseStatus.Invalid;
                StatusReason = ex.Message;
            }
        }
    }
}
