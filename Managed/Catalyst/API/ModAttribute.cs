using System;

namespace Catalyst.API
{
    [AttributeUsage(AttributeTargets.Class)]
    public class ModAttribute : Attribute
    {
        public string ID { get; }

        public ModAttribute(string id)
            => ID = id;
    }
}