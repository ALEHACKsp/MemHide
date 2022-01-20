#include "PIDHelp.h"

namespace MemHide
{
	__forceinline bool SetFakeProcessId(HANDLE procId, const char* procNameParsePID)
	{
		PEPROCESS proc;

		if (procId && NT_SUCCESS(PIDHelp::GetEProcessByProcId(procId, &proc)))
		{
			auto procIdWinlogon = PIDHelp::GetID(procNameParsePID);

			*(HANDLE*)((uint64_t)proc + PIDHelp::OffsetHelp::OffsetUniqueProcessId) = procIdWinlogon;

			Log("Old process id ->\t%d\n", procId);
			Log("New process id ->\t%d\n", *(HANDLE*)((uint64_t)proc + PIDHelp::OffsetHelp::OffsetUniqueProcessId));

			return procId != *(HANDLE*)((uint64_t)proc + PIDHelp::OffsetHelp::OffsetUniqueProcessId);

		}
		return false;
	}
}