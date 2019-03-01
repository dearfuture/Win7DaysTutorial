/*
 * 编码：袁春旭
 * 讲解：袁春旭
 * 内容：调用带参数的导出函数
 */
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

        [DllImport("Calculater.dll", CharSet = CharSet.Ansi)]
        static extern int add1(int a, int b);

        [DllImport("Calculater.dll", CharSet = CharSet.Ansi)]
        static extern double add2(double a, double b);

        [DllImport("Calculater.dll", CharSet = CharSet.Ansi)]
        static extern int add3(StringBuilder str1, int size, StringBuilder str2);

        static void Main(string[] args)
        {
            SayHello();

            Console.WriteLine(add1(3, 5));
            Console.WriteLine(add2(3.3, 5.2));
            StringBuilder str1 = new StringBuilder("hello ", 255);
            StringBuilder str2 = new StringBuilder("world");

            Console.WriteLine(add3(str1, 255, str2));
            Console.WriteLine(str1);

            Console.ReadKey();
        }
    }
}
