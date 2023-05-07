using Catalyst.Runtime;
using Catalyst.Runtime.Systems;
using Catalyst.Runtime.Utilities;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace Catalyst
{
    internal class Catalyst
    {
        private GameObject _gameObject = null!;
        
        
        public static LogManager LogManager { get; } = new(Paths.LogStoreDirectory);
        public static StorageManager StorageManager { get; } = new(Paths.DataStoreDirectory);
        public static ModManager ModManager { get; private set; } = null!;
        
        public static ModId ModID { get; } = new("eu.vddcore/Catalyst");
        public static Log Log { get; } = LogManager.CreateLog(ModID);

        public Catalyst()
        {
            ConsoleEx.Redirect();
            
            Log.Info("Catalyst .NET layer starting. Waiting for Unity to finish initializing.");

            SceneManager.sceneLoaded += OnSceneLoaded;
        }

        private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
        {
            SceneManager.sceneLoaded -= OnSceneLoaded;
            
            _gameObject = new GameObject(ModID);
            Object.DontDestroyOnLoad(_gameObject);
            
            _gameObject.SetActive(false);
            {
                Log.InstallUnityLogHook();
                ModManager = _gameObject.AddComponent<ModManager>();
                ModManager.ModId = ModID;
            }
            _gameObject.SetActive(true);
        }
    }
}