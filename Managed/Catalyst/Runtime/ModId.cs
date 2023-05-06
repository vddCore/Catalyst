using System;
using System.Text;
using System.Text.RegularExpressions;

namespace Catalyst.Runtime
{
    internal class ModId
    {
        private static readonly Regex _regex = new(@"(?<domain>([A-Za-z]+)(\.[A-Za-z]+)?)\/(?<mod_name>[A-Za-z]+)");

        public string Domain { get; }
        public string ModName { get; }
        
        public ModId(string modIdString)
        {
            var match = _regex.Match(modIdString);
            
            if (!match.Success)
            {
                throw new FormatException($"'{modIdString}' is not a valid mod ID.");
            }

            Domain = match.Groups["domain"].Value;
            ModName = match.Groups["mod_name"].Value;
        }

        public static implicit operator string(ModId modId) 
            => modId.ToString();

        public override string ToString()
        {
            var sb = new StringBuilder();

            sb.Append(Domain);
            sb.Append('/');
            sb.Append(ModName);

            return sb.ToString();
        }
    }
}