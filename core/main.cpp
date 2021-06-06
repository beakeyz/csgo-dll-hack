#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "../core/menu/variables.hpp"

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif

	return TRUE;
}

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("csgo-cheat console");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		hooks::initialize();
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "error while initializing hooks!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (true) {

		if (GetAsyncKeyState(VK_END))
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
		
	if (variables::menu::opened) {
		variables::menu::opened = false;
		//wait for paint_traverse::hook to be called and restore input.
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	release();

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}


std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
