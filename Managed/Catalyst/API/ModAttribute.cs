using System;
using Catalyst.Runtime;

namespace Catalyst.API
{
    [AttributeUsage(AttributeTargets.Class)]
    public sealed class ModAttribute : Attribute
    {
        public ModId ModID { get; }

        public ModAttribute(string modIdString)
            => ModID = new(modIdString);
    }
}