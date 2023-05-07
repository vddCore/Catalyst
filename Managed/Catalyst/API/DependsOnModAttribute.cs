using System;
using Catalyst.Runtime;

namespace Catalyst.API
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
    public sealed class DependsOnModAttribute : Attribute
    {
        public ModId ModId { get; }
        public bool Required { get; set; } = true;

        public DependsOnModAttribute(string modIdString) 
            => ModId = new ModId(modIdString);
    }
}