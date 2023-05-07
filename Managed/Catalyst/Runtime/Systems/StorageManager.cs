using System.IO;
using Catalyst.API;
using Catalyst.API.Systems;
using Catalyst.Runtime.Utilities;

namespace Catalyst.Runtime.Systems
{
    internal sealed class StorageManager : IStorageManager
    {
        public string DataStoreDirectory { get; }

        public StorageManager(string dataStoreDirectory)
        {
            DataStoreDirectory = dataStoreDirectory;
        }

        internal string[] RetrieveModFilePaths()
            => Directory.GetFiles(Paths.ModStoreDirectory, "*.zip");
    }
}