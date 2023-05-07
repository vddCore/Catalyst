using System;
using System.Collections.Generic;
using System.Linq;
using Catalyst.API;
using Catalyst.API.Systems;

namespace Catalyst.Runtime.Systems
{
    internal class ModRepository : IModRepository
    {
        private readonly string _modsDirectory;
        
        private readonly Dictionary<ModId, ModContext> _mods = new();
        
        public ModRepository(string modsDirectory)
        {
            _modsDirectory = modsDirectory;
        }

        public object QueryModInstance(ModId modId)
        {
            try
            {
                return _mods[modId].ModInstance;
            }
            catch (Exception e)
            {
                throw new ModNotFoundException(modId, e);
            }
        }

        public T? QueryModInstance<T>(ModId modId) where T : class
        {
            try
            {
                return _mods[modId].ModInstance as T;
            }
            catch (Exception e)
            {
                throw new ModNotFoundException(modId, e);
            }
        }

        public bool TryQueryModInstance(ModId modId, out object mod)
        {
            if (!_mods.TryGetValue(modId, out var context))
            {
                mod = null!;
                return false;
            }

            mod = context.ModInstance;
            return true;
        }

        public bool TryQueryModInstance<T>(ModId modId, out T mod) where T : class
        {
            if (!_mods.TryGetValue(modId, out var context) || context.ModInstance is not T value)
            {
                mod = default!;
                return false;
            }

            mod = value;
            return true;
        }

        public List<object> QueryLoadedMods()
            => new(_mods.Values.Select(x => x.ModInstance));
    }
}