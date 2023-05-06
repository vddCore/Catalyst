using System;
using System.Runtime.InteropServices;
using Catalyst.Native;
using UnityEngine;

namespace Catalyst
{
    public class Catalyst
    {
        private readonly CatalystMonoContext _monoContext;
        
        public Catalyst(IntPtr catalystMonoContext)
        {
            _monoContext = (CatalystMonoContext)Marshal.PtrToStructure(
                catalystMonoContext,
                typeof(CatalystMonoContext)
            );
            
            Console.WriteLine($"Catalyst: I'm alive! Unity version: {Application.unityVersion}");
            Console.WriteLine($"Catalyst: {catalystMonoContext.ToInt32():X8}");
        }
    }
}