using Catalyst.API;
using UnityEngine;

namespace Catalyst.Runtime.Systems
{
    internal sealed class ModManager : MonoBehaviour
    {
        private LogManager _logManager = null!;
        private StorageManager _storageManager = null!;
        private Log _log = null!;
        
        internal ModId? ModId { get; set; }

        private void Awake()
        {
            _logManager = new LogManager(Paths.LogStoreDirectory);
            _storageManager = new StorageManager(Paths.BaseDirectory);
            _log = _logManager.CreateLog(ModId!);

            _log.Info("Catalyst .NET layer starting...");
        }

        private void Update()
        {
        }
    }
}