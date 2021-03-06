#pragma once
#include <ntifs.h> 
#include <minwindef.h>  
#include <cstdint>

#define Log(x,...)  DbgPrint( "[Mem hide] " x, __VA_ARGS__)
//	https://www.unknowncheats.me/forum/c-and-c-/467342-kernel-getting-pid-xp-vista-7-8-8-1-10-11-a.html



 
namespace PIDHelp
{

	namespace OffsetHelp
	{

		static UINT OffsetUniqueProcessId = 0x0;
		static UINT OffsetActiveProcessLinks = 0x0;
		static UINT OffsetImageFileName = 0x0;
		static UINT OffsetActiveThreads = 0x0;
		static PEPROCESS myPsInitialSystemProcess = 0x0;

		__forceinline bool InitOffset()
		{


			myPsInitialSystemProcess = PsInitialSystemProcess;


			PEPROCESS SystemProcess = myPsInitialSystemProcess;

			for (int i = 0; i < 0xFFF; i++) // 0xFFF larger than the size of full struct
			{
				if (!OffsetUniqueProcessId && !OffsetActiveProcessLinks)
				{
					if (
					*(UINT64*)((UINT64)SystemProcess + i) == 4&& // 4 always, pid of system process
						*(UINT64*)((UINT64)SystemProcess + i + 0x8) > 0xFFFF000000000000)  // > 0xFFFF000000000000 always
					{
						OffsetUniqueProcessId = i;
						OffsetActiveProcessLinks = i + 0x8;
					}
				}
				if (!OffsetImageFileName && !OffsetActiveThreads)
				{
					if (*(UINT64*)((UINT64)SystemProcess + i) > 0x0000400000000000 && *(UINT64*)((UINT64)SystemProcess + i) < 0x0000800000000000 && // 0x00006D6574737953 always, but better to make range
						*(UINT64*)((UINT64)SystemProcess + i + 0x48) > 0 && *(UINT64*)((UINT64)SystemProcess + i + 0x48) < 0xFFF) // 80 ~ 300 in general
					{
						OffsetImageFileName = i;
						OffsetActiveThreads = i + 0x48;
					}
				}

				if (OffsetUniqueProcessId && OffsetActiveProcessLinks && OffsetImageFileName && OffsetActiveThreads)
				{
					return true;
				}
			}

			return false;
		}

	}

	__forceinline NTSTATUS  GetPEprocessByProcName(IN const CHAR* szProcessName, OUT PEPROCESS* pProcessInfo)
	{


		PEPROCESS SystemProcess = OffsetHelp::myPsInitialSystemProcess;


		PEPROCESS CurrentProcess = SystemProcess;

		do
		{
			if (strstr((CHAR*)((UINT64)CurrentProcess + OffsetHelp::OffsetImageFileName), szProcessName))
			{
				if (*(UINT*)((UINT64)CurrentProcess + OffsetHelp::OffsetActiveThreads))
				{
					*pProcessInfo = CurrentProcess;
					return STATUS_SUCCESS;
				}
			}

			PLIST_ENTRY List = (PLIST_ENTRY)((UINT64)(CurrentProcess)+OffsetHelp::OffsetActiveProcessLinks);
			CurrentProcess = (PEPROCESS)((UINT64)List->Flink - OffsetHelp::OffsetActiveProcessLinks);

		} while (CurrentProcess != SystemProcess);

		return STATUS_NOT_FOUND;


	}



	__forceinline HANDLE GetID(IN const CHAR* szProcessName)
	{



		PEPROCESS SystemProcess = OffsetHelp::myPsInitialSystemProcess;

		PEPROCESS CurrentProcess = SystemProcess;



		do
		{
			if (strstr((CHAR*)((UINT64)CurrentProcess + OffsetHelp::OffsetImageFileName), szProcessName))
			{
				if (*(ULONG*)((UINT64)CurrentProcess + OffsetHelp::OffsetActiveThreads))
				{


					return *(HANDLE*)((UINT64)CurrentProcess + OffsetHelp::OffsetUniqueProcessId);
				}
			}

			PLIST_ENTRY List = (PLIST_ENTRY)((UINT64)(CurrentProcess)+OffsetHelp::OffsetActiveProcessLinks);
			CurrentProcess = (PEPROCESS)((UINT64)List->Flink - OffsetHelp::OffsetActiveProcessLinks);

		} while (CurrentProcess != SystemProcess);

		return 0;


	}



	__forceinline NTSTATUS GetEProcessByProcId(HANDLE procId, OUT PEPROCESS* pProcessInfo)
	{

		PEPROCESS SystemProcess = OffsetHelp::myPsInitialSystemProcess;

		PEPROCESS CurrentProcess = SystemProcess;



		do
		{
			if (*(uint64_t*)((UINT64)CurrentProcess + OffsetHelp::OffsetUniqueProcessId) == (uint64_t)procId)
			{
				if (*(UINT*)((UINT64)CurrentProcess + OffsetHelp::OffsetActiveThreads))
				{
					*pProcessInfo = CurrentProcess;
					return STATUS_SUCCESS;
				}
			}

			PLIST_ENTRY List = (PLIST_ENTRY)((UINT64)(CurrentProcess)+OffsetHelp::OffsetActiveProcessLinks);
			CurrentProcess = (PEPROCESS)((UINT64)List->Flink - OffsetHelp::OffsetActiveProcessLinks);

		} while (CurrentProcess != SystemProcess);

		return STATUS_NOT_FOUND;




	}



}