using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

using System.Xml;
using System.Security.Cryptography; // for MD5 hashes

namespace VideoDashboard
{
    public struct ApplicationSettings
    {
        public String appPath; // application root path
        public List<string> dashPaths;	// path to dashboard layout files
        //public String imgPath;	// path to dashboard images
        //public String projectPath; // path to which project files are saved
        public String videoProfilesPath; // path where WMV video profiles are located

        public Units units;

        // new v0.8.2.3 - not used yet!
        // These information are saved when the user closes the application, to restore the window
        // position and size the next time when the application is started
        public int wndPosX; // initial X coordinate for window
        public int wndPosY;
        public int wndWidth;
        public int wndHeight;
    }

    public struct VideoSyncData
    {
        public int startFrame;	// start at video frame #:
        public int endFrame;	// to video frame #:
        public int startSample;	// data sample index to start with
        public int endSample;		// data sample index last
        //public int dataSampleInc;	// data sample index increment (depending on video framerate to data tick rate)
    }

    public struct RenderConfigData
    {
        public VideoEncoderType encoder;
        public int wmvProfileIndex;
        public string wmvProfileFileName;

        public int rawCompressorIndex;
        public string rawCompressorName;

        public int cbOutputWidthIdx;
        public int cbAspectRatioIdx;

        //public IntPtr wmvProfilerObj;
        //public IntPtr compressorObj;
    }

    public enum Units
    {
        Units_undefined = -1,
        Units_Metric = 0,
        Units_English
    }

    public struct InputVideoInfo
    {
        public String fileName;
        public int width;
        public int height;
        public int framerate;
        public int avgTimeFrame;	// average time/frame
        public float aspectR;
    }

    public struct OutputVideoInfo
    {
        public String fileName;
        public int width;
        public int height;
        public int framerate;
        public int avgTimeFrame;	// average time/frame
        public float aspectR;
        public VideoSyncData syncData;
        public OverlayConfig overlayCfg;
    }

    public struct OverlayConfig
    {
        public string name;
        public int posX;
        public int posY;
        public int transp;
        public int scale;
    }

    public struct ProjectInfo
    {
        public String projectName;
        public String projectFileName;
        public String projectDescription;

        public IntPtr DataLoggerObj;
        //public IntPtr DashboardOverlayObj;
        public string dashboardFileName;

        public InputVideoInfo inVideoData;
        public OutputVideoInfo outVideoData;
        public RenderConfigData renderCfg;
        public String dataFile;

        //public ProjectSettings settings;
        public ApplicationSettings appSet;

        public MD5 projectFileHash;
    }

    /// <summary>
    /// /////////////////////////// NEW project structure (class) layout v0.8.x
    /// </summary>
    /// 

    public struct SourceVideoInfo
    {
        public String fileName;
        public int width;
        public int height;
        public int framerate;
        public int avgTimeFrame;	// average time/frame
        public float aspectRatio;
    }

    public struct DataLoggerInfo
    {
        public String dataLogFileName;
        public int dataLogger;
    }

    public struct OverlaySettings
    {
        public String overlayFileName;
        public float scale;			// selected scale factor
        public float transp;			// selected transparency value
        public int margin;			// selected bounding border value
    }

    public struct RenderSettings
    {
        public String targetFileName;
        public int renderer;

        public int width;
        public int height;
        public int framerate;
        public int avgTimeFrame;	// average time/frame
        public float aspectRatio;

        public VideoSyncData syncInfo;
    }

    /////////////////////////////////////////////////////////////////////////////


    /// <summary>
    /// Class representing project object. 
    /// </summary>
    public class Project
    {
        private static readonly log4net.ILog _log = log4net.LogManager.GetLogger(typeof(Project));

        #region // private variables

        private string projectName;
        private string projectDescription;
        private String projectFileName;

        private IntPtr dataLoggerObj;

        // XML 
        private XmlDocument _projectFile;
        private XmlNode _rootNode;

        public IntPtr DataLoggerObj
        {
            get { return dataLoggerObj; }
            set { dataLoggerObj = value; }
        }
        private String dataFile;

        public String DataFile
        {
            get { return dataFile; }
            set { dataFile = value; }
        }
        private IntPtr dashboardOverlayObj;

        public IntPtr DashboardOverlayObj
        {
            get { return dashboardOverlayObj; }
            set { dashboardOverlayObj = value; }
        }

        private InputVideoInfo inVideoData;

        public InputVideoInfo InVideoData
        {
            get { return inVideoData; }
            set { inVideoData = value; }
        }
        private OutputVideoInfo outVideoData;

        public OutputVideoInfo OutVideoData
        {
            get { return outVideoData; }
            set { outVideoData = value; }
        }
        private RenderConfigData renderCfg;

        public RenderConfigData RenderCfg
        {
            get { return renderCfg; }
            set { renderCfg = value; }
        }

        private ApplicationSettings appSet;

        public ApplicationSettings ApplicationConfig
        {
            get { return appSet; }
            set { appSet = value; }
        }

        private MD5 projectFileHash;

        public MD5 FileHash
        {
            get { return projectFileHash; }
            set { projectFileHash = value; }
        }

        private ProjectInfo m_projectInfo;

        #endregion

        #region // public variables
        #endregion


        #region // public methods

        public Project()
        {
        }

        public Project(string name, string notes, ApplicationSettings appSet)
        {
            projectName = name;
            projectDescription = notes;

            m_projectInfo = new ProjectInfo();
            m_projectInfo.projectName = this.projectName;
            m_projectInfo.projectDescription = this.projectDescription;
            m_projectInfo.appSet = appSet;
            m_projectInfo.projectFileHash = MD5.Create();
            m_projectInfo.projectFileHash.ComputeHash(new byte[] { 0 });
        }

        ~Project()
        {
            m_projectInfo.projectFileHash.Dispose();
        }

        public bool Load(string filename)
        {
            try
            {
                if (_projectFile == null)
                    _projectFile = new XmlDocument();

                string buf = File.ReadAllText(filename, Encoding.UTF8);
                MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(buf));
                _projectFile.Load(ms);

                // iterate through XML nodes and extract project information
                _rootNode = _projectFile.DocumentElement;
                if (_rootNode.Name == "Project")
                {
                    m_projectInfo.projectName = _rootNode.Attributes["Name"].Value;
                    if (_rootNode.HasChildNodes)
                    {
                        m_projectInfo.projectDescription = getKeyValue("Notes");
                        m_projectInfo.projectFileName = filename;
                        m_projectInfo.dashboardFileName = getAttributeValue<string>("Overlay", "File");
                        m_projectInfo.dataFile = getAttributeValue<string>("DataLog", "File");

                        m_projectInfo.inVideoData.fileName = getAttributeValue<string>("SourceVideo", "File");

                        m_projectInfo.outVideoData.fileName = getAttributeValue<string>("OutputVideo", "File");
                        m_projectInfo.outVideoData.overlayCfg.name = getAttributeValue<string>("Overlay", "Name");

                        m_projectInfo.outVideoData.overlayCfg.posX = getAttributeValue<int>("Overlay", "PosX");
                        m_projectInfo.outVideoData.overlayCfg.posY = getAttributeValue<int>("Overlay", "PosY");
                        m_projectInfo.outVideoData.overlayCfg.scale = getAttributeValue<int>("Overlay", "Scale");
                        m_projectInfo.outVideoData.overlayCfg.transp = getAttributeValue<int>("Overlay", "Opacity");

                        m_projectInfo.outVideoData.syncData.endFrame = getAttributeValue<int>("VideoSynchronization", "EndFrame");
                        m_projectInfo.outVideoData.syncData.endSample = getAttributeValue<int>("VideoSynchronization", "EndSample");
                        m_projectInfo.outVideoData.syncData.startFrame = getAttributeValue<int>("VideoSynchronization", "StartFrame");
                        m_projectInfo.outVideoData.syncData.startSample = getAttributeValue<int>("VideoSynchronization", "StartSample");

                        outVideoData = m_projectInfo.outVideoData;

                        m_projectInfo.renderCfg.cbAspectRatioIdx = getAttributeValue<int>("OutputVideo", "AspectRatioIdx");
                        m_projectInfo.renderCfg.cbOutputWidthIdx = getAttributeValue<int>("OutputVideo", "WidthIdx");
                        string encoder = getAttributeValue<string>("OutputVideo", "Encoder");

                        if (!string.IsNullOrEmpty(encoder))
                            m_projectInfo.renderCfg.encoder = (VideoEncoderType)Enum.Parse(typeof(VideoEncoderType), encoder);

                        switch (m_projectInfo.renderCfg.encoder)
                        {
                            case VideoEncoderType.VideoEncoder_RAW:
                                m_projectInfo.renderCfg.rawCompressorIndex = getAttributeValue<int>("Compressor", "Idx");
                                m_projectInfo.renderCfg.rawCompressorName = getAttributeValue<string>("Compressor", "Name");
                                break;

                            case VideoEncoderType.VideoEncoder_WMV:
                                m_projectInfo.renderCfg.wmvProfileFileName = getAttributeValue<string>("VideoProfile", "Name");
                                m_projectInfo.renderCfg.wmvProfileIndex = getAttributeValue<int>("VideoProfile", "Idx");
                                break;

                            default:
                                break;
                        }
                    }
                }

                //string txt = File.ReadAllText(filename);
                m_projectInfo.projectFileHash.ComputeHash(Encoding.UTF8.GetBytes(_projectFile.OuterXml));

            }
            catch (XmlException e)
            {
                _log.Error("error during project file parsing", e);
                return false;
            }
            catch (Exception e)
            {
                _log.Error("exception caught", e);
                return false;
            }
            return true;
        }

        public void LoadFromFile(string filename)
        {
            string buf;
            if (string.IsNullOrEmpty(filename))
                throw new Exception("No file name specified!");

            FileStream fs;
            List<string> lines = new List<string>();

            try
            {
                fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                StreamReader sr;
                sr = new StreamReader(fs);

                buf = sr.ReadToEnd();
                sr.BaseStream.Position = 0;
                // calculate initial MD5 hash
                if (m_projectInfo.projectFileHash == null)
                    m_projectInfo.projectFileHash = MD5.Create();

                m_projectInfo.projectFileHash.ComputeHash(Encoding.UTF8.GetBytes(buf));
                sr.BaseStream.Position = 0;

                while (!sr.EndOfStream)
                {
                    string ln = sr.ReadLine();
                    if (ln.StartsWith("#") || ln == "")
                    {
                        continue;
                    }
                    lines.Add(ln);
                }

                ProjectFileName = filename;

                //parse each line
                foreach (string ln in lines)
                {
                    if (ln.StartsWith("["))
                        continue;

                    string[] toks = ln.Split(new char[] { '=' });
                    if (toks[0] == "ProjectName")
                        ProjectName = toks[1];

                    // data file
                    if (toks[0] == "DataFile")
                        m_projectInfo.dataFile = toks[1];

                    // input video info
                    if (toks[0] == "SourceFileName")
                        m_projectInfo.inVideoData.fileName = toks[1];

                    // overlay info
                    if (toks[0] == "Name")
                        m_projectInfo.outVideoData.overlayCfg.name = toks[1];
                    if (toks[0] == "PosX")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.overlayCfg.posX);
                    if (toks[0] == "PosY")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.overlayCfg.posY);
                    if (toks[0] == "Scale")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.overlayCfg.scale);
                    if (toks[0] == "Opacity")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.overlayCfg.transp);

                    // output video file info
                    if (toks[0] == "FileName")
                        m_projectInfo.outVideoData.fileName = toks[1];

                    // render settings
                    if (toks[0] == "Encoder")
                    {
                        if (toks[1] == VideoEncoderType.VideoEncoder_RAW.ToString())
                            m_projectInfo.renderCfg.encoder = VideoEncoderType.VideoEncoder_RAW;
                        else if (toks[1] == VideoEncoderType.VideoEncoder_WMV.ToString())
                            m_projectInfo.renderCfg.encoder = VideoEncoderType.VideoEncoder_WMV;
                        else
                            m_projectInfo.renderCfg.encoder = VideoEncoderType.VideoEncoder_none;
                    }
                    if (toks[0] == "Profile")
                        m_projectInfo.renderCfg.wmvProfileFileName = toks[1];
                    if (toks[0] == "ProfileIdx")
                        int.TryParse(toks[1], out m_projectInfo.renderCfg.wmvProfileIndex);
                    if (toks[0] == "Compressor")
                        m_projectInfo.renderCfg.rawCompressorName = toks[1];
                    if (toks[0] == "CompressorIdx")
                        int.TryParse(toks[1], out m_projectInfo.renderCfg.rawCompressorIndex);
                    if (toks[0] == "WIdx")
                        int.TryParse(toks[1], out m_projectInfo.renderCfg.cbOutputWidthIdx);
                    if (toks[0] == "ARIdx")
                        int.TryParse(toks[1], out m_projectInfo.renderCfg.cbAspectRatioIdx);

                    // video sync settings
                    //if (toks[0] == "DataSampleIncrement")
                    //    int.TryParse(toks[1], out m_projectInfo.outVideoData.syncData.dataSampleInc);
                    if (toks[0] == "StartFrame")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.syncData.startFrame);
                    if (toks[0] == "EndFrame")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.syncData.endFrame);
                    if (toks[0] == "StartSample")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.syncData.startSample);
                    if (toks[0] == "EndSample")
                        int.TryParse(toks[1], out m_projectInfo.outVideoData.syncData.endSample);
                }

                for (int i = 0; i < lines.Count; i++)
                {
                    if ((lines[i] == "ProjectNotes=") && (!string.IsNullOrEmpty(lines[i + 1])))
                    {
                        for (int j = i + 1; !lines[j].StartsWith("["); j++)
                        {
                            if (lines[j + 1].StartsWith("["))
                            {
                                projectDescription += lines[j];
                            }
                            else
                            {
                                projectDescription += lines[j] + "\r\n";
                            }
                        }
                    }
                }

                sr.Close();
                fs.Close();
                sr.Dispose();
                fs.Dispose();
                sr = null;
                fs = null;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public string setFileData()
        {
            string str =
            "# !!! DO NOT EDIT THIS FILE BY HAND !!!\n" +
            "# VideoDashboard project file (v1.0)\n" +
            "# file: {0}\n" +
            "[Project]\n" +
            "ProjectName={1}\n" +
            "ProjectNotes=\n" +
            "{2}\n" +
            "\n" +
            "[ApplicationSettings]\n" +
            "ApplicationPath={3}\n" +
            "DashboardPath={4}\n" +
            "ImagePath={5}\n" +
            "ProjectPath={6}\n" +
            "VideoProfilesPath={7}" +
            "\n" +
            "[SourceVideo]\n" +
            "SourceFileName={8}\n" +
            "\n" +
            "[DataLogger]\n" +
            "DataFile={9}\n" +
            "\n" +
            "[Overlay]\n" +
            "Name={10}\n" +
            "PosX={11}\n" +
            "PosY={12}\n" +
            "Scale={13}\n" +
            "Opacity={14}\n" +
            "\n" +
            "[OutputVideo]\n" +
            "FileName={15}\n" +
            "Encoder={16}\n" +
            "Profile={17}\n" +
            "ProfileIdx={18}\n" +
            "Compressor={19}\n" +
            "CompressorIdx={20}\n" +
            "WIdx={21}\n" +
            "ARIdx={22}\n" +
            "\n" +
            "[VideoSynchronization]\n" +
            "StartFrame={23}\n" +
            "EndFrame={24}\n" +
            "StartSample={25}\n" +
            "EndSample={26}\n" +
            "\n";

            //string data = string.Format(str, new object[] { m_projectInfo.projectFileName,
            //                                          projectName, 
            //                                          projectDescription,
            //                                          m_projectInfo.appSet.appPath,
            //                                          m_projectInfo.appSet.dashPath,
            //    m_projectInfo.appSet.imgPath,m_projectInfo.appSet.projectPath,
            //    m_projectInfo.appSet.videoProfilesPath,
            //    m_projectInfo.inVideoData.fileName,
            //    m_projectInfo.dataFile,
            //    m_projectInfo.outVideoData.overlayCfg.name,
            //    m_projectInfo.outVideoData.overlayCfg.posX,
            //    m_projectInfo.outVideoData.overlayCfg.posY,
            //    m_projectInfo.outVideoData.overlayCfg.scale,
            //    m_projectInfo.outVideoData.overlayCfg.transp,
            //    m_projectInfo.outVideoData.fileName,
            //    m_projectInfo.renderCfg.encoder,
            //    m_projectInfo.renderCfg.wmvProfileFileName,
            //    m_projectInfo.renderCfg.wmvProfileIndex,
            //    m_projectInfo.renderCfg.rawCompressorName,
            //    m_projectInfo.renderCfg.rawCompressorIndex,
            //    m_projectInfo.renderCfg.cbOutputWidthIdx,
            //    m_projectInfo.renderCfg.cbAspectRatioIdx,
            //    //m_projectInfo.outVideoData.syncData.dataSampleInc,
            //    m_projectInfo.outVideoData.syncData.startFrame,
            //    m_projectInfo.outVideoData.syncData.endFrame,
            //    m_projectInfo.outVideoData.syncData.startSample,
            //    m_projectInfo.outVideoData.syncData.endSample});

            //return data;
            return null;
        }

        public void Save(bool writeToFile)
        {
            try
            {
                if (File.Exists(m_projectInfo.projectFileName) && _projectFile == null)
                {
                    // file exists but project info has not been loaded previously from any other project
                    // so we overwrite an existing project file with a new project
                    if (writeToFile)
                        File.Delete(m_projectInfo.projectFileName);

                    // just to be sure
                    _rootNode = null;
                }
                else if (File.Exists(m_projectInfo.projectFileName) && _projectFile != null)
                {
                    // file exists and project info has been loaed from another project
                    // so reset the XML instance to create a new XML document
                    _projectFile = null;
                    _rootNode = null;
                }
                // when this instance is null the project was not loaded from file before
                if (_projectFile == null)
                    _projectFile = new XmlDocument();
                if (_rootNode == null) // root node does not exist; so create the whole XML structure
                {
                    _projectFile.AppendChild(_projectFile.CreateXmlDeclaration("1.0", "utf-8", null));
                    _projectFile.AppendChild(_projectFile.CreateComment("VideoDashboard project file - do NOT edit manually"));

                    // root node
                    // <Project>
                    _rootNode = _projectFile.CreateNode(XmlNodeType.Element, "Project", "");
                    _projectFile.AppendChild(_rootNode);
                    XmlAttribute name = _projectFile.CreateAttribute("Name");
                    name.Value = m_projectInfo.projectName;
                    _rootNode.Attributes.Append(name);

                    // notes node
                    // <Notes>
                    XmlElement notes = _projectFile.CreateElement("Notes");
                    notes.AppendChild(_projectFile.CreateTextNode(m_projectInfo.projectDescription));
                    _rootNode.AppendChild(notes);

                    // SourceVideo node
                    // <SourceVideo>
                    XmlElement srcVideo = _projectFile.CreateElement("SourceVideo");
                    _rootNode.AppendChild(srcVideo);
                    XmlAttribute a = _projectFile.CreateAttribute("File");
                    a.Value = m_projectInfo.inVideoData.fileName;
                    srcVideo.Attributes.Append(a);

                    // data log node
                    // <DataLog>
                    XmlElement dataLog = _projectFile.CreateElement("DataLog");
                    _rootNode.AppendChild(dataLog);
                    a = _projectFile.CreateAttribute("File");
                    a.Value = m_projectInfo.dataFile;
                    dataLog.Attributes.Append(a);

                    // Overlay node
                    // <Overlay>
                    Dictionary<string, XmlAttribute> map = new Dictionary<string, XmlAttribute>();
                    map.Add("Name", _projectFile.CreateAttribute("Name"));
                    map.Add("File", _projectFile.CreateAttribute("File"));
                    map.Add("PosX", _projectFile.CreateAttribute("PosX"));
                    map.Add("PosY", _projectFile.CreateAttribute("PosY"));
                    map.Add("Scale", _projectFile.CreateAttribute("Scale"));
                    map.Add("Opacity", _projectFile.CreateAttribute("Opacity"));

                    map["Name"].Value = m_projectInfo.outVideoData.overlayCfg.name;
                    map["File"].Value = m_projectInfo.dashboardFileName;
                    map["PosX"].Value = m_projectInfo.outVideoData.overlayCfg.posX.ToString();
                    map["PosY"].Value = m_projectInfo.outVideoData.overlayCfg.posY.ToString();
                    map["Scale"].Value = m_projectInfo.outVideoData.overlayCfg.scale.ToString();
                    map["Opacity"].Value = m_projectInfo.outVideoData.overlayCfg.transp.ToString();

                    XmlElement dash = _projectFile.CreateElement("Overlay");
                    _rootNode.AppendChild(dash);
                    for (int i = 0; i < map.Count; i++)
                    {
                        dash.Attributes.Append(map.ElementAt(i).Value);
                    }

                    // OutputVideo node
                    // <OutputVideo> -> contains 2 childs
                    XmlElement outVid = _projectFile.CreateElement("OutputVideo");
                    _rootNode.AppendChild(outVid);

                    map.Clear();
                    map.Add("File", _projectFile.CreateAttribute("File"));
                    map.Add("Encoder", _projectFile.CreateAttribute("Encoder"));
                    map.Add("WidthIdx", _projectFile.CreateAttribute("WidthIdx"));
                    map.Add("AspectRatioIdx", _projectFile.CreateAttribute("AspectRatioIdx"));

                    map["File"].Value = m_projectInfo.outVideoData.fileName;
                    map["Encoder"].Value = m_projectInfo.renderCfg.encoder.ToString();
                    map["WidthIdx"].Value = m_projectInfo.renderCfg.cbOutputWidthIdx.ToString();
                    map["AspectRatioIdx"].Value = m_projectInfo.renderCfg.cbAspectRatioIdx.ToString();

                    for (int i = 0; i < map.Count; i++)
                        outVid.Attributes.Append(map.ElementAt(i).Value);

                    // child nodes
                    //    <VideoProfile>
                    XmlElement vidProfile = _projectFile.CreateElement("VideoProfile");
                    outVid.AppendChild(vidProfile);

                    a = _projectFile.CreateAttribute("Name");
                    a.Value = m_projectInfo.renderCfg.wmvProfileFileName;
                    vidProfile.Attributes.Append(a);

                    a = _projectFile.CreateAttribute("Idx");
                    a.Value = m_projectInfo.renderCfg.wmvProfileIndex.ToString();
                    vidProfile.Attributes.Append(a);

                    //    <Compressor>
                    XmlElement comp = _projectFile.CreateElement("Compressor");
                    outVid.AppendChild(comp);

                    a = _projectFile.CreateAttribute("Name");
                    a.Value = m_projectInfo.renderCfg.rawCompressorName;
                    comp.Attributes.Append(a);

                    a = _projectFile.CreateAttribute("Idx");
                    a.Value = m_projectInfo.renderCfg.rawCompressorIndex.ToString();
                    comp.Attributes.Append(a);

                    /////////////////////////////////////////////////////////////////////////////////////////////////////

                    // video sync node
                    // <VideoSynchronization>
                    XmlElement vidSync = _projectFile.CreateElement("VideoSynchronization");
                    _rootNode.AppendChild(vidSync);

                    map.Clear();
                    map.Add("StartFrame", _projectFile.CreateAttribute("StartFrame"));
                    map.Add("EndFrame", _projectFile.CreateAttribute("EndFrame"));
                    map.Add("StartSample", _projectFile.CreateAttribute("StartSample"));
                    map.Add("EndSample", _projectFile.CreateAttribute("EndSample"));

                    map["StartFrame"].Value = m_projectInfo.outVideoData.syncData.startFrame.ToString();
                    map["EndFrame"].Value = m_projectInfo.outVideoData.syncData.endFrame.ToString();
                    map["StartSample"].Value = m_projectInfo.outVideoData.syncData.startSample.ToString();
                    map["EndSample"].Value = m_projectInfo.outVideoData.syncData.endSample.ToString();

                    for (int i = 0; i < map.Count; i++)
                        vidSync.Attributes.Append(map.ElementAt(i).Value);

                }
                else // root node exists; so update XML structure
                {
                    setAttributeValue("Project", "Name", m_projectInfo.projectName);
                    setKeyValue("Notes", m_projectInfo.projectDescription);
                    setAttributeValue("Overlay", "File", m_projectInfo.dashboardFileName);
                    setAttributeValue("DataLog", "File", m_projectInfo.dataFile);

                    setAttributeValue("SourceVideo", "File", m_projectInfo.inVideoData.fileName);

                    setAttributeValue("OutputVideo", "File", m_projectInfo.outVideoData.fileName);
                    setAttributeValue("Overlay", "Name", m_projectInfo.outVideoData.overlayCfg.name);

                    setAttributeValue("Overlay", "PosX", m_projectInfo.outVideoData.overlayCfg.posX.ToString());
                    setAttributeValue("Overlay", "PosY", m_projectInfo.outVideoData.overlayCfg.posY.ToString());
                    setAttributeValue("Overlay", "Scale", m_projectInfo.outVideoData.overlayCfg.scale.ToString());
                    setAttributeValue("Overlay", "Opacity", m_projectInfo.outVideoData.overlayCfg.transp.ToString());

                    setAttributeValue("VideoSynchronization", "EndFrame", m_projectInfo.outVideoData.syncData.endFrame.ToString());
                    setAttributeValue("VideoSynchronization", "EndSample", m_projectInfo.outVideoData.syncData.endSample.ToString());
                    setAttributeValue("VideoSynchronization", "StartFrame", m_projectInfo.outVideoData.syncData.startFrame.ToString());
                    setAttributeValue("VideoSynchronization", "StartSample", m_projectInfo.outVideoData.syncData.startSample.ToString());

                    setAttributeValue("OutputVideo", "AspectRatioIdx", m_projectInfo.renderCfg.cbAspectRatioIdx.ToString());
                    setAttributeValue("OutputVideo", "WidthIdx", m_projectInfo.renderCfg.cbOutputWidthIdx.ToString());
                    setAttributeValue("OutputVideo", "Encoder", m_projectInfo.renderCfg.encoder.ToString());
                }

                if (writeToFile)
                {
                    _projectFile.Save(m_projectInfo.projectFileName);
                    m_projectInfo.projectFileHash.ComputeHash(Encoding.UTF8.GetBytes(_projectFile.OuterXml));
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public bool compareFileHash()
        {
            byte[] hashOrig, hashNew;

            Save(false);

            MD5 md5 = new MD5CryptoServiceProvider();

            hashOrig = m_projectInfo.projectFileHash.Hash;
            hashNew = md5.ComputeHash(Encoding.UTF8.GetBytes(_projectFile.OuterXml));
            int i = 0;

            while ((i < hashNew.Length) && (hashNew[i] == hashOrig[i]))
            {
                i += 1;
            }
            if (i == hashNew.Length)
                return true;

            return false;
        }

        #endregion

        #region // public properties

        public String ProjectName
        {
            get
            {
                return projectName;
            }
            set
            {
                projectName = value;
            }
        }

        public String ProjectFileName
        {
            get
            {
                return m_projectInfo.projectFileName;   //m_fileName;
            }
            set
            {
                projectFileName = value;
                m_projectInfo.projectFileName = value;  // m_fileName = value;
            }
        }

        public String ProjectDescription
        {
            get
            {
                return projectDescription;
            }
            set
            {
                projectDescription = value;
            }
        }

        public ProjectInfo curProjectInfo
        {
            get
            {
                return this.m_projectInfo;
            }
            set
            {
                m_projectInfo = value;
            }
        }
        /**************************************************************************************************/
        #endregion

        #region // private methods

        private void setAttributeValue(string key, string attrib, string value)
        {
            if (_rootNode != null)
            {
                foreach (XmlNode n in _rootNode.ChildNodes)
                {
                    if (n.HasChildNodes)
                    {
                        foreach (XmlNode c in n.ChildNodes)
                        {
                            if (c.Name == key)
                            {
                                if (c.Attributes.Count > 0)
                                {
                                    foreach (XmlAttribute a in c.Attributes)
                                    {
                                        if (a.Name == attrib)
                                        {
                                            a.Value = value;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (n.Name == key)
                    {
                        if (n.Attributes.Count > 0)
                        {
                            foreach (XmlAttribute a in n.Attributes)
                            {
                                if (a.Name == attrib)
                                {
                                    a.Value = value;
                                }
                            }
                        }
                    }
                }
            }
        }

        private void setKeyValue(string key, string value)
        {
            foreach (XmlNode n in _rootNode.ChildNodes)
            {
                if (n.Name == key)
                    n.InnerText = value;
            }
        }

        private T getAttributeValue<T>(string key, string attrib)
        {
            foreach (XmlNode n in _rootNode.ChildNodes)
            {
                if (n.HasChildNodes)
                {
                    foreach (XmlNode c in n.ChildNodes)
                    {
                        if (c.Name == key)
                        {
                            if (c.Attributes.Count > 0)
                            {
                                foreach (XmlAttribute a in c.Attributes)
                                {
                                    if (a.Name == attrib)
                                    {
                                        if (typeof(T) == typeof(string))
                                            return (T)(object)a.Value;
                                        else if (typeof(T) == typeof(int))
                                        {
                                            int x = 0;
                                            bool b = int.TryParse(a.Value, out x);
                                            if (b)
                                                return (T)(object)x;
                                            else
                                                return (T)Convert.ChangeType(0, typeof(T));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (n.Name == key)
                {
                    if (n.Attributes.Count > 0)
                    {
                        foreach (XmlAttribute a in n.Attributes)
                        {
                            if (a.Name == attrib)
                            {
                                if (typeof(T) == typeof(string))
                                    return (T)(object)a.Value;
                                else if (typeof(T) == typeof(int))
                                {
                                    int x = 0;
                                    bool b = int.TryParse(a.Value, out x);
                                    if (b)
                                        return (T)(object)x;
                                    else
                                        return (T)Convert.ChangeType(0, typeof(T));
                                }
                            }
                        }
                    }
                }
            }

            return default(T);
        }

        private string getKeyValue(string key)
        {
            foreach (XmlNode n in _rootNode.ChildNodes)
            {
                if (n.Name == key)
                    return n.InnerText;
            }

            return null;
        }


        #endregion

    }
}
