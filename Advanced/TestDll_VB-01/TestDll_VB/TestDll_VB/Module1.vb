Module Module1
    Declare Sub SayHello2 Lib "HelloWorld.dll" Alias "SayHello" ()

    Sub Main()
        SayHello2()
        Console.ReadKey()
    End Sub

End Module
