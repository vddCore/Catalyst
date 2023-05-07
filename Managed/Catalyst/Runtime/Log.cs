﻿using System;
using System.Text;
using Catalyst.API;
using Catalyst.Runtime.Systems;
using UnityEngine;

namespace Catalyst.Runtime
{
    internal class Log : ILog
    {
        private readonly LogManager _logManager;
        private readonly ModId _modId;

        internal Log(LogManager logManager, ModId modId)
        {
            _logManager = logManager;
            _modId = modId;
        }

        public void Info(string msg)
            => _logManager.EnqueueWriteOperation(FormatMessage(msg, "INF"));

        public void Warning(string msg)
            => _logManager.EnqueueWriteOperation(FormatMessage(msg, "WRN"));

        public void Error(string msg)
            => _logManager.EnqueueWriteOperation(FormatMessage(msg, "ERR"));

        public void Exception(Exception e)
        {
            var sb = new StringBuilder();
            sb.AppendLine(FormatMessage("An exception has occurred.", "EXC"));

            var lines = e.ToString().Split('\n');
            for (var i = 0; i < lines.Length; i++)
            {
                sb.Append("  ");

                if (i < lines.Length - 1)
                {
                    sb.AppendLine(lines[i]);
                }
                else
                {
                    sb.Append(lines[i]);
                }
            }

            _logManager.EnqueueWriteOperation(sb.ToString());
        }
        
        internal void InstallUnityLogHook()
        {
            Application.logMessageReceivedThreaded += OnUnityLogMessageReceived;
        }

        private void ExceptionWithStackTrace(string message, string stackTrace)
        {
            var sb = new StringBuilder();
            sb.AppendLine(FormatMessage("An exception has occurred.", "EXC"));
            sb.Append("  ");
            sb.AppendLine(message);
            
            var lines = stackTrace.Split('\n');
            for (var i = 0; i < lines.Length; i++)
            {
                sb.Append("  ");

                if (i < lines.Length - 1)
                {
                    sb.AppendLine(lines[i]);
                }
                else
                {
                    sb.Append(lines[i]);
                }
            }

            _logManager.EnqueueWriteOperation(sb.ToString());
        }

        private void OnUnityLogMessageReceived(string condition, string stacktrace, LogType type)
        {
            switch (type)
            {
                case LogType.Log:
                    Info(condition);
                    break;
                
                case LogType.Warning:
                    Warning(condition);
                    break;
                
                case LogType.Exception:
                    ExceptionWithStackTrace(condition, stacktrace);
                    break;
                
                case LogType.Assert:
                case LogType.Error:
                    Error(condition);
                    break;
            }
        }

        private string FormatMessage(string msg, string logLevelTag)
        {
            var sb = new StringBuilder();

            sb.Append(logLevelTag);
            sb.Append(" :: ");
            sb.Append(_modId);
            sb.Append(" :: ");
            sb.Append(msg);

            return sb.ToString();
        }
    }
}