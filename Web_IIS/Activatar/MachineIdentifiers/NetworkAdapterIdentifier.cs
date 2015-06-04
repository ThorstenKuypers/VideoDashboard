using System;
using System.Management;

namespace Activatar.MachineIdentifiers
{
    public class NetworkAdapterIdentifier : MachineIdentifierBase, IMachineIdentifier
    {
        protected override byte[] GetIdentifierHash()
        {
            string identifier = "NOTFOUND";
            try
            {
                using (ManagementClass management = new ManagementClass("Win32_NetworkAdapterConfiguration"))
                {
                    using (ManagementObjectCollection managementObjects = management.GetInstances())
                    {
                        foreach (var item in managementObjects)
                        {
                            if ((bool)item["IPEnabled"])
                            {
                                identifier = item["MacAddress"].ToString();
                                break;
                            }
                        }
                    }
                }
            }
            catch { }
            return base.ComputeHash(identifier);
        }
    }
}
