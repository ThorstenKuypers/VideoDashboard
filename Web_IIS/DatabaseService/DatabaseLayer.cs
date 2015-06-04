using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;

namespace DatabaseService
{

    public class DatabaseLayer
    {
#if DEBUG
        private static readonly string _conStr = "Data Source=(LocalDB)\\v11.0;AttachDbFilename=D:\\Development\\Projects\\VideoDashboard\\Web_IIS\\Homepage\\App_Data\\VideoDashboardDB.mdf;Integrated Security=True;Connect Timeout=30";
#else
        private static readonly string _conStr = "server=db548058260.db.1and1.com;Initial Catalog=db548058260; User ID=dbo548058260;Password=88Master88_SQL";
#endif

        private static SqlConnection _sqlConnection;

        public DatabaseLayer()
        {
        }

        public static DataTable GetTable(string tableName)
        {
            DataSet ds = null;
            DataTable dt = null;

            try
            {
                _sqlConnection = new SqlConnection(_conStr);
                SqlDataAdapter da = new SqlDataAdapter(string.Format("SELECT * FROM {0}", tableName), _sqlConnection);

                ds = new DataSet(tableName);
                da.FillSchema(ds, SchemaType.Source);

                da.Fill(ds, tableName);
                dt = ds.Tables[tableName];

                ds.Dispose();
                ds = null;
                da.Dispose();
                da = null;

            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                if (_sqlConnection != null)
                {
                    _sqlConnection.Dispose();
                    _sqlConnection = null;
                }
            }
            return dt;
        }


        public static void UpdateTable(string tableName, DataTable tbl)
        {
            SqlDataAdapter da =null;
            SqlCommandBuilder cb =null;

            try
            {
                _sqlConnection = new SqlConnection(_conStr);
                da = new SqlDataAdapter(string.Format("SELECT * FROM {0}", tableName), _sqlConnection);
                cb = new SqlCommandBuilder(da);

                da.UpdateCommand = cb.GetUpdateCommand();
                da.InsertCommand = cb.GetInsertCommand();
                da.DeleteCommand = cb.GetDeleteCommand();

                da.Update(tbl);

                da.Dispose();
            }
            catch(Exception ex)
            {
                throw ex;
            }
            finally
            {
                if (cb != null)
                {
                    cb.Dispose();
                    cb = null;
                }

                if (da!=null)
                {
                    da.Dispose();
                    da = null;
                }
                if (_sqlConnection!=null)
                {
                    _sqlConnection.Dispose();
                    _sqlConnection = null;
                }
            }
        }

        public static void AddBetaTester(BetaTester t)
        {
            //SqlDataAdapter da = null;
            //SqlCommandBuilder cb = null;
            DataTable dt = null;

            if (t != null)
            {
                try
                {
                    dt = GetTable("BetaTesters");
                    if (dt != null)
                    {
                        dt.Columns[0].AutoIncrementStep = -1;
                        dt.Columns[0].AutoIncrementSeed = -1;

                        DataRow r = dt.NewRow();

                        r.BeginEdit();
                        r["name"] = t.Name;
                        r["email"] = t.Email;
                        r["code"] = t.Code;
                        r["ip"] = t.IP;
                        r["registerDate"] = t.RegistrationDate.ToBinary();
                        r.EndEdit();

                        dt.Rows.Add(r);

                        UpdateTable("BetaTesters", dt);
                    }
                    //_sqlConnection = new SqlConnection(_conStr);
                    //da = new SqlDataAdapter("SELECT * FROM BetaTesters", _sqlConnection);
                    //cb = new SqlCommandBuilder(da);

                    //da.UpdateCommand = cb.GetUpdateCommand();
                    //da.InsertCommand = cb.GetInsertCommand();

                    //DataSet ds = new DataSet("BetaTesters");
                    //da.Fill(ds);

                    //DataTable dt =ds.Tables["BetaTesters"];
                    //ds.Tables["BetaTesters"].Columns[0].AutoIncrementSeed = -1;
                    //ds.Tables["BetaTesters"].Columns[0].AutoIncrementStep = -1;

                    //DataRow r = dt.NewRow();

                    //r.BeginEdit();
                    //r["name"] = t.Name;
                    //r["email"] = t.Email;
                    //r["code"] = t.Code;
                    //r["ip"] = t.IP;
                    //r["registerDate"] = t.RegistrationDate.ToBinary();
                    //r.EndEdit();

                    //dt.Rows.Add(r);

                    //da.Update(dt);
                }
                catch(Exception ex)
                {
                    throw ex;
                }
                //finally
                //{
                //    if (cb != null)
                //    {
                //        cb.Dispose();
                //        cb = null;
                //    }
                //    if (da != null)
                //    {
                //        da.Dispose();
                //        da = null;
                //    }
                //    if (_sqlConnection!=null)
                //    {
                //        _sqlConnection.Dispose();
                //        _sqlConnection = null;
                //    }
                //}
            }
        }
    }
}