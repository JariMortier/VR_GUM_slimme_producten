using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;

namespace GameEmulation
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private SerialPort _serialPort;
        public MainWindow()
        {
            InitializeComponent();
            InitializeSerialPort();
            endGame();
        }

        private void GameButton_Click(object sender, RoutedEventArgs e)
        {
            endGame();
            _serialPort.Write("e");
        }
        private void InitializeSerialPort()
        {
            _serialPort = new SerialPort
            {
                PortName = "COM8", // Adjust as necessary
                BaudRate = 9600,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One,
                Handshake = Handshake.None
            };

            _serialPort.DataReceived += SerialPort_DataReceived;

            try
            {
                _serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error opening serial port: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = _serialPort.ReadExisting();

                // Update the UI (Dispatcher is needed to ensure thread safety)
                Dispatcher.Invoke(() =>
                {
                    // Assuming there's a TextBox named "ReceivedDataTextBox" in your XAML
                    ReceivedDataTextBox.AppendText(data);
                });

                if(data.Trim() == "s")
                {
                    Dispatcher.Invoke(() =>
                    {
                        // Assuming there's a TextBox named "ReceivedDataTextBox" in your XAML
                        startGame();
                    });
                }
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    MessageBox.Show($"Error reading serial data: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                });
            }
        }

        private void startGame()
        {
            System.Windows.Media.Color LimeGreen = System.Windows.Media.Color.FromRgb(50, 205, 50);
            GameStatus.Fill = new SolidColorBrush(LimeGreen);
            GameButton.IsEnabled = true;
        }

        private void endGame()
        {
            System.Windows.Media.Color Red = System.Windows.Media.Color.FromRgb(255, 0, 0);
            GameStatus.Fill = new SolidColorBrush(Red);
            GameButton.IsEnabled = false;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            _serialPort.Write(" ");
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
            }
        }

        private void SerialButton_Click(object sender, RoutedEventArgs e)
        {
            _serialPort.Write(ReadText(SerialBox));
        }

        private string ReadText(TextBox box)
        {
            string text = box.Text;
            box.Text = "";
            return text;
        }

        private void RedButton_Click(object sender, RoutedEventArgs e)
        {
            _serialPort.Write("red");
        }

        private void GreenButton_Click(object sender, RoutedEventArgs e)
        {
            _serialPort.Write("green");
        }

        private void BlueButton_Click(object sender, RoutedEventArgs e)
        {
            _serialPort.Write("blue");
        }

        private void RaveButton_Click(object sender, RoutedEventArgs e)
        {
            _serialPort.Write("rave");
        }
    }
}