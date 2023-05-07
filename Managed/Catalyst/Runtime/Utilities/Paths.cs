using System.IO;
using UnityEngine;

namespace Catalyst.Runtime.Utilities
{
    internal static class Paths
    {
        public static string BaseDirectory { get; } = Path.Combine(
            Path.GetDirectoryName(Application.dataPath)!,
            "Catalyst"
        );

        public static string DataStoreDirectory { get; } = Path.Combine(
            BaseDirectory,
            "storage"
        );

        public static string ModStoreDirectory { get; } = Path.Combine(
            BaseDirectory,
            "mods"
        );

        public static string LogStoreDirectory { get; } = Path.Combine(
            BaseDirectory,
            "logs"
        );

        static Paths()
        {
            CreateMissingBaseDirectories();
        }

        internal static string MakeDataStorePath(ModId modId, string subDirectory)
        {
            return Path.Combine(
                Path.Combine(
                    DataStoreDirectory,
                    modId.Domain
                ),
                modId.ModName
            );
        }

        private static void CreateMissingBaseDirectories()
        {
            Directory.CreateDirectory(LogStoreDirectory);
            Directory.CreateDirectory(ModStoreDirectory);
            Directory.CreateDirectory(DataStoreDirectory);
        }
    }
}