# imMapper
- A Demonstration on how to abuse the Vulnerable Driver AmdTools64. The Driver has the ability to (only) Read/Write Kernel Space Memory, as well as to Read/Write the MSR. This requires Admin Privileges.

## Exclamation-mark
- This Demonstration is only for Learning Purposes, and I Discourage the Misuse of this vulnerability, as of 8/25/2026 there is no valid CVE and is not in progress, any future CVE will not be of the original author, me.

## Question-mark
- At the address 0x140005380 [Asset/SDDLSnippet](Assets/SDDLSnippet.png) the device object is registered with the SDDL D:P(A;;GA;;;SY)(A;;GA;;;BA), which means only the system and admins can open a handle. The Dispatch Handler at 0x1400058D8 [Asset/DispatcherSnippet](Assets/DispatcherSnippet.png), the Vulnerability lies in it. It gives the usermode the parameters of dangerous operations, like when reading and writing physical memory instead of range and size checking, nothing is validated and everything is usermode passed.

## Add
- [IDA](https://hex-rays.com/ida-pro)

## At
- I'm crediting Leo for helping me with the utilities of this project.
