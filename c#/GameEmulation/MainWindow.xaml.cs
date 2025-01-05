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
        }

        private void GameButton_Click(object sender, RoutedEventArgs e)
        {
            Brush brush;
            System.Windows.Media.Color Red = System.Windows.Media.Color.FromRgb(255, 0, 0);
            System.Windows.Media.Color LimeGreen = System.Windows.Media.Color.FromRgb(50, 205, 50);
            if (endGame())
            {
                brush = new SolidColorBrush(Red);
                GameButton.Content = "Start game";
            } else
            {
                brush = new SolidColorBrush(LimeGreen);
                GameButton.Content = "End game";
            }
            GameStatus.Fill = brush;
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
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    MessageBox.Show($"Error reading serial data: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                });
            }
        }

        private bool endGame()
        {
            if ((string)GameButton.Content == "Start game")
            {
                return false;
            }
            _serialPort.Write("stop");
            return true;
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