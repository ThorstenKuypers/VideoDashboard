using System;
using System.Runtime.Serialization;

namespace Activatar
{
    [Serializable]
    public class LicenseNotFoundException : Exception
    {
        public LicenseNotFoundException() { }
        public LicenseNotFoundException(string message) : base(message) { }
        public LicenseNotFoundException(string message, Exception inner) : base(message, inner) { }
        protected LicenseNotFoundException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }

    [Serializable]
    public class InvalidLicenseException : Exception
    {
        public InvalidLicenseException() { }
        public InvalidLicenseException(string message) : base(message) { }
        public InvalidLicenseException(string message, Exception inner) : base(message, inner) { }
        protected InvalidLicenseException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }

    [Serializable]
    public class InvalidProductKeyException : Exception
    {
        public InvalidProductKeyException() { }
        public InvalidProductKeyException(string message) : base(message) { }
        public InvalidProductKeyException(string message, Exception inner) : base(message, inner) { }
        protected InvalidProductKeyException(SerializationInfo info, StreamingContext context) : base(info, context) { }
    }
}
