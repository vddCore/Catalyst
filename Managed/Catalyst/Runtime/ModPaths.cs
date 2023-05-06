using System.IO;
using Catalyst.API;

namespace Catalyst.Runtime
{
    internal class ModPaths
    {
        public ModId ModID { get; }

        public string SettingsPath { get; }
        public string DataPath { get; }

        public ModPaths(ModId modId)
        {
            ModID = modId;

            SettingsPath = Paths.MakeDataStorePath(modId, "settings");
            DataPath = Paths.MakeDataStorePath(modId, "data");
        }

        public void Create()
        {
            Directory.CreateDirectory(SettingsPath);
            Directory.CreateDirectory(DataPath);
        }
    }
}