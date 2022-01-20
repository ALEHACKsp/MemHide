#include  "SetFakePID.h"

NTSTATUS DriverEntry(uint64_t baseNtoskrnl, IN PUNICODE_STRING pRegistryPath)
{
	
	
	if (PIDHelp::OffsetHelp::InitOffset())
	{
		
		auto procIdTestProg = PIDHelp::GetID("Test.exe");
		if (procIdTestProg)
		{


			Log("Is hide process ->\t%x\n", MemHide::SetFakeProcessId(procIdTestProg, "winlogon.exe"));
		} 
			
	}
	else
	{
		Log("Can\'t init offset");
	}
	
	
	return STATUS_SUCCESS;
}