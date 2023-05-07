using System.Collections.Generic;
using Catalyst.Runtime;

namespace Catalyst.API.Systems
{
    public interface IModRepository
    {
        object QueryModInstance(ModId modId);
        T? QueryModInstance<T>(ModId modId) where T : class;
        
        bool TryQueryModInstance(ModId modId, out object mod);
        bool TryQueryModInstance<T>(ModId modId, out T mod) where T : class;
        
        List<object> QueryLoadedMods();
    }
}