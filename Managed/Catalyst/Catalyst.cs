using System;
using UnityEngine;

namespace Catalyst
{
    public class Catalyst
    {
        public Catalyst(IntPtr catalystMonoContext)
        {
            Console.WriteLine($"Catalyst: I'm alive! Unity version: {Application.unityVersion}");
            Console.WriteLine($"Catalyst: {catalystMonoContext.ToInt32():X8}");
        }
    }
}