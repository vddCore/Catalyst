using System;

namespace Catalyst.API
{
    public interface ILog
    {
        void Info(string msg);
        void Warning(string msg);
        void Error(string msg);
        void Exception(Exception e);
    }
}