using System;
using System.Runtime.InteropServices;

namespace Catalyst.Native
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    internal struct CatalystMonoContext
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string ConfigurationFilePath;

        private IntPtr _catalystConfigObject;

        [MarshalAs(UnmanagedType.LPStr)]
        public string LoadAfterAssemblyName;
        
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string UnityMonoDllPath;
        
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string CatalystDllPath;

        private IntPtr _monoLibraryHandle;
        private IntPtr _monoDomainHandle;
        private IntPtr _monoImageHandle;

        public uint CatalystRootGCHandle;
        
        [MarshalAs(UnmanagedType.LPStr)]
        public string Namespace;
        
        [MarshalAs(UnmanagedType.LPStr)]
        public string ClassName;
        
        [MarshalAs(UnmanagedType.LPStr)]
        public string ConstructorDescriptor;
    }
}