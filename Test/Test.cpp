#include <Windows.h>
#include <stdio.h>

int main()
{

	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{

			printf("brrrrrrrr\n");
			Sleep(500);
		}

	}
}
