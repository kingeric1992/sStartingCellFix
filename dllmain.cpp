// dllmain.cpp : Defines the entry point for the DLL application.
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>
#include <RE/Skyrim.h>

using namespace std::literals;

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <xbyak/xbyak.h>

#include "version.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
#ifdef _DEBUG
        while (!IsDebuggerPresent()) Sleep(100);
#endif
    } break;
    }
    return TRUE;
}
//AE
extern "C" __declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.pluginVersion = PACKED_VERSION;
    v.PluginName(PLUGIN_NAME);
    v.AuthorName(AUTHOR);
    v.AuthorEmail(EMAIL);
    v.UsesAddressLibrary();
    v.UsesNoStructs();
    return v;
    }();

//SE
extern "C" __declspec(dllexport) bool SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info)
{
    info->infoVersion = SKSEPlugin_Version.kVersion;
    info->name = SKSEPlugin_Version.pluginName;
    info->version = SKSEPlugin_Version.pluginVersion;
    return !skse->IsEditor();
}
extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse, false);
    struct code_t : Xbyak::CodeGenerator {
        inline code_t(void* buf, size_t size, REL::Relocation<>&& target) : Xbyak::CodeGenerator(size, buf)
        {
            auto _target = (uint8_t*)target.address();

            cmp(qword[rdx + 0x18], 0);
            jnz(getCurr() + 0x7);
            ret();

            db(_target[0]);
            db(_target[1]);
            db(_target[2]);
            db(_target[3]);
            db(_target[4]);
            db(_target[5]);

            jmp(_target + 0x6);

            char c[6];
            {
                c[0] = 0xE9;
                auto offset = (int64_t)buf - (int64_t)_target - 0x5;
                *reinterpret_cast<uint32_t*>(c + 1) = Xbyak::inner::VerifyInInt32( *reinterpret_cast<uint64_t*>(&offset) );
                c[5] = 0x90;
            };
            target.write<char>( c );
        }
    };

    if (REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_1130) != std::strong_ordering::less)
        return true;

    size_t size{ 24 };
    if (auto buf = SKSE::GetTrampolineInterface()->AllocateFromBranchPool(size))
        return code_t(buf, size, REL::Relocation<>{ REL::RelocationID(26498, 27094) }), true;

    SKSE::log::init();
    SKSE::log::info("{} v{} by {}", SKSE::GetPluginName(), SKSE::GetPluginVersion(), SKSEPlugin_Version.GetAuthorEmail());
    SKSE::log::error("failed to alloc buf{} in SKSE branch pool", size);

    return false;
}


