﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool
//     Changes to this file will be lost if the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.ServiceModel;
using System.Data.SqlClient;
using System.Configuration;


namespace DatabaseService
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
    public class DatabaseService : IDatabaseService
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(DatabaseService).Name);


        private bool _isAuthenticated;

        public bool Login(string password)
        {
            if (_isAuthenticated)
                return true;

            if (password == "$$secret_passwd$$")
            {
                _isAuthenticated = true;
                return true;
            }

            return false;
        }

        public virtual DataTable GetBetaTestersTable()
        {
            DataTable dt = null;

            if (!_isAuthenticated)
            {
                throw new FaultException<DatabaseServiceException>(new DatabaseServiceException("Unauthorized access"), new FaultReason("User not authenticated"));
            }

            try
            {
                dt = DatabaseLayer.GetTable("BetaTesters");
            }
            catch(Exception ex)
            {
                log.Error("", ex);
                return null;
            }

            return dt;
        }

        public virtual void AddNewBetaTester(BetaTester tester)
        {
            throw new System.NotImplementedException();
        }

        public virtual void DeleteBetaTester(BetaTester bt)
        {
            throw new System.NotImplementedException();
        }

        public virtual void AddNewCustomer(Customer cs)
        {
            throw new System.NotImplementedException();
        }

        public virtual void DeleteCustomer(Customer cs)
        {
            throw new System.NotImplementedException();
        }

        public virtual DataTable GetCustomersTable()
        {
            throw new System.NotImplementedException();
        }

        public virtual void UpdateCustomersTable(DataTable table)
        {
            throw new System.NotImplementedException();
        }

        public virtual void UpdateBetaTestersTable(DataTable table)
        {
            if (!_isAuthenticated)
            {
                throw new FaultException<DatabaseServiceException>(new DatabaseServiceException("Unauthorized access"), new FaultReason("User not authenticated"));
            }

            try
            {
                if (table != null)
                {
                    DatabaseLayer.UpdateTable("BetaTesters", table);
                }
            }
            catch(Exception ex)
            {
                log.Error("", ex);
                return;
            }
        }

        public virtual bool CheckTesterExists(string name, string mail, string ip)
        {
            throw new NotImplementedException();
        }

    }

}