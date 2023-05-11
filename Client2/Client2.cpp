// Client2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <string.h>

using namespace std;
int main()
{
	char machineName[80];
	wchar_t pipeName[80]; //изменили тип переменной на wchar_t
	HANDLE hNamedPipe;
	DWORD dwBytesRead; //для колич прочит байтов
	DWORD dwTotalBytesAvail; //колич байтов в канале
	DWORD dwBytesLeftThisMessage; //колич непрочит байтов
	char pchMessage[80]; //дл соо

	//вводим имя машины в сетм, на которой работает сервак
	cout << "--Enter a name of the server machine-- ('Vvedite imya servernoy mashiny'): ";
	cin >> machineName;
	cin.get();
	//подставляем имя машины в имя канала и преобразуем строку в широкий символ
	wsprintf(pipeName, L"\\\\%S\\pipe\\demo_pipe", machineName);

	//связываемся с именнованным каналом
	hNamedPipe = CreateFile(
		pipeName, //имя канала
		GENERIC_READ | GENERIC_WRITE, //читаем и записываем в канал
		FILE_SHARE_READ | FILE_SHARE_WRITE, //разркшакем чтение и запись
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "--Coonection with the named pipe failed-- ('Ne udalos podklyuchitsya k imenovannomu kanalu')" << endl
			<< "--The last error code-- ('Posledniy kod oshibki'): " << GetLastError() << endl;
		cout << "--Press any key to exit-- ('Nazhmite lyubuyu klavishu dlya vykhoda')";
		cin.get();
		return 0;
	}
	//ждем команду на копирование соо из канала
	cout << "--Press any key to peek a message-- ('Nazhmite lyubuyu klavishu, chtoby prosmotret soobshcheniye')" << endl;
	cin.get();
	//копируем инфо из именованного канала
	if (!PeekNamedPipe(
		hNamedPipe,
		pchMessage,
		sizeof(pchMessage),
		&dwBytesRead,
		&dwTotalBytesAvail,
		&dwBytesLeftThisMessage
	))
	{
		//ошибка чтения соо
		cerr << "--Peek named pipe failed-- ('Ne udalos prosmotret imenovannyy kanal'): " << endl
			<< "--The last error code-- ('Posledniy kod oshibki'): " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "--Pipe any key to exit-- ('Peredayte lyubuyu klavishu dlya vykhoda'): ";
		cin.get();
		return 0;
	}
	//выводим полученное соо на консоль
	if (dwTotalBytesAvail)
		cout << "--The peeked message-- ('Prosmotrennoye soobshcheniye'): "
		<< endl << '\t' << pchMessage << endl;
	else
		cout << "--There is no message-- ('Net soobshcheniya')" << endl;

	// теперь читаем сообщение из именованного канала
	if (!ReadFile(
		hNamedPipe, // дескриптор канала 
		pchMessage, // данные 
		sizeof(pchMessage),// размер данных
		&dwBytesRead, // количество записанных байтов
		NULL)) // синхронное чтение
	{
		// ошибка чтения 
		cerr << "--Read file failed-- ('Oshibka chteniya fayla'): " << endl
			<< "--The last error code-- ('Posledniy kod oshibki'): " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "--Press any key to exit-- ('Nazhmite lyubuyu klavishu dlya vykhoda')";
		cin.get();
		return 0;
	}
	// выводим полученное сообщение на консоль
	cout << "--The client received the message from a server-- ('Kliyent poluchil soobshcheniye ot servera'): "
		<< endl << '\t' << pchMessage << endl;
	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);
	// завершаем процесс
	cout << "--Press any key to exit-- ('Nazhmite lyubuyu klavishu dlya vykhoda')" << endl;
	cin.get();
	return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
