using System;
using System.Text;

namespace Activatar.Server
{
    public class Base32Converter
    {
        #region BASE32_TABLE
        static readonly char[] BASE32_TABLE = new char[]
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
            'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P',
            'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'
        };
        #endregion

        static public string ToBase32String(byte[] buffer)
        {
            char[] chars = new char[buffer.Length * 2];

            int mod = buffer.Length % 3;
            if (mod != 0)
                throw new InvalidOperationException("Input data incorrect. Required multiple of 3 bytes length.");

            int res = buffer.Length - mod;
            int inx = 0;

            for (int i = 0; i < res; i += 3)
            {
                chars[inx + 0] = BASE32_TABLE[(buffer[i] & 0xf8) >> 3];
                chars[inx + 1] = BASE32_TABLE[((buffer[i] & 0x07) << 2) | ((buffer[i + 1] & 0xc0) >> 6)];
                chars[inx + 2] = BASE32_TABLE[((buffer[i + 1] & 0x3e) >> 1)];
                chars[inx + 3] = BASE32_TABLE[((buffer[i + 1] & 0x01) << 4) | ((buffer[i + 2] & 0xf0) >> 4)];
                chars[inx + 4] = BASE32_TABLE[(buffer[i + 2] & 0x0f)];
                inx += 5;
            }

            return new String(chars, 0, inx);
        }

        static public byte[] FromBase32String(string base32)
        {
            byte[] bytes = new byte[base32.Length];

            int mod = base32.Length % 5;
            if (mod != 0)
                throw new InvalidOperationException("Base32 input string incorrect. Required multiple of 5 character length.");

            int res = base32.Length - mod;
            int inx = 0;

            long triple;

            for (int i = 0; i < res; i += 5)
            {
                triple = GetBase32Number(base32[i]) << 19;
                triple = triple | ((long)GetBase32Number(base32[i + 1]) << 14);
                triple = triple | ((long)GetBase32Number(base32[i + 2]) << 9);
                triple = triple | ((long)GetBase32Number(base32[i + 3]) << 4);
                triple = triple | ((byte)GetBase32Number(base32[i + 4]));

                bytes[inx + 0] = (byte)((triple & 0x00ff0000) >> 16);
                bytes[inx + 1] = (byte)((triple & 0x0000ff00) >> 8);
                bytes[inx + 2] = (byte)(triple & 0x000000ff);

                inx += 3;
            }
            byte[] buffer = new byte[inx];
            Array.Copy(bytes, 0, buffer, 0, inx);
            return buffer;
        }

        static private int GetBase32Number(char c)
        {
            if (c == 'I' || c == 'O' || c == 'Q' || c == 'U')
                throw new ArgumentOutOfRangeException();

            int number = c - 0x30;

            if (number > 9)
            {
                number = number - 0x10;
                if (number > 9)
                {
                    number--;
                    if (number > 13)
                    {
                        number--;
                        if (number > 14)
                        {
                            number--;
                            if (number > 17)
                            {
                                number--;
                            }
                        }
                    }
                }
                number += 9;
            }

            if (number < 0 || number > 31)
                throw new ArgumentOutOfRangeException();

            return number;
        }
    }
}
