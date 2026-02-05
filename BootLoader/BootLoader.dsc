[Defines]
	PLATFORM_NAME = BootLoader
	PLATFORM_GUID = e407a021-4c2a-421a-9e6b-ee12436f30c1
	PLATFORM_VERSION = 0.0.0.1
	DSC_SPECIFICATION = 0x00010006
	SUPPORTED_ARCHITECTURES = X64
	BUILD_TARGETS = DEBUG|RELEASE

[LibraryClasses]
	UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
	UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
	PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
	PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
	MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
	DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
	BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
	BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
	UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
	DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
	UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
	RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
	StackCheckLib|MdePkg/Library/StackCheckLib/StackCheckLib.inf
	StackCheckFailureHookLib|MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf

[Components]
	BootLoader.inf