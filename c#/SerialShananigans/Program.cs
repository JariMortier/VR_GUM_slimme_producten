using System.IO.Ports;

SerialPort serialPort = new SerialPort("COM8");
serialPort.BaudRate = 9600;
serialPort.Parity = Parity.None;
serialPort.DataBits = 8;
serialPort.StopBits = StopBits.One;

serialPort.Open();

string intext = "";
while (true)
{
    Console.Write("Write to arduino: ");
    string text = Console.ReadLine();
    serialPort.Write(text);
    
    Thread.Sleep(100);

    intext = serialPort.ReadExisting();
    Console.WriteLine(intext);
}
