using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;

namespace Catalyst.Runtime.Systems
{
    internal sealed class LogManager
    {
        private readonly string _logDirectory;
        private readonly string _logFilePath;
        private readonly StreamWriter _logWriter;

        private readonly Queue<string> _writeQueue;
        private readonly Thread _writerThread;

        public LogManager(string logDirectory)
        {
            _logDirectory = logDirectory;
            _logFilePath = CreateLogFilePath();
            _logWriter = new StreamWriter(_logFilePath) { AutoFlush = true };

            _writeQueue = new Queue<string>();

            _writerThread = new Thread(WriterBackgroundTask);
            _writerThread.Start();
        }

        public Log CreateLog(ModId modId)
            => new(this, modId);

        public void EnqueueWriteOperation(string text)
        {
            lock (_writeQueue)
            {
                _writeQueue.Enqueue(text);
            }
        }

        private string CreateLogFilePath()
            => Path.Combine(_logDirectory, $"{DateTime.Now:dd-MM-yyyy_HH_mm_ss}.log");

        private void WriterBackgroundTask()
        {
            while (true)
            {
                lock (_writeQueue)
                {
                    while (_writeQueue.Count > 0)
                    {
                        var msg = _writeQueue.Dequeue();
                        
                        lock (_logWriter)
                        {
                            _logWriter.WriteLine();
                        }
                        
                        Console.WriteLine(msg);
                    }
                }

                Thread.Sleep(10);
            }
        }
    }
}