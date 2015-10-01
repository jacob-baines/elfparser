#ifndef CAPABILITIES_HPP
#define CAPABILITIES_HPP

namespace elf
{
    enum Capabilties
    {
        k_fileFunctions = 0,
        k_networkFunctions,
        k_processManipulation,
        k_pipeFunctions,
        k_crypto,
        k_infoGathering,
        k_envVariables,
        k_permissions,
        k_syslog,
        k_packetSniff,
        k_shell,
        k_packed,
        k_irc,
        k_http,
        k_compression,
        k_ipAddress,
        k_url,
        k_hooking,
        k_antidebug,
        k_dropper
    };
}
#endif
