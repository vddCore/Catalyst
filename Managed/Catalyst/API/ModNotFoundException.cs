using System;
using Catalyst.Runtime;

namespace Catalyst.API
{
    public class ModNotFoundException : Exception
    {
        public ModId RequestedModID { get; }

        internal ModNotFoundException(ModId requestedModId, Exception innerException)
            : base("The mod with the requested ID could not be found.", innerException)
        {
            RequestedModID = requestedModId;
        } 
    }
}