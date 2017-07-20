using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;
using System.Diagnostics;

namespace WindowsFormsApp1
{
    class SqlBridge
    {
        public MySqlConnection connection;

        //config for local database info
        private string localServer = "localhost";
        private string localDBName = "xinchejian";
        private string localUid = "root";
        private string localPassword = "123456";
        private string authorizationDb = "staff_authorization";
        private string attendanceDb = "staff_attendance";


        //Constructor
        public SqlBridge()
        {
            Initialize();

        }

        //Initialize for local database connection
        private void Initialize()
        {
            string connectionString;
            connectionString = "SERVER=" + localServer + ";" + "DATABASE=" +
            localDBName + ";" + "UID=" + localUid + ";" + "PASSWORD=" + localPassword + ";Charset=utf8";

            connection = new MySqlConnection(connectionString);
        }

        //open connection to database
        public bool OpenConnection()
        {
            try
            {
                connection.Open();
                return true;
            }
            catch (MySqlException ex)
            {
                //When handling errors, you can your application's response based 
                //on the error number.
                //The two most common error numbers when connecting are as follows:
                //0: Cannot connect to server.
                //1045: Invalid user name and/or password.
                switch (ex.Number)
                {
                    case 1042:
                        Console.WriteLine("Cannot connect to server.  Contact administrator");
                        break;

                    case 1045:
                        Console.WriteLine("Invalid username/password, please try again");
                        break;
                }
                return false;
            }
        }

        //Close connection
        public bool CloseConnection()
        {
            try
            {
                connection.Close();
                return true;
            }
            catch (MySqlException ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }

        //Insert statement
        public bool Insert(string name, string id, int code)
        {
            string query = "INSERT INTO " + authorizationDb + "(staff_name,staff_id,NoEntry) VALUES(" + "\"" + name + "\", " + "\"" + id + "\",\"" + code + "\");";

            //open connection
            if (this.OpenConnection() == true)
            {
                //create command and assign the query and connection from the constructor
                MySqlCommand cmd = new MySqlCommand(query, connection);

                //Execute command
                cmd.ExecuteNonQuery();

                //close connection
                this.CloseConnection();

                return true;
            }
            else
            {
                return false;
            }
        }


    }
}
