Module Module1
    Declare Sub SayHello2 Lib "HelloWorld.dll" Alias "_SayHello@0" ()
    Declare Function add1 Lib "Calculater.dll" Alias "_add1@8" (ByVal a As Integer, ByVal a As Integer) As Integer
    Declare Function add2 Lib "Calculater.dll" Alias "_add2@16" (ByVal a As Double, ByVal a As Double) As Double
    Declare Function add3 Lib "Calculater.dll" Alias "_add3@12" (ByVal str1() As Byte, ByVal size As Integer, ByVal str2() As Byte) As Integer
    Sub Main()
        SayHello2()
        Console.WriteLine(add1(3, 5))
        Console.WriteLine(add2(3.2, 5.8))
        Dim str1(255) As Byte
        Dim str2(100) As Byte
        str1(0) = 65
        str2(0) = 66
        Console.WriteLine(add3(str1, 255, str2))
        Console.WriteLine(str1(0))
        Console.WriteLine(str1(1))
        Console.ReadKey()
    End Sub
End Module
