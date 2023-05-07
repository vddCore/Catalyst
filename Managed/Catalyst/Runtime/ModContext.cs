using System.Reflection;

namespace Catalyst.Runtime
{
    internal class ModContext
    {
        public ModId ModID { get; }
        public ModPaths Paths { get; }
        
        public Assembly AssociatedAssembly { get; }
        public object ModInstance { get; }
        
        public ModContext(ModId modId, Assembly associatedAssembly, object modInstance)
        {
            ModID = modId;
            Paths = new ModPaths(modId);
            
            AssociatedAssembly = associatedAssembly;
            ModInstance = modInstance;
        }
    }
}