using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace CLIENT
{
    internal class Communicator
    { 



        private TcpClient sock = new TcpClient();
        private const int port = 9090;
        private const string ip = "127.0.0.1";
        /*the C'tor: this function will create a new communicator
         * input: non
         * output: non
         */
        public Communicator()
        {
            sock.Connect(ip, port);
            if (!sock.Connected)
            {
                sock.Close();
            }
        }

        /*the D'tor: this function will close the socket
         * input: non
         * output non
         */
        public void close()
        {
            sock.Close();
        }

        /*SendData: this function will send the data of a custom message
         * input: the json and the code
         * output non
         */
        public void SendData(byte code, JObject data)
        {
            string jsonStr = JsonConvert.SerializeObject(data);
            byte[] lengthBytes = BitConverter.GetBytes(jsonStr.Length);
            byte[] dataBytes = Encoding.UTF8.GetBytes(jsonStr);
            byte[] codeBytes = { code };//getting bytes for all 3 fields
            byte[] result;
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(lengthBytes);
            }
            result = codeBytes.Concat(lengthBytes.Concat(dataBytes).ToArray()).ToArray();//adding all of the byte arrays into a one big array
            sock.GetStream().Write(result, 0, result.Length);
            
        }

        /*GetData: this function will get the json, and also insert the code from the server
         * input: non
         *  output: the response Json from the server
         */
        public JObject GetData()
        {
            JObject result;
            string jsonStr;
            byte[] code = {0x0};
            int len = 0;
            byte[] length = { 0x0, 0x0, 0x0, 0x0 };
            byte[] data;
            sock.GetStream().Read(code, 0, 1);// getting code and length
            sock.GetStream().Read(length, 0, 4);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(length);
            }
            len = BitConverter.ToInt32(length, 0);
            data = new byte[len];
            sock.GetStream().Read(data, 0, len);//getting data
            jsonStr = Encoding.UTF8.GetString(data);
            result = JObject.Parse(jsonStr);
            result["Code"] = code[0];//adding the code to the json
            return result;
        }

    }





}























