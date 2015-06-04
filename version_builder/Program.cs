using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;

using Rhino.Licensing;

namespace version_builder
{
    public struct VersionInfo
    {
        public int major;
        public int minor;
        public int revision;
        public int release;
    }

    class Program
    {
        private static readonly string ProductName = "VideoDashboard";

        static void Main(string[] args)
        {
            // arg[1] - defines command (what to do)
            // arg[2] - defines resource (where to do)
            // arg[3] - defines assembly or additional file (which file or assembly)
            if (args[0] == "version")
            {
                string project = args[1];
                string productVersionFilePath = args[2];
                string resFile = args[3];
                string fileVersionFilePath = "";

                if (args.Length > 4)
                    fileVersionFilePath = args[4];
                else
                    fileVersionFilePath = productVersionFilePath;

                VersionInfo productVer = new VersionInfo();
                VersionInfo fileVer = new VersionInfo();

                string f = productVersionFilePath + "\\VERSION";
                if (File.Exists(f))
                {
                    string s = File.ReadAllText(f);
                    string[] toks = s.Split(new char[] { '.' });
                    productVer.major = int.Parse(toks[0]);
                    productVer.minor = int.Parse(toks[1]);
                    productVer.revision = int.Parse(toks[2]);
                    productVer.release = int.Parse(toks[3]);

                    if (project == ProductName)
                    {
                        productVer.release++;
                        s = string.Format("{0}.{1}.{2}.{3}", new object[]{productVer.major,
                                                                    productVer.minor,
                                                                    productVer.revision,
                                                                    productVer.release});

                        File.WriteAllText(f, s);
                        Console.WriteLine("[*] VideoDashboard Version: {0}.{1}.{2}.{3}", productVer.major, productVer.minor, productVer.revision, productVer.release);
                    }
                }

                if (args.Length > 4)
                {
                    f = fileVersionFilePath + "\\VERSION";
                    if (File.Exists(f))
                    {
                        string s = File.ReadAllText(f);
                        string[] toks = s.Split(new char[] { '.' });
                        fileVer.major = int.Parse(toks[0]);
                        fileVer.minor = int.Parse(toks[1]);
                        fileVer.revision = int.Parse(toks[2]);
                        fileVer.release = int.Parse(toks[3]);

                        fileVer.release++;
                        s = string.Format("{0}.{1}.{2}.{3}", new object[]{fileVer.major,
                                                                    fileVer.minor,
                                                                    fileVer.revision,
                                                                    fileVer.release});

                        File.WriteAllText(f, s);
                        Console.WriteLine("[*] {0} Version: {1}.{2}.{3}.{4}", new object[] { project, fileVer.major, fileVer.minor, fileVer.revision, fileVer.release });
                    }
                }
                else
                    fileVer = productVer;

                int pos = resFile.LastIndexOf('.');
                if (pos != -1)
                {
                    string[] lines = File.ReadAllLines(resFile);
                    string s = resFile.Substring(pos + 1, (resFile.Length - pos) - 1);
                    if (s == "rc")
                    {
                        // write to rc resource file
                        for (int i = 0; i < lines.Length; i++)
                        {
                            if (lines[i].Contains("FILEVERSION"))
                                lines[i] = string.Format(" FILEVERSION {0},{1},{2},{3}", fileVer.major, fileVer.minor, fileVer.revision, fileVer.release);

                            if (lines[i].Contains("PRODUCTVERSION"))
                                lines[i] = string.Format(" PRODUCTVERSION {0},{1},{2},{3}", productVer.major, productVer.minor, productVer.revision, productVer.release);

                            if (lines[i].Contains("FileVersion"))
                                lines[i] = string.Format("            VALUE \"FileVersion\", \"{0}.{1}.{2}.{3}\"", fileVer.major, fileVer.minor, fileVer.revision, fileVer.release);

                            if (lines[i].Contains("ProductVersion"))
                                lines[i] = string.Format("            VALUE \"ProductVersion\", \"{0}.{1}.{2}.{3}\"", productVer.major, productVer.minor, productVer.revision, productVer.release);

                        }

                        File.WriteAllLines(resFile, lines);
                   }
                    else if (s == "cs")
                    {
                        // write to cs file
                        for (int i = 0; i < lines.Length; i++)
                        {
                            if (lines[i].Contains("AssemblyVersion") && !lines[i].StartsWith("//"))
                            {
                                lines[i] = string.Format("[assembly: AssemblyVersion(\"{0}.{1}.{2}.{3}\")]", productVer.major, productVer.minor, productVer.revision, productVer.release);
                            }
                            if (lines[i].Contains("AssemblyFileVersion") && !lines[i].StartsWith("//"))
                            {
                                lines[i] = string.Format("[assembly: AssemblyFileVersion(\"{0}.{1}.{2}.{3}\")]", fileVer.major, fileVer.minor, fileVer.revision, fileVer.release);
                            }
                        }

                        File.WriteAllLines(resFile, lines);
                    }
                    else
                    {
                        Console.WriteLine("[-] unknown resource file type");
                        return;
                    }
                }
            }
            if (args[0] == "license")
            {
                // create license (args[1] define type of license)

                string path = ".\\";
                if (args.Length > 1)
                    path = args[1] + "\\";

                Console.Write("[*] generating new beta license...");
                string s = File.ReadAllText(path + "privateKey.xml");
                LicenseGenerator gen = new LicenseGenerator(s);
                DateTime exp = new DateTime(2014, 12, 31, 23, 59, 59);
                exp = exp.ToUniversalTime();
                string lic = gen.Generate("beta_test", Guid.NewGuid(), exp, LicenseType.Trial);

                File.WriteAllText(path + "license.lic", lic);
                Console.WriteLine("DONE\n");
            }
            if (args[0] == "keygen")
            {
                string path = ".\\";
                if (args.Length > 1)
                    path = args[1] + "\\";

                Console.Write("[*] creating RSA key pair with 2048 bit key size...");
                // generate license RSA keys
                RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(2048);
                string privateKey = rsa.ToXmlString(true);
                string publicKey = rsa.ToXmlString(false);

                Console.WriteLine("DONE\n");

                Console.Write("[*] writing keys to file...");
                File.WriteAllText(path + "publicKey.xml", publicKey);
                File.WriteAllText(path + "privateKey.xml", privateKey);

                Console.WriteLine("DONE\n");
            }
            if (args[0] == "prepare-embed")
            {
                // copy files for embedding
            }
        }
    }

    public enum AssemblyType
    {
        Product = 1,
        libDataLogging = 2,
        libds = 3,
        libLDF = 4,
        customControls = 5,
        TVOFilter = 6
    }

    public class VersionCreator
    {
        public VersionInfo ProductVersion;
        public VersionInfo LDFVersion;
        public VersionInfo DataLoggerVersion;
        public VersionInfo DShowVersion;
        public VersionInfo FilterVersion;


        public void LoadVersioningInfo()
        {
            StreamReader r = new StreamReader(".\\VERSION", Encoding.ASCII);
            char[] buf = new char[r.BaseStream.Length];
            r.ReadBlock(buf, 0, buf.Length);

            string file = new string(buf);
            string[] lines = file.Split(new char[] { '\n' });
            foreach (string ln in lines)
            {
                if (ln.StartsWith("VERSION"))
                {
                    string[] toks = ln.Split(new char[] { '=' });
                    toks[1].Trim();
                    if (toks.Length == 2)
                    {
                        toks = toks[1].Split(new char[] { '.' });
                        ProductVersion.major = int.Parse(toks[0]);
                        ProductVersion.minor = int.Parse(toks[1]);
                        ProductVersion.revision = int.Parse(toks[2]);
                        ProductVersion.release = int.Parse(toks[3]);
                    }

                }
                else if (ln.StartsWith("LDF"))
                {
                    string[] toks = ln.Split(new char[] { '=' });
                    toks[1].Trim();
                    if (toks.Length == 2)
                    {
                        toks = toks[1].Split(new char[] { '.' });
                        LDFVersion.major = int.Parse(toks[0]);
                        LDFVersion.minor = int.Parse(toks[1]);
                        LDFVersion.revision = int.Parse(toks[2]);
                        LDFVersion.release = int.Parse(toks[3]);
                    }
                }
                else if (ln.StartsWith("DATALOG"))
                {
                    string[] toks = ln.Split(new char[] { '=' });
                    toks[1].Trim();
                    if (toks.Length == 2)
                    {
                        toks = toks[1].Split(new char[] { '.' });
                        DataLoggerVersion.major = int.Parse(toks[0]);
                        DataLoggerVersion.minor = int.Parse(toks[1]);
                        DataLoggerVersion.revision = int.Parse(toks[2]);
                        DataLoggerVersion.release = int.Parse(toks[3]);
                    }
                }
                else if (ln.StartsWith("DS"))
                {
                    string[] toks = ln.Split(new char[] { '=' });
                    toks[1].Trim();
                    if (toks.Length == 2)
                    {
                        toks = toks[1].Split(new char[] { '.' });
                        DShowVersion.major = int.Parse(toks[0]);
                        DShowVersion.minor = int.Parse(toks[1]);
                        DShowVersion.revision = int.Parse(toks[2]);
                        DShowVersion.release = int.Parse(toks[3]);
                    }
                }
                else if (ln.StartsWith("FILTER"))
                {
                    string[] toks = ln.Split(new char[] { '=' });
                    toks[1].Trim();
                    if (toks.Length == 2)
                    {
                        toks = toks[1].Split(new char[] { '.' });
                        FilterVersion.major = int.Parse(toks[0]);
                        FilterVersion.minor = int.Parse(toks[1]);
                        FilterVersion.revision = int.Parse(toks[2]);
                        FilterVersion.release = int.Parse(toks[3]);
                    }
                }
            }
        }
        public void VersioningAssembly(AssemblyType type)
        {
            switch (type)
            {
                case AssemblyType.Product:
                    break;

                case AssemblyType.customControls:
                    break;

                case AssemblyType.libDataLogging:
                    break;

                case AssemblyType.libds:
                    break;

                case AssemblyType.libLDF:
                    break;

                case AssemblyType.TVOFilter:

                    break;

                default:
                    Console.WriteLine("undefined project");
                    break;
            }
        }
    }
}
