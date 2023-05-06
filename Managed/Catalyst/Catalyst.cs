using System.Threading;
using Catalyst.Runtime;
using Catalyst.Runtime.Systems;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace Catalyst
{
    internal class Catalyst
    {
        private readonly ModId _modId = new("eu.vddcore/Catalyst");

        private GameObject _gameObject = null!;
        private ModManager _modManager = null!;

        public Catalyst()
        {
            ConsoleEx.Redirect();           
            SceneManager.sceneLoaded += OnSceneLoaded;
        }

        private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
        {
            SceneManager.sceneLoaded -= OnSceneLoaded;
            
            _gameObject = new GameObject(_modId);
            Object.DontDestroyOnLoad(_gameObject);
            
            _gameObject.SetActive(false);
            {
                _modManager = _gameObject.AddComponent<ModManager>();
                _modManager.ModId = _modId;
            }
            _gameObject.SetActive(true);
        }
    }
}