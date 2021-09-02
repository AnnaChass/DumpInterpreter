//int Reader::FindNewPacket(uint8_t* buffer) доработать - вдруг data[] кончится
// ЗАМЕНИТЬ STRING НА ДРУГУЮ СТРОКУ
// подсчет crc устарел
// возможно, добавить обнову reader.data при обновлении пакета


#include "Interpreter.h"



int main()
{
	Interpreter interpreter;
	interpreter.Start();

	system("pause");
	return 0;
}