using System;
using System.Text;
using System.Text.RegularExpressions;

namespace Catalyst.Runtime
{
    public class ModId : IEquatable<ModId>
    {
        private static readonly Regex _regex = new(@"(?<domain>([A-Za-z]+)(\.[A-Za-z]+)?)\/(?<mod_name>[A-Za-z]+)");

        public string Domain { get; }
        public string ModName { get; }
        
        internal ModId(string modIdString)
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

        public static implicit operator ModId(string str)
            => new(str);

        public override string ToString()
        {
            var sb = new StringBuilder();

            sb.Append(Domain);
            sb.Append('/');
            sb.Append(ModName);

            return sb.ToString();
        }

        public bool Equals(ModId? other)
        {
            return Domain == other?.Domain
                && ModName == other.ModName;
        }

        public override bool Equals(object? obj)
        {
            return (obj is ModId modId)
                && Equals(modId);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (Domain.GetHashCode() * 397) ^ ModName.GetHashCode();
            }
        }

        public static bool operator ==(ModId? left, ModId? right) 
            => Equals(left, right);

        public static bool operator !=(ModId? left, ModId? right) 
            => !Equals(left, right);
    }
}