﻿using System;
using System.IO;

namespace Catalyst.Runtime.Utilities
{
    internal class ConsoleEx
    {
        private static StreamWriter? _outputWriter;
        private static TextWriter? _originalStream;

        public static void Redirect()
        {
            RecreateOutputStream();
            RedirectConsoleOut();
        }
        
        private static void RecreateOutputStream()
        {
            _originalStream = Console.Out;
            _outputWriter = new StreamWriter(Console.OpenStandardOutput()) {AutoFlush = true};
            
            RedirectConsoleOut();
        }

        private static void RedirectConsoleOut()
        {
            if (_outputWriter != null)
            {
                Console.SetOut(_outputWriter);
            }
        }
    }
}