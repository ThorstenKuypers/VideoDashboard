using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace DatabaseService
{

    [DataContract]
    public class DatabaseServiceException
    {
        private string _message;

        [DataMember]
        public string Message
        {
            get { return _message; }
            private set
            {
                _message = value;
            }
        }

        public DatabaseServiceException(string msg)
        {
            _message = msg;
        }
    }
}
