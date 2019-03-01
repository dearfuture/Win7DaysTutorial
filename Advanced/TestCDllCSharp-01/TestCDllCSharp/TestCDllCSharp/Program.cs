using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace TestCDllCSharp
{
    class Program
    {
        //[DllImport("C:/Users/ycx/Desktop/TestCDllCSharp/TestCDllCSharp/HelloWorld.dll", CharSet = CharSet.Ansi)]
        [DllImport("HelloWorld.dll", CharSet = CharSet.Ansi)]
        static extern void SayHello();
        static void Main(string[] args)
        {
            SayHello();
            Console.ReadKey();
        }
    }
}
