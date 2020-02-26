using System;
using System.IO;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace Image_Converter
{
    public class Image_Process
    {
        public const string UnamanagedAssemblyName = "libpub-sub-cpp.so";

        public delegate void ScreenShotCallBack(int status, int imageSize, IntPtr imageData);

        [DllImport(UnamanagedAssemblyName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetSubscriber")]
        public static extern void SetSubscriber(ScreenShotCallBack callback);
        
        [DllImport(UnamanagedAssemblyName, CallingConvention = CallingConvention.Cdecl, 
        EntryPoint = "read_test_image")]
        public static extern void check();

        private readonly ScreenShotCallBack _SubscriptionDelegate;

        public Image_Process()
        {
            
            _SubscriptionDelegate = UnmanagedCallBack;
        }

        public void call()
        {
            System.Console.WriteLine("Reached");
            //read_test_image();
            //check();
            SetSubscriber(_SubscriptionDelegate);
        }

        private void UnmanagedCallBack(int statusCode, int dataLength, IntPtr data)
        {
            try
            {
                System.Console.WriteLine("statusCode: " + statusCode + " dataLength: "+dataLength);
                var ImageData = new byte[dataLength];
                Marshal.Copy(data, ImageData, 0, dataLength);
		System.Console.WriteLine("\nWrite OUT.jpg\n");
                //Marshal.FreeHGlobal(data);
                MemoryStream ms = new MemoryStream(ImageData);
		System.Console.WriteLine("\nWrite OUT.jpg\n");
                Image returnImage = Image.FromStream(ms);
		System.Console.WriteLine("\nWrite OUT.jpg\n");
                returnImage.Save("OUT.jpg",ImageFormat.Jpeg);
		System.Console.WriteLine("\nWrite OUT.jpg\n");
                //Bitmap bmp = new Bitmap(1920, 1080);
                // for (int y = 0; y < 1080; ++y)
                //     for (int x = 0; x < 1920; ++x)
                //     {
                //     int offset = y * 5 * 4 + x * 4;
                //     bmp.SetPixel(x, y, Color.FromArgb(ImageData[offset], ImageData[offset + 1], ImageData[offset + 2], ImageData[offset + 3]));
                //     }
                // bmp.Save("tmp.png",ImageFormat.Png);
                // using(Image image = Image.FromStream(new MemoryStream(ImageData)))
                // {
                //     image.Save("output.jpg", ImageFormat.Jpeg);  // Or Png
                // }
                //_PublishedTask.TrySetResult(new ImageTransaction(ImageData));
            }
            catch (Exception Error)
            {
                Console.WriteLine(Error);
		System.Console.WriteLine("\nWrite OUT.jpg\n");

            }
        }

    }
}
